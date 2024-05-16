/*******************************************************************************
 *
 * mod_file.c
 *
 ******************************************************************************/

#include "src/term_cmd_input.h"
#include "src/cmd_processor.h"
#include "src/xfs_printf.h"
#include "src/xfs_utils.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "src/xfs_dbg.h"
#include "frida.h"


/******************************************************************************/

#define IS_FILE 0x01
#define IS_DIR  0x02


/******************************************************************************/

static char root_path[]    = "/";
static char current_path[] = ".";



/******************************************************************************/

int exists(char *path)
{
  FILINFO fi;

  if ( ((path[0] == '.') || (path[0] == '/')) && (path[1] == 0) ) return IS_DIR; // not covered by f_stat

  if (f_stat(path, &fi) == FR_OK)
  {
    return (fi.fattrib & AM_DIR) ? IS_DIR : IS_FILE;
  }

  return 0;
}


/******************************************************************************/

int trim_slashes(char *path)
{
  /* trim trailing slashes in place but keep a '/' at beginning in case of e.g. '///' */
  int i = strlen(path) - 1;
  while ((i > 0) && (path[i])=='/') i--;
  path[i+1] = 0;
  return i;
}


/******************************************************************************/

char* basename(char *path)
{
  int i = trim_slashes(path);
  while ((i >= 0) && (path[i])!='/') i--;
  return path+i+1;
}


/******************************************************************************/

char* dirname(char *path)
{
  int i = trim_slashes(path);
  while ((i > 0) && (path[i]) != '/') i--;
  if (i>0) path[i] = 0;
  else if (path[0] == '/') return root_path;
  else return current_path;
  return path;
}


/******************************************************************************/

static int contains_whitespace(char *s)
{
  int i;
  for (i=0; s[i]; i++) if ((s[i]==' ') || (s[i]==0x09)) return 1;
  return 0;
}


/******************************************************************************/

int cmd_ls(int argc, char **argv)
{
  CMD_HELP("[<path>]", "list files",
           "The last segment of path may contain * as wildcard character\r\n"
  );

  FRESULT res;
  DIR  dir;
  FILINFO fi;
  DWORD nclst;
  FATFS *fs;

  char def_path[]    = ".";
  char def_pattern[] = "*";
  char label[12];
  char *path    = def_path;
  char *pattern = def_pattern;
  int  files  = 0;
  int  dirs   = 0;
  char quote;

  if (argc > 1)
  {
    path = argv[1];
    trim_slashes(path);
    if (exists(path) != IS_DIR)
    {
      pattern = basename(path);
      path    = dirname(path);
    }
    if (strcmp(pattern, def_path) == 0) pattern = def_pattern;
  }

  res = f_findfirst(&dir, &fi, path, pattern);
  if (res != FR_OK) {
     xfs_printf("Path '%s' not found\r\n", path);
     // f_closedir(&dir); // ?
     return -1;
  }

  res = f_getlabel((TCHAR const*)SDPath, label, NULL);
  if (res != FR_OK) label[0] = 0;
  xfs_printf("Listing '%s' in path '%s' on disk '%s'\r\n\r\n", pattern, path, label);

  do {
    if (fi.fname[0] == 0) break;
    quote = (contains_whitespace(fi.fname)) ? '\'' : ' ';

    if (fi.fattrib & AM_DIR)
    {
      xfs_printf("   <DIR> %c%s%c\r\n", quote, fi.fname, quote);
      dirs++;
    }
    else
    {
      xfs_printf("%8u %c%s%c\r\n", fi.fsize, quote, fi.fname, quote);
      files++;
    }
    res = f_findnext(&dir, &fi);
  } while (res == FR_OK);

  f_closedir(&dir);
  if (files + dirs) xfs_printf("\r\n");

  xfs_printf(" %d file%s,  %d director%s\r\n", files, ((files==1)?"":"s"), dirs, ((dirs==1)?"y":"ies"));

  // fatfs.free_clst = fatfs.n_fatent; // FORCE full FAT scan // now changed in ff.c
  res = f_getfree((TCHAR const*)SDPath, &nclst, &fs);
  if (res == FR_OK) xfs_printf("%u bytes free (%u cluster of %u byte)\r\n", nclst * fs->csize * _MIN_SS, nclst, fs->csize * _MIN_SS);

  return 0;
}




