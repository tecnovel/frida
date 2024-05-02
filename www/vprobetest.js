
// === tab animation ==========================================================

function openTab(evt, tabName) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
      tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
      tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "flex";
    evt.currentTarget.className += " active";
}


// === status control, error messages =========================================

const Status = { off:0, on:1, run:2 }
var stat = Status.off;
var connected = false;
let ws = null;

function DebugMsg(msg)
{
    document.getElementById("debug").value = msg;
}

function SetStatus(newStatus)
{
    stat = newStatus;
    const img = document.getElementById('statusimg');
    switch (stat)
    {
        case Status.off: img.src = 'red.png'; break;
        case Status.on:  img.src = 'green.png'; break;
        case Status.run: img.src = 'gear_running.gif'; break;
    }
}

function RequestOpen(allowDisconnected = false)
{
    if (stat == Status.off && !allowDisconnected)
    {
        DebugMsg('not connected');
        return false;
    }
    if (stat == Status.run)
    {
        DebugMsg('still running');
        return false;
    }
    SetStatus(Status.run);
    return true;
}

function RequestClose()
{
    SetStatus(connected?Status.on:Status.off);
}


// === server request =========================================================

function DummyRun(title, delay)
{
    if (!RequestOpen()) return;
    const run = setTimeout( ()=> {
        log_Open(title);
        log_Add('not yet implemented');
        log_Close();
        DebugMsg('ok');
        RequestClose();
    }, delay);
}

/*function serverSend(req, handlerResponse, allowDisconnected=false) {
    if (!RequestOpen(allowDisconnected)) return;
    fetch("/vp?" + req, {
        method: 'POST',
    }).then((response) => {
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        return response.json(); // response.json() or response.text() or whatever the server sends
    }).then((body) => {
        if (body['status'] == 0) throw new Error(`VProbe: ${body['msg']}`);
        DebugMsg('ok');
        RequestClose();
        handlerResponse(body);
    }).catch((error) => {
        DebugMsg('ERROR: ' + error)
        RequestClose();
    }).finally(() => {
        log_Close();
    })
}*/

function serverSend(req, handlerResponse, allowDisconnected=false) {
    if (!RequestOpen(allowDisconnected)) return;
    ws.send(req);

    ws.onmessage = (evt) => {
        const data = JSON.parse(evt.data);
        if (data['status'] == 0) throw new Error(`VProbe: ${data['msg']}`);
        DebugMsg('ok');
        RequestClose();
        handlerResponse(data);
        log_Close();
    }
}


function AddFormAction(formId, handlerResponse)
{
    document.forms[formId].addEventListener('submit', (event) =>
    {
        event.preventDefault();

        //if (!RequestOpen()) return;
        /*fetch(event.target.action, {
            method: 'POST',
            body: new URLSearchParams(new FormData(event.target)) // event.target is the form
        }).then((response) => {
            if (!response.ok) throw new Error(`HTTP ${response.status}`);
            return response.json(); // json(); // or response.text() or whatever the server sends
        }).then((body) => {
            if (body['status'] == 0) throw new Error(`VProbe: ${body['msg']}`);
            DebugMsg('ok');
            RequestClose();
            handlerResponse(body);
        }).catch((error) => {
            DebugMsg('ERROR: ' + error)
            RequestClose();
        }).finally(() => {
            log_Close();
        })*/

        //console.log(new URLSearchParams(new FormData(event.target)));
        ws.send(new URLSearchParams(new FormData(event.target)));

        ws.onmessage = (evt) => {
            const data = JSON.parse(evt.data);
            if (data['status'] == 0) throw new Error(`VProbe: ${data['msg']}`);
            DebugMsg('ok');
            RequestClose();
            handlerResponse(data);
            log_Close();
        }
    });
}



// ============================================================================

// === define websocke callbacks ==============================================

function onWsOpen(evt) {
    DebugMsg('ws connected');
    SetStatus(Status.on);
    connected = true;
}

