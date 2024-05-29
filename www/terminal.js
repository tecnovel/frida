
// Create a WebSocket connection to the server
//const ws = new WebSocket('ws://localhost:8765');
const ws = new WebSocket('ws://frida.psi/ws');
let container = document.getElementById('container');

ws.onopen = () => {
  console.log('WebSocket connection established');
  ws.send('\x1b[H'); // Send Home key to move cursor to start of line
  ws.send('\x0B'); // Send Ctrl+K clear to end of line
  ws.send('\x07'); // Send Ctrl+G refresh prompt
};

var cmdline_pre = document.getElementById("cmdline_pre");
var cmdline_after = document.getElementById("cmdline_after");
var histroy = document.getElementById("history");

let cursorpos = 0; // Cursor position in the input line
let escapeBuffer = ''; // Buffer to hold incoming escape sequences

ws.onmessage = (e) => {
  let scroll = false;
  let text_pre = cmdline_pre.textContent;
  let text_after = cmdline_after.textContent;

  // Process each character in the received data
  for (let i = 0; i < e.data.length; i++) {
    const charCode = e.data.charCodeAt(i);
    console.log('Char:', charCode, e.data.charAt(i));

    // Append to escape buffer if we detect the start of an escape sequence
    if (charCode === 0x1b || escapeBuffer.length > 0) {
      escapeBuffer += e.data.charAt(i);

      // Regex to match CSI sequences like \x1b[...D
      if (/\x1b\[\d*[A-Z]/.test(escapeBuffer)) {
        // Handle the complete escape sequence
        let match = escapeBuffer.match(/\x1b\[(\d*)?([A-Z])/);
        let num = match[1];
        let command = match[2];

        console.log('Escape:', command, num);


        switch (command) {
          case 'K': // End key - erase to end of line
            n = parseInt(num, 10);
            if(isNaN(n)){
              text_after = '';
            } else if(n == 1) {
              text_pre = '';
              cursorpos = 0;}
            else if(n == 2){
              text_pre = '';
              text_after = '';
              cursorpos = 0;
            }
            break;
          case 'J': // Clear screen
            n = parseInt(num, 10);
            if(num == 2){
              histroy.textContent = '';
              text_pre = '';
              text_after = '';
            }
            break;
          case 'H': // Home key - move to start of line
            text_after = text_pre + text_after;
            text_pre = '';
            break;
            case 'C': // Right arrow
            let moveRight = num ? parseInt(num, 10) : 1; // Default to 1 if no number provided
            moveRight = Math.min(moveRight, text_after.length); // Do not move beyond available characters
            for (let j = 0; j < moveRight; j++) {
              text_pre += text_after.charAt(0);
              text_after = text_after.slice(1);
            }
            cursorpos = Math.min(cursorpos + moveRight, text_pre.length); // Safely update cursor position
            break;
          
          case 'D': // Move cursor left
            let moveLeft = num ? parseInt(num, 10) : 1; // Default to 1 if no number provided
            moveLeft = Math.min(moveLeft, text_pre.length); // Do not move beyond available characters
            for (let j = 0; j < moveLeft; j++) {
              text_after = text_pre.charAt(text_pre.length - 1) + text_after;
              text_pre = text_pre.slice(0, -1);
            }
            cursorpos = Math.max(0, cursorpos - moveLeft); // Ensure cursor does not go below zero
            break;
          default:
            console.log('Unhandled escape:', command);
            break;
        }
        // Clear the escape sequence buffer after processing
        escapeBuffer = '';
        continue;
      } else {
        // Continue buffering if sequence is incomplete
        continue;
      }
    }

    switch (charCode) {
      case 0x08: // Handle backspace
        if (cursorpos > 0) {
          text_pre = text_pre.slice(0, -1);
          cursorpos--;
        }
        break;
      case 0x0d: // Handle carriage return
        cursorpos = 0;
        text_after = text_pre + text_after;
        text_pre = '';
        break;
      case 0x0a: // Handle line feed (usually do nothing)
        histroy.textContent += text_pre + text_after + '\n';
        scroll = true;
        text_pre = '';
        text_after = '';
        break;
      default: // Handle regular characters
        text_pre += String.fromCharCode(charCode);
        text_after = text_after.slice(1);
        cursorpos++;
        break;
    }
  }

  cmdline_pre.textContent = text_pre;
  cmdline_after.textContent = text_after;
  if (scroll)
  container.scrollTop = container.scrollHeight;
};


ws.onclose = () => {
  console.log('WebSocket connection closed');
};

ws.onerror = (error) => {
  console.error('WebSocket error:', error);
};

document.addEventListener('keydown', (e) => {
  e.preventDefault(); // Prevent default action for all keydown events

  if (e.ctrlKey && e.key === 'k') {  // Handle Ctrl+K
    ws.send('\x0B');
    return;
  } else if (e.ctrlKey && e.key === 'l') {  // Handle Ctrl+L
    ws.send('\x0C');
    return;
  } else if (e.ctrlKey && e.key === 'c') {  // Handle Ctrl+C
    if(window.getSelection().toString().length){
      let exactText = window.getSelection().toString();        
      navigator.clipboard.writeText(exactText);
   }
    return;
  } else if (e.ctrlKey && e.key === 'v') {  // Handle Ctrl+V
    navigator.clipboard.readText().then((text) => {
      ws.send(text);
    });
    return;
  } else if (e.ctrlKey && e.key === 'r') {  // Handle Ctrl+R
    document.location.reload();
    return;
  } else if (e.ctrlKey && e.key === 'g') {  // Handle Ctrl+G
    ws.send('\x07');
    return;
  }else if (e.ctrlKey && e.key === 'u') {  // Handle Ctrl+U
    ws.send('\x15');
    return;
  }

  // Handling other key combinations and single key presses
  switch (e.key) {
    case 'Enter':
      ws.send('\r');
      break;
    case 'Backspace':
      ws.send('\b');
      break;
    case 'ArrowUp':
      ws.send('\x1b[A');
      break;
    case 'ArrowDown':
      ws.send('\x1b[B');
      break;
    case 'ArrowLeft':
      ws.send('\x1b[D');
      break;
    case 'ArrowRight':
      ws.send('\x1b[C');
      break;
    case 'Home':
      ws.send('\x1b[H');
      break;
    case 'End':
      ws.send('\x1b[F');
      break;
    case 'Delete':
      ws.send('\x1b[3~');
      break;
    case 'Escape':
      ws.send('\x1b');
      break;
    case 'Tab':
      ws.send('\t');
      break;
    case 'Dead':
      ws.send('`');
      break;
    case 'CapsLock':
      break;
    case 'Shift':
      break;
    default:
      if (!e.ctrlKey) {  // Send the character as it is, only if Ctrl is not pressed
        ws.send(e.key);
      }
      break;
  }
});