/******************************************************************************/

int cmd_cd(int argc, char **argv)
{
  CMD_HELP("[<dir>]", "change directory");

  FRESULT res;
  char    *path = (argc >1) ? argv[1] : "/";

  res = f_chdir (path);
  if (res != FR_OK) xfs_printf("could not change to '%s'\r\n", path);

  return 0;
}


/******************************************************************************/

int cmd_pwd(int argc, char **argv)
{
  CMD_HELP("", "print working directoy");

  FRESULT res;
  TCHAR   buff[256];

  res =  f_getcwd (buff, 256);
  if (res != FR_OK) xfs_printf("could not get current directory\r\n");
  else xfs_printf("%s\r\n",buff);

  return 0;
}


/******************************************************************************/


#define LOAD_BUFF_LEN 512


/******************************************************************************/

int cmd_cat(int argc, char **argv)
{
  CMD_HELP("<file>", "show ascii contents of <file>");

  FIL     file;
  TCHAR   buff[LOAD_BUFF_LEN+1]; // add 1 char for termination
  FRESULT res;
  UINT    n;
  int     i;

  res = f_open (&file, argv[1], FA_READ);
  if (res != FR_OK)
  {
    xfs_printf("could not open file '%s'\r\n", argv[1]);
    return -1;
  }

  for (; ((f_read(&file, buff, LOAD_BUFF_LEN, &n) == FR_OK) && (n > 0)); )
  {
    // replace non printable chars with dot
    for (i=0; i < n; i++) if (((buff[i] < 32) && (buff[i] != '\r') && (buff[i] != '\n'))|| (buff[i] > 126)) buff[i] = '.';
    buff[n] = 0; // terminate string buffer
    xfs_print(buff);
  }

  xfs_print("\r\n");
  f_close(&file);

  return 0;
}


/******************************************************************************/

void mg_hexdump(const void *buf, size_t len);

int cmd_hexdump(int argc, char **argv)
{
  CMD_HELP("<file>", "show hexdump of <file>");

  FIL     file;
  TCHAR   buff[LOAD_BUFF_LEN+1]; // add 1 char for termination
  FRESULT res;
  UINT    n = 1;

  res = f_open (&file, argv[1], FA_READ);
  if (res != FR_OK)
  {
    xfs_printf("could not open file '%s'\r\n", argv[1]);
    return -1;
  }

  for (; ((f_read(&file, buff, LOAD_BUFF_LEN, &n) == FR_OK) && (n > 0)); )
  {
    // TBD: use other hexdump function to get correct buffer offset printing
    mg_hexdump(buff, n);
  }

  xfs_print("\r\n");
  f_close(&file);

  return 0;
}



/******************************************************************************/

#define CMD_BUFF_LEN  128

int cmd_source(int argc, char **argv)
{
  CMD_HELP("<file>", "execute <file> as command script");

  FIL     file;
  TCHAR   buff[LOAD_BUFF_LEN+1]; // add 1 char for termination
  FRESULT res;
  UINT    n = 1;

  char tmp_buffer[CMD_BUFF_LEN];
  char *cmd_buff = tmp_buffer;
  int  cmd_ptr = 0;
  int  comment = 0;
  int  dquote  = 0;
  int  squote  = 0;
  int  i;

  res = f_open (&file, argv[1], FA_READ);
  if (res != FR_OK)
  {
    xfs_printf("could not open file '%s'\r\n", argv[1]);
    return -1;
  }

  if (DBG_INF1) xfs_printf("#> sourcing script '%s'\r\n", argv[1]);

  for (; ((f_read(&file, buff, LOAD_BUFF_LEN, &n) == FR_OK) && (n > 0)); )
  {
    for (i=0; i<n; i++)
    {
      if ((buff[i] == 0x0a) || (buff[i] == 0x0d) || (buff[i] == 0x00))
      {
        // found termination
        if (cmd_ptr > 0)
        {
          if (cmd_ptr < CMD_BUFF_LEN)
          {
            cmd_ptr = skip_whitespace(cmd_buff, cmd_ptr-1, 1);
            cmd_buff[cmd_ptr+1]=0;

            /* remove matching quotes around cmd */
            if (((cmd_buff[0]=='"' && cmd_buff[cmd_ptr]=='"') || (cmd_buff[0]=='\'' && cmd_buff[cmd_ptr]=='\'')))
            {
              cmd_buff[cmd_ptr]=0;
              cmd_buff++;
            }
            if (DBG_INF1) xfs_printf("#> '%s'\r\n", cmd_buff);
            cmd_process(cmd_buff, cmd_ptr);
          }
        }
        comment = 0;
        squote  = 0;
        dquote  = 0;
        cmd_ptr = 0;
      }
      else if ((!cmd_ptr) && ((buff[i] == ' ') || (buff[i] == 0x09)))
      {
        // skip whitespace at beginning of line
      }
      else if ((cmd_ptr < CMD_BUFF_LEN) && (!comment))
      {
        if      ((buff[i]=='\'') && (!dquote)) squote = !squote;
        else if ((buff[i]=='"')  && (!squote)) dquote = !dquote;
        else if ((buff[i] == '#')  && (!dquote)  && (!squote)) comment = 1;

        if (!comment) cmd_buff[cmd_ptr++] = buff[i];
      }
    }
  }

  xfs_print("\r\n");
  f_close(&file);

  return 0;
}