function onWsClose(evt) {
    DebugMsg('ws disconnected');
    SetStatus(Status.off);

    if (evt.wasClean) {
        //alert(`[close] Connection closed cleanly, code=${evt.code} reason=${evt.reason}`);
    } else {
    // e.g. server process killed or network down
    // event.code is usually 1006 in this case
        alert('[close] Connection died');
    }
    connected = false;
}

function onWsError(evt) {
    DebugMsg('ws error:' + evt.message);
    SetStatus(Status.off);
    connected = false;
}


// === register form submit callbacks =========================================

function InitVprobe()
{
    AddFormAction('statistics', showStatistics);
    AddFormAction('linearity', showLinearity);

    doConnect();
}

// === connection to tester ===================================================

function doConnect()
{
    if (ws) ws.close();
    delete ws;
    ws = new WebSocket('ws://' + window.location.host + '/ws');
    ws.onopen = onWsOpen;
    ws.onclose = onWsClose;
    ws.onerror = onWsError;
}


// === get tester version =====================================================

function doGetVersion()
{
    serverSend('version', (response)=>
    {
        let version  = response['version'];
        DebugMsg('Version: ' + version.slice(0,-2));
    });
}


// === qualification ==========================================================

function doQualification(event)
{
    DummyRun('Qualification', 3000);
}


// === statistics =============================================================

function showStatistics(data)
{
    let ch = data['req']['ch'];
    let v  = data['req']['v'];
    let n  = data['req']['n'];
    let imgfile = data['rsp']['img'];
    log_Open('Statistics');
    log_Add(`CH=${ch} &nbsp; V<sub>pin</sub>=${v}V &nbsp; n=${n}`);
    log_Add('<table><tr><th>pin</th><th>mean</th><th>stdev</th><th>error</th><th>min</th><th>max</th></tr>');
    let pin = 1;
    let vmean_sum = 0;
    let vstdev_sum = 0;
    let verr_sum = 0;
    data['rsp']['stat'].forEach((d)=>
    {
        let vmean  = d['v_mean'];
        let vstdev = d['v_stdev'];
        let verr = d['v_err'];
        let vmin = d['v_min'];
        let vmax = d['v_max'];
        log_Add(`<tr><td>${pin}</td><td>${vmean.toFixed(2)}</td><td>${vstdev.toFixed(2)}</td><td>${verr.toFixed(2)}</td><td>${vmin.toFixed(2)}</td><td>${vmax.toFixed(2)}</td></tr>\n`);
        pin += 1;
        vmean_sum += vmean;
        vstdev_sum += vstdev;
        verr_sum += verr;
    });
    vmean_sum /= 8;
    vstdev_sum /= 8;
    verr_sum /= 8;
    log_Add(`<tr><td>mean</td><td>${vmean_sum.toFixed(2)}</td><td>${vstdev_sum.toFixed(2)}</td><td>${verr_sum.toFixed(2)}</td><td></td><td></td></tr>\n`);
    log_Add(`</table><img id="quali-animation" src="${imgfile}?${+new Date()}" width=800px>`);
}


// === linearity ==============================================================

function showLinearity(data)
{
    let ch = data['req']['ch'];
    let n  = data['req']['n'];
    let imgfile = data['rsp']['img'];
    log_Open('Linearity');
    log_Add(`CH=${ch} &nbsp; n=${n}<br>`);
    log_Add(`<img src="${imgfile}?${+new Date()}" width=800px>`);
}


// === debug ==================================================================

function doProgSelect(n)
{
    serverSend('prog=' + n, (data)=>
    {
        let channel  = data['prog'];
        log_Open('Prog Select');
        log_Add('Prog: ' + (channel==0 ? 'no CPU selected' : 'CPU ' + channel + '<br>'));
        document.getElementById("btnProg0").className = "btn";
        document.getElementById("btnProg1").className = "btn btn-small" + ((n==1)? " btn-green" : "");
        document.getElementById("btnProg2").className = "btn btn-small" + ((n==2)? " btn-green" : "");
    });
}


