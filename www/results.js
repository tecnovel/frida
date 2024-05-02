
var log_IsOpen = false;
var log_txt = "";
var log_entry = 0;




function _log_Flush(scroll = true)
{
    const res = document.getElementById("results");
    res.innerHTML += log_txt;
    log_txt = "";
    if (scroll) res.scrollTop = res.scrollHeight - res.clientHeight;
}

function log_DeleteAll()
{
    document.getElementById("results").innerHTML = "";
    log_txt = "";
    log_entry = 0;
}


function log_Open(title)
{
    log_IsOpen = true;
    const date = new Date();
    let year = date.getFullYear().toString();
    let month = (date.getMonth()+1).toString().padStart(2.0);
    let day = date.getDate().toString().padStart(2,0);
    let hour = date.getHours().toString().padStart(2,0);
    let min = date.getMinutes().toString().padStart(2,0);
    let sec = date.getSeconds().toString().padStart(2,0);
    let ms = date.getMilliseconds().toString().padStart(3,0);

    log_Add( 
`<div class="log-entry" id="logentry-${log_entry}">
<div class="log-header" id="logheader-${log_entry}" onclick="log_ShowHide(${log_entry})">
  ${log_entry}: ${year}-${month}-${day} ${hour}:${min}:${sec}.${ms} - ${title}
</div>
<div class="log-content" id="logcontent-${log_entry}">\n`);
}

function log_Add(s)
{
    log_txt += s;
}

function log_Close()
{
    if (!log_IsOpen) return
    log_Add('</div></div>\n')
    _log_Flush(true);
    log_entry += 1;
    log_IsOpen = false;
}

function log_ShowHide(id)
{
    const el = document.getElementById('logcontent-'+ id);
    el.className = (el.className == 'log-content')? 'log-content-hidden' : 'log-content';
}

function log_ShowAll()
{
    const elList = document.querySelectorAll('.log-content-hidden').forEach( (el) =>
        { el.className = 'log-content'; });
}

function log_HideAll()
{
    const elList = document.querySelectorAll('.log-content').forEach( (el) =>
        { el.className = 'log-content-hidden'; });
}