/******************************************************************************/

int cmd_mkdir(int argc, char **argv)
{
  CMD_HELP("<dir>", "make directory");

  FRESULT res;

  res = f_mkdir(argv[1]);
  if (res != FR_OK) xfs_printf("could not make dir '%s'\r\n", argv[1]);

  return 0;
}


/******************************************************************************/

int cmd_rm(int argc, char **argv)
{
  CMD_HELP("<path>", "remove file or empty directory");

  FRESULT res;

  res = f_unlink(argv[1]);
  if (res != FR_OK) xfs_printf("could not remove dir '%s'\r\n", argv[1]);

  return 0;
}


/******************************************************************************/

int cmd_mv(int argc, char **argv)
{
  CMD_HELP("<old> <new>", "mv / rename file or directory");

  FRESULT res;
  int exists_src,  exists_dst;
  char *src = argv[1]; // alias for convinience
  char *dst = argv[2];
  char buffer[_MAX_LFN];

  trim_slashes(src); // note: argv[1] may be modified as src is pointer to argv[1]
  trim_slashes(dst); // note: argv[2] may be modified as dst is pointer to argv[2]

  exists_src = exists(src);
  if (!exists_src)
  {
    xfs_printf("source '%s' not found\r\n", src);
    return -1;
  }
  exists_dst = exists(argv[2]);

  if (exists_dst == IS_FILE)
  {
    xfs_printf("destination file '%s' exists\r\n", dst);
    return -1;
  }
  else if (exists_dst == IS_DIR)
  {
    xfs_snprintf(buffer, _MAX_LFN, "%s/%s", dst, basename(src));
    dst = buffer;
  }

  // xfs_printf("mv '%s' to '%s'\r\n", src, dst);

  res = f_rename(src, dst);
  if (res != FR_OK) xfs_printf("could not move '%s' to '%s'\r\n", src, dst);

  return 0;
}


/******************************************************************************/

int cmd_label(int argc, char **argv)
{
  CMD_HELP("[<new_label>]", "show or change label of disk");

  FRESULT res;
  char  label[13];

  if (argc > 1) // set new label
  {
    res = f_setlabel(argv[1]);
    if (res != FR_OK) xfs_printf("could not set label '%s'\r\n", argv[1]);
  }

  res = f_getlabel((TCHAR const*)SDPath, label, NULL);
  if (res != FR_OK) label[0] = 0;
  xfs_printf("disk label is '%s'\r\n", label);

  return 0;
}


/******************************************************************************/