function doProgRun()
{
    // document.getElementById("btnProgRun").className = "btn btn-red";
    DummyRun('Programming', 1000);
}


function doSetVoltage(set)
{
    var v = 0;
    if (set == 'value') v = parseFloat(document.getElementById("vdipValue").value);
    else v = parseFloat(set);
    serverSend(`setv=${v.toFixed(4)}`, (data)=>
    {
        log_Open('Set V<sub>dip</sub>');
        let voltage  = data['setv'];
        log_Add(`Set V<sub>dip</sub> to ${voltage.toFixed(4)} V<br>`);
    });
}


function doReadRaw(i2caddr, len)
{
    var a = parseInt(i2caddr)
    if (isNaN(a)) return;
    var n = parseInt(len)
    if (a < 0) {
        a = parseInt(document.getElementById("i2caddr").value);
        n = parseInt(document.getElementById("i2csize").value);
    }
    if (isNaN(a) || isNaN(n)) return;

    serverSend(`raw=${a} ${n}`, (data)=>
    {
        log_Open('Read raw');
        let raw_value  = data['raw'];

        let nmax = raw_value.length;
        log_Add('<table><tr><th>Addr</th>');
        for (let i=0; i<nmax; i++) log_Add(`<th>${i}</th>`);
        log_Add(`</tr><tr><td>${a.toString(16).padStart(2,"0")}</td>`);
        for (let i=0; i<nmax; i++) log_Add(`<td>${raw_value[i].toString(16).padStart(2,"0")}</td>`);
        log_Add(`</tr><tr><td colspan="2">dec</td>`);
        for (let i=1; i<nmax-3; i+=2)
            log_Add(`<td colspan="2">${(raw_value[i] + 256*raw_value[i+1]).toString()}</td>`);
        log_Add('</tr></table>');
    });
}


function doReset()
{
    serverSend('reset', (data)=>{});
}


function doGetState()
{
    serverSend('state', (data)=>
    {
        state = data['state'];
        log_Open('I2C status');
        log_Add(`${state}<br>`);
        log_Add(`<span style="width:10em:">NACKF:</span> ${(state & 0x0010)?1:0}<br>`);
        log_Add(`<span style="width:10em:">STOPF:</span> ${(state & 0x0020)?1:0}<br>`);
        log_Add(`<span style="width:10em:">TC:</span> ${(state & 0x0040)?1:0}<br>`);
        log_Add(`<span style="width:10em:">TCR:</span> ${(state & 0x0080)?1:0}<br>`);
        log_Add(`<span style="width:10em:">BERR:</span> ${(state & 0x0100)?1:0}<br>`);
        log_Add(`<span style="width:10em:">ARLO:</span> ${(state & 0x0200)?1:0}<br>`);
        log_Add(`<span style="width:10em:">OVR:</span> ${(state & 0x0400)?1:0}<br>`);
        log_Add(`<span style="width:10em:">BUSY:</span> ${(state & 0x8000)?1:0}<br>`);
    });
}


function doScanI2C()
{
    serverSend('scani2c=0', (data)=>
    {
        log_Open('Scan I2C');
        let list = data['rsp'];
        let n = 0;
        s = "";
        for (const [addr, state] of Object.entries(list)) {
            if (state != 0) s += " " + addr.toString(16).padStart(2,"0");
            n += 1;
        }
        log_Add(`${n} devices found:${s}`)
    });    
}


function doRead(channel)
{
    var n = parseInt(document.getElementById("count").value);
    if (isNaN(n)) return;
    serverSend(`getv=${channel} ${n}`, (data)=>
    {
        log_Open('Read');
        var i=0;
        let voltage = data['getv'];
        voltage.forEach( (x)=>{ log_Add(`${i}: ${x.toFixed(4)} V<br>`); i+=1; } )
    });
}