#define WORK_BUFF_LEN _MAX_SS
static char mkfs_security_phrase[] = "ERASE-ALL";
int cmd_mkfs(int argc, char **argv)
{
  CMD_HELP(mkfs_security_phrase, "format drive and erase all data");

  FRESULT res;
  char  buff[WORK_BUFF_LEN];

  if ((argc != 2) || (strcmp(argv[1], mkfs_security_phrase)))
  {
    xfs_printf("type 'mkfs %s' if you really want to format drive and erase all data\r\n", mkfs_security_phrase);
    return -1;
  }

  res = f_mount(NULL, (TCHAR const*)SDPath, 0);
  if (res != FR_OK)
  {
     xfs_printf("could not unmount filesystem\r\n");
     return -1;
  }

  // use option FM_SFD : start filesystem at sector 0 without partition table
  res = f_mkfs((TCHAR const*)SDPath, FM_ANY | FM_SFD, _MIN_SS, buff, WORK_BUFF_LEN);
  if (res != FR_OK)
  {
     xfs_printf("making filesystem failed\r\n");
     return -1;
  }

  res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
  if (res != FR_OK)
  {
     xfs_printf("could not mount new filesystem\r\n");
     return -1;
  }

  res = f_setlabel(DRIVE_LABEL_NAME);
  if (res != FR_OK)
  {
     xfs_printf("failed setting label\r\n");
  }
  return 0;
}


/******************************************************************************/

int write_string_to_file(char* s, char *fname, int append)
{
  FRESULT res;
  FIL     file;
  UINT    len, written;
  BYTE    mode = FA_WRITE;
  int rc = 0;

  // FA_OPEN_ALWAYS| FA_CREATE_ALWAYS
  mode |= append ? FA_OPEN_APPEND : FA_CREATE_ALWAYS;
  res = f_open(&file, fname, mode);
  if (res != FR_OK)
  {
    xfs_printf("could not open file '%s'\r\n", fname);
    return -1;
  }

  len = strlen(s);

  res = f_write(&file, s, len, &written);
  if ((res != FR_OK) || (written != len))
  {
    xfs_printf("failed writing to file '%s'\r\n", fname);
    rc = -1;
  }
  else
  {
    if (DBG_INF2) xfs_printf("written %d of %d byte to file '%s'.\r\n", written, len, fname);
  }

  f_close(&file);
  return rc;
}


/******************************************************************************/

static int check_invalid_char(char c)
{
  char invalid_chars[] = "<>'\":\\`|&";
  int i;

  if ((c < 32) || (c>126)) return 1;
  for (i=0;i < sizeof(invalid_chars); i++) if (c == invalid_chars[i]) return 1;
  return 0;
}

/******************************************************************************/

static int check_valid_fname(char *s, int start, int end)
{
  int i;
  if (start > end) return 0;
  for (i=start; i<=end; i++) if (check_invalid_char(s[i])) return 0;
  return 1;
}


/******************************************************************************/

int check_redirection(char *s, char** str, char** fname)
{
  int i;
  int fn_start,  fn_end;
  int str_start = 0;
  int str_end;
  int newline;
  int rpos = -1;
  int append = 0;

  for (i=0; s[i]; i++) if (s[i]=='>') rpos = i;  // check position of last '>'
  if (rpos < 0) return 0;

  fn_end   = skip_whitespace(s, i-1, 1);    // skip whitespace at end of string
  fn_start = skip_whitespace(s, rpos+1, 0); // skip whitespace after '>'

  newline = ((fn_start-rpos) > 1);  // insert newline if there is at least one space between '>' and filename

  if ((rpos > 0) && (s[rpos-1] == '>')) {append = 1; rpos--;} // checko for append and skip possible second '>'

  /* remove mathcing quotes from filename */
  if ((s[fn_start]=='"' && s[fn_end]=='"') || (s[fn_start]=='\'' && s[fn_end]=='\''))
  {
     fn_start++; fn_end--;
  }


  if (check_valid_fname(s, fn_start, fn_end))
  {
    s[fn_end+1] = 0;  // terminate file name

    str_end = (rpos > 0) ? skip_whitespace(s, rpos-1, 1) : -1;
    /* remove mathcing quotes from strin */
    if ((str_end > 0) && ((s[str_start]=='"' && s[str_end]=='"') || (s[str_start]=='\'' && s[str_end]=='\'')))
    {
      str_start++;
      str_end--;
    }

    if (newline)
    {
      s[str_end+1] = '\n';
      s[str_end+2] = 0;
    }
    else
    {
      s[str_end+1] = 0;
    }
    *str = s+str_start;
    *fname = s+fn_start;
    return append;
  }
  *fname = 0;
  return 0;
}


/******************************************************************************/

int cmd_echo_redir(int argc, char **argv)
{
  CMD_HELP("[<text>] [>[>][ ]<file>]", "display a line of text or write/append to file",
           " >  creates a new file or overwrites !!! existing file\r\n"
           " >> creates a new file or appends to existing file\r\n"
           " when there is at least one space  between '>' and\r\n"
           " the filename, a newline '\\n' is appended after <text>\r\n"
  );

  char* str = 0;
  char* fname = 0;
  int append;

  if (argc < 2)
  {
    xfs_printf("\r\n");
    return 0;
  }

  append = check_redirection(argv[1], &str, &fname);
  if (fname)
  {
  //  xfs_printf("redirecting '%s'\r\n to '%s' append=%d\r\n", str, fname, append);
    write_string_to_file(str, fname, append);
  }
  else
  {
    xfs_printf("%s\r\n", argv[1]);
  }
  return 0;
}

/******************************************************************************/


#define CP_BUFF_LEN _MIN_SS

int cmd_cp(int argc, char **argv)
{
  CMD_HELP("<src> <dst>", "copy file <src> to <dst>");

  FRESULT res;
  FIL     fsrc;
  FIL     fdst;
  UINT    written;
  int  exists_dst;
  char buff[CP_BUFF_LEN];
  int rc = 0;
  char *src = argv[1]; // alias for convinience
  char *dst = argv[2];
  UINT n=1;

  trim_slashes(src); // note: argv[1] may be modified as src is pointer to argv[1]
  trim_slashes(dst); // note: argv[2] may be modified as dst is pointer to argv[2]

  if (exists(src) != IS_FILE)
  {
    xfs_printf("source file '%s' not found\r\n", src);
    return -1;
  }

  exists_dst = exists(dst);

  if (exists_dst == IS_FILE)
  {
    xfs_printf("destination file '%s' exists\r\n", dst);
    return -1;
  }
  else if (exists_dst == IS_DIR)
  {
    xfs_snprintf(buff, CP_BUFF_LEN, "%s/%s", dst, basename(src));
    dst = buff;
  }

  // xfs_printf("cp '%s' to '%s'\r\n", src, dst);

  res = f_open (&fsrc, src, FA_READ);
  if (res != FR_OK)
  {
    xfs_printf("could not open source file '%s'\r\n", fsrc);
    return -1;
  }

  res = f_open(&fdst, dst, FA_WRITE | FA_CREATE_ALWAYS );
  if (res != FR_OK)
  {
    xfs_printf("could not create destination file '%s'\r\n", dst);
    f_close(&fsrc);
    return -1;
  }

  while ((res == FR_OK) && (n > 0)) {
    res = f_read(&fsrc, buff, CP_BUFF_LEN, &n);
    if ((res == FR_OK) && (n > 0) && (n <= CP_BUFF_LEN))
    {
      res = f_write(&fdst, buff, n, &written);
      if ((res != FR_OK) || (written != n))
      {
        xfs_printf("error writing to destination file'\r\n");
        rc = -1;
        break;
      }
    }
  }

  f_close(&fsrc);
  f_close(&fdst);

  return rc;
}


/******************************************************************************/

int mod_file_help(int argc, char **argv)
{
  CMD_HELP("","filesystem related commands");
  return 0;
}


/******************************************************************************/

cmd_table_entry_type cmd_table_mod_file[] =
{
  {0, "mod_file",  mod_file_help},  /* first entry is module name and help */
  {1, "ls",        cmd_ls},
  {1, "dir",       cmd_ls},
  {1, "label",     cmd_label},
  {1, "cd",        cmd_cd},
  {0, "pwd",       cmd_pwd},
  {2, "cp",        cmd_cp},
  {2, "mv",        cmd_mv},
  {2, "rename",    cmd_mv},
  {1, "rm",        cmd_rm},
  {1, "del",       cmd_rm},
  {1, "cat",       cmd_cat},
  {1, "hexdump",   cmd_hexdump},
  {1, "echo",      cmd_echo_redir},
  {1, "mkdir",     cmd_mkdir},
  {1, "mkfs",      cmd_mkfs},
  {1, "source",    cmd_source},
  {1, ".",         cmd_source},
  {0, 0, 0}
};

/******************************************************************************/
