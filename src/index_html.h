#pragma once

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="ro">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <title>Dronă biomimetică - Sistem de Control</title>
  <style>
    :root {
      --bg: #121212;
      --card-bg: #1e1e1e;
      --card-bg-alt: #252525;
      --primary: #00adb5;
      --danger: #ff4c4c;
      --success: #4CAF50;
      --text: #ffffff;
      --text-muted: #aaaaaa;
      --border: #333;
      --tab-inactive: #2a2a2a;
    }
    * { box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: var(--bg);
      color: var(--text);
      margin: 0 auto;
      padding: 15px;
      max-width: 480px;        
      text-align: center;
    }
    h1 { color: var(--primary); font-size: 22px; margin: 0 0 4px 0; }
    h2 { margin: 0 0 10px 0; font-size: 16px; color: var(--primary);
         border-bottom: 1px solid var(--border); padding-bottom: 5px; }
    .status { font-size: 13px; font-weight: bold; color: var(--text-muted); margin-bottom: 12px; }

    /* Comutator mod (Experimental <-> Joystick) */
    .mode-switch {
      display: flex; gap: 8px;
      background: var(--card-bg);
      padding: 6px;
      border-radius: 12px;
      margin-bottom: 15px;
    }
    .mode-btn {
      flex: 1; padding: 12px; border: none; border-radius: 8px;
      background: transparent; color: var(--text-muted);
      font-size: 14px; font-weight: bold; cursor: pointer;
      transition: 0.2s;
    }
    .mode-btn.active {
      background: var(--primary); color: white;
      box-shadow: 0 2px 6px rgba(0,173,181,0.4);
    }

    .mode-panel { display: none; }
    .mode-panel.active { display: block; animation: fadeIn 0.25s; }
    @keyframes fadeIn { from { opacity: 0; transform: translateY(4px); } to { opacity: 1; transform: translateY(0); } }

    .card {
      background: var(--card-bg);
      border-radius: 12px;
      padding: 14px;
      margin-bottom: 12px;
      text-align: left;
      box-shadow: 0 2px 4px rgba(0,0,0,0.3);
    }
    .card.primary-border { border-bottom: 3px solid var(--primary); }

    .tabs { display: flex; gap: 8px; margin-bottom: 12px; }
    .tab-btn {
      flex: 1; padding: 10px; background: var(--tab-inactive);
      color: var(--text-muted); border: none; border-radius: 8px;
      font-weight: bold; font-size: 13px; cursor: pointer; transition: 0.2s;
    }
    .tab-btn.active {
      background: var(--primary); color: white;
      box-shadow: 0 2px 6px rgba(0,173,181,0.3);
    }
    .tab-content { display: none; }
    .tab-content.active { display: block; animation: fadeIn 0.25s; }

    .input-row { display: flex; gap: 10px; margin-bottom: 12px; }
    .input-group { flex: 1; display: flex; flex-direction: column; }
    label { font-size: 12px; color: var(--text-muted); margin-bottom: 4px; font-weight: bold; }
    input[type=number] {
      background: #2a2a2a; border: 1px solid #444; color: white;
      padding: 9px; border-radius: 8px; font-size: 15px; text-align: center;
    }
    select {
      background: #2a2a2a; border: 1px solid #444; color: white;
      padding: 9px; border-radius: 8px; font-size: 15px;
      width: 100%; margin-bottom: 12px;
    }

    button.btn-send {
      background: var(--primary); color: white; border: none;
      padding: 11px; font-size: 15px; font-weight: bold;
      border-radius: 8px; cursor: pointer; width: 100%; margin-top: 8px;
    }
    button.btn-send:active { transform: scale(0.97); }
    button.btn-danger {
      background: var(--danger); border: none; color: white;
      padding: 14px; font-size: 17px; font-weight: bold;
      border-radius: 8px; cursor: pointer; width: 100%;
      box-shadow: 0 3px 6px rgba(255,76,76,0.3);
    }
    button.btn-danger:active { transform: scale(0.97); }

    .slider-box { margin-bottom: 12px; }
    .slider-val { float: right; color: var(--primary); font-family: monospace;
                  font-weight: bold; font-size: 14px; }
    input[type=range] {
      -webkit-appearance: none; width: 100%; height: 18px;
      background: #333; border-radius: 9px; outline: none; margin-top: 4px;
    }
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance: none; appearance: none;
      width: 26px; height: 26px; border-radius: 50%;
      background: var(--primary); cursor: pointer; border: 2px solid #fff;
    }

    /* D-pad joystick */
    .dpad {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      grid-template-rows: 1fr 1fr 1fr;
      gap: 10px;
      max-width: 320px;
      margin: 10px auto;
      touch-action: none;      
    }
    .dpad button {
      aspect-ratio: 1 / 1;
      border: none; border-radius: 14px;
      background: var(--card-bg-alt); color: var(--text);
      font-size: 15px; font-weight: bold; cursor: pointer;
      box-shadow: 0 3px 6px rgba(0,0,0,0.4);
      transition: background 0.1s, transform 0.05s;
      user-select: none; -webkit-user-select: none;
    }
    .dpad button.pressed {
      background: var(--primary); color: white;
      transform: scale(0.95);
    }
    .dpad .btn-stop-center {
      background: var(--danger); color: white;
    }
    .dpad .btn-stop-center.pressed { background: #cc0000; }
    .btn-up    { grid-column: 2; grid-row: 1; }
    .btn-left  { grid-column: 1; grid-row: 2; }
    .btn-stop-center { grid-column: 2; grid-row: 2; }
    .btn-right { grid-column: 3; grid-row: 2; }
    .btn-down  { grid-column: 2; grid-row: 3; }

    .preset-info {
      font-size: 13px; color: var(--text-muted);
      text-align: center; margin: 8px 0 0 0;
    }
    .preset-info b { color: var(--primary); }
    .small-note { font-size: 11px; color: var(--text-muted); margin: 0; }
    ul.preset-list { font-size: 12px; color: var(--text-muted);
                     padding-left: 20px; margin: 4px 0; }
    ul.preset-list b { color: var(--primary); }
  </style>
</head>
<body>

  <h1>Sistemul de control al dronei biomimetice</h1>
  <div class="status" id="statusText">Se conectează...</div>

  <!-- Comutator de nivel superior intre cele 2 moduri -->
  <div class="mode-switch">
    <button class="mode-btn active" id="btnModeExp" onclick="switchMode('exp')">Mod Experimental</button>
    <button class="mode-btn"        id="btnModeJoy" onclick="switchMode('joy')">Joystick</button>
  </div>


  <div id="panelExp" class="mode-panel active">

    <div class="card primary-border">
      <button class="btn-danger" onclick="sendStop()">Oprire servomotoare (0 Hz)</button>

      <div class="slider-box" style="margin-top: 14px;">
        <label>Frecvența bătăilor de aripi</label>
        <span class="slider-val" id="valFreq">0.0 Hz</span>
        <input type="range" id="freqSlider" min="0" max="4" step="0.1" value="0" oninput="sendFreq()">
      </div>
    </div>

    <div class="card">
      <h2>Cinematică</h2>
      <label>Forma Undei</label>
      <select id="waveType" onchange="sendKine()">
        <option value="0">Sinusoidală</option>
        <option value="1">Triunghiular</option>
      </select>

      <div class="slider-box">
        <label>Durată Downstroke </label>
        <span class="slider-val" id="valRatio">50%</span>
        <input type="range" id="ratioSlider" min="30" max="80" step="5" value="50" oninput="sendKine()">
        <p class="small-note">Peste 50% = coborâre mai lentă decât urcarea.</p>
      </div>
    </div>

    <div class="card">
      <div class="tabs">
        <button class="tab-btn active" id="btnTab1" onclick="switchTab(1)">Zbor Drept</button>
        <button class="tab-btn"        id="btnTab2" onclick="switchTab(2)">Viraje</button>
      </div>

      <div id="tab1" class="tab-content active">
        <h2>Setări Simetrice</h2>
        <div class="slider-box">
          <label>Offset</label>
          <span class="slider-val" id="valCenterG">0°</span>
          <input type="range" id="centerG" min="-45" max="45" value="0"
                 oninput="updateDisplays(); sendFlightData();">
        </div>
        <div class="input-row">
          <div class="input-group">
            <label>Unghi Minim (°)</label>
            <input type="number" id="minG" value="-30">
          </div>
          <div class="input-group">
            <label>Unghi Maxim (°)</label>
            <input type="number" id="maxG" value="30">
          </div>
        </div>
        <button class="btn-send" onclick="sendFlightData()">Trimite parametrii de zbor</button>
      </div>

      <div id="tab2" class="tab-content">
        <h2>Setări Aripa Stângă</h2>
        <div class="slider-box">
          <label>Offset Centru</label>
          <span class="slider-val" id="valCenterL">0°</span>
          <input type="range" id="centerL" min="-45" max="45" value="0"
                 oninput="updateDisplays(); sendFlightData();">
        </div>
        <div class="input-row">
          <div class="input-group">
            <label>Minim  (°)</label>
            <input type="number" id="minL" value="-30">
          </div>
          <div class="input-group">
            <label>Maxim (°)</label>
            <input type="number" id="maxL" value="30">
          </div>
        </div>

        <h2 style="margin-top: 18px;">Setări Aripa Dreaptă</h2>
        <div class="slider-box">
          <label>Offset Centru </label>
          <span class="slider-val" id="valCenterR">0°</span>
          <input type="range" id="centerR" min="-45" max="45" value="0"
                 oninput="updateDisplays(); sendFlightData();">
        </div>
        <div class="input-row">
          <div class="input-group">
            <label>Minim  (°)</label>
            <input type="number" id="minR" value="-30">
          </div>
          <div class="input-group">
            <label>Maxim  (°)</label>
            <input type="number" id="maxR" value="30">
          </div>
        </div>
        <button class="btn-send" onclick="sendFlightData()">Trimite Asimetria</button>
      </div>
    </div>

  </div>

  
  <div id="panelJoy" class="mode-panel">

    <div class="card">
      <h2>Control Joystick</h2>
      

      <div class="dpad" id="dpad">
        <button class="btn-up"          data-action="up">Sus</button>
        <button class="btn-left"        data-action="left">Stânga</button>
        <button class="btn-stop-center" data-action="stop">STOP</button>
        <button class="btn-right"       data-action="right">Dreapta</button>
        <button class="btn-down"        data-action="down">Jos</button>
      </div>

      <p class="preset-info">Comanda curentă: <b id="activePresetName">Planare</b></p>
    </div>

    <div class="card">
      <div class="slider-box">
        <label>Scalare Frecvență</label>
        <span class="slider-val" id="valThrottle">100%</span>
        <input type="range" id="throttle" min="0" max="100" step="1" value="100"
               oninput="onThrottleChange()">
        <p class="small-note">
          Scalează frecvența profilului activ
        </p>
      </div>
    </div>

    <div class="card">
      <h2>Profile de zbor predefinite</h2>
      <p class="small-note">Valorile au fost determinate experimental.</p>
      <ul class="preset-list">
        <li><b>Planare:</b> 3.0 Hz, simetric, -15° → +45°</li>
        <li><b>Sus:</b> 3.5 Hz, simetric, -25° → +55°</li>
        <li><b>Jos:</b> 2.0 Hz, simetric, 0° → +30°</li>
        <li><b>Stânga:</b> 3.5 Hz, Stânga 0°→+20°, Dreapta -15°→+45°</li>
        <li><b>Dreapta:</b> 3.5 Hz, Stânga -15°→+45°, Dreapta 0°→+20°</li>
      </ul>
      <p class="small-note" style="margin-top: 8px;">
        Forma undei și durata pentru downstroke preiau din ultimele
        setări trimise din Modul Experimental.
      </p>
    </div>

  </div>

  <script>
   
    var gateway = `ws://${window.location.hostname}:81/`;
    var websocket;
    var currentMode = 'exp';      // 'exp' sau 'joy'

    window.addEventListener('load', initWebSocket);

    function initWebSocket() {
      websocket = new WebSocket(gateway);
      websocket.onopen = function() {
        document.getElementById('statusText').innerText = "Conectat";
        document.getElementById('statusText').style.color = "#4CAF50";
        // Sincronizeaza firmware-ul cu modul afisat in browser
        sendModeFlag();
      };
      websocket.onclose = function() {
        document.getElementById('statusText').innerText = "Deconectat";
        document.getElementById('statusText').style.color = "#ff4c4c";
        setTimeout(initWebSocket, 2000);
      };
    }

    function sendModeFlag() {
      if (!websocket || websocket.readyState !== 1) return;
      websocket.send("M:" + (currentMode === 'exp' ? "0" : "1"));
    }

   
    function switchMode(mode) {
      if (mode === currentMode) return;
      currentMode = mode;

      // trimite comanda catre firmware pentru a comuta modul
      sendModeFlag();

      // resetare interna pentru modul curent
      releaseAllDpad();
      lastSentFreq = null;
      lastSentParams = null;

      // comuta vizibilitatea panourilor 
      document.getElementById('btnModeExp').classList.toggle('active', mode === 'exp');
      document.getElementById('btnModeJoy').classList.toggle('active', mode === 'joy');
      document.getElementById('panelExp').classList.toggle('active', mode === 'exp');
      document.getElementById('panelJoy').classList.toggle('active', mode === 'joy');

      // reseteaza indicatoarele vizuale
      if (mode === 'exp') {
        document.getElementById('freqSlider').value = 0;
        document.getElementById('valFreq').innerText = "0.0 Hz";
      } else {
        activePreset = 'hover';
        document.getElementById('activePresetName').innerText = "Hover (inactiv)";
      }
    }

    var currentTab = 1;

    function switchTab(tabId) {
      currentTab = tabId;
      document.getElementById('btnTab1').classList.toggle('active', tabId === 1);
      document.getElementById('btnTab2').classList.toggle('active', tabId === 2);
      document.getElementById('tab1').classList.toggle('active', tabId === 1);
      document.getElementById('tab2').classList.toggle('active', tabId === 2);
      sendFlightData();
    }

    function updateDisplays() {
      document.getElementById('valCenterG').innerText = document.getElementById('centerG').value + "°";
      document.getElementById('valCenterL').innerText = document.getElementById('centerL').value + "°";
      document.getElementById('valCenterR').innerText = document.getElementById('centerR').value + "°";
    }

    function sendStop() {
      document.getElementById('freqSlider').value = 0;
      document.getElementById('valFreq').innerText = "0.0 Hz";
      if (websocket.readyState == 1) websocket.send("F:0.0");
    }

    function sendFreq() {
      let val = document.getElementById('freqSlider').value;
      document.getElementById('valFreq').innerText = val + " Hz";
      if (websocket.readyState == 1) websocket.send("F:" + val);
    }

    function sendKine() {
      let wType = document.getElementById('waveType').value;
      let ratio = document.getElementById('ratioSlider').value;
      document.getElementById('valRatio').innerText = ratio + "%";
      if (websocket.readyState == 1) websocket.send("K:" + wType + "," + ratio);
    }

    function sendFlightData() {
      if (websocket.readyState !== 1) return;
      let cL, cR, minL, maxL, minR, maxR;
      if (currentTab === 1) {
        let centerG = document.getElementById('centerG').value;
        cL = centerG; cR = centerG;
        let minGlobal = document.getElementById('minG').value;
        let maxGlobal = document.getElementById('maxG').value;
        minL = minGlobal; maxL = maxGlobal; minR = minGlobal; maxR = maxGlobal;
      } else {
        cL = document.getElementById('centerL').value;
        cR = document.getElementById('centerR').value;
        minL = document.getElementById('minL').value; maxL = document.getElementById('maxL').value;
        minR = document.getElementById('minR').value; maxR = document.getElementById('maxR').value;
      }
      websocket.send("C:" + cL + "," + cR);
      websocket.send("A:" + minL + "," + maxL + "," + minR + "," + maxR);
    }

     //mod joystick
     //profile de zbor predefinite 
    const PRESETS = {
      hover: { freq: 3.0, cL: 0, cR: 0, minL: -15, maxL: 45, minR: -15, maxR: 45 },
      up:    { freq: 3.5, cL: 0, cR: 0, minL: -25, maxL: 55, minR: -25, maxR: 55 },
      down:  { freq: 2.0, cL: 0, cR: 0, minL:   0, maxL: 30, minR:   0, maxR: 30 },
      left:  { freq: 3.5, cL: 0, cR: 0, minL:   0, maxL: 20, minR: -15, maxR: 45 },
      right: { freq: 3.5, cL: 0, cR: 0, minL: -15, maxL: 45, minR:   0, maxR: 20 }
    };
    
    const PRESET_NAMES_RO = {
      hover: 'Hover', up: 'Sus', down: 'Jos', left: 'Stânga', right: 'Dreapta'
    };

    let activePreset = 'hover';
    let throttlePercent = 100;
    let lastSentFreq = null;
    let lastSentParams = null;
    let throttleDebounce = null;
    let pressedButton = null;

    function applyPreset(name) {
      if (currentMode !== 'joy') return;
      if (websocket.readyState !== 1) return;

      activePreset = name;
      document.getElementById('activePresetName').innerText = PRESET_NAMES_RO[name];

      const p = PRESETS[name];
      // trimite C: si A: doar daca preset-ul s-a schimbat
      if (name !== lastSentParams) {
        websocket.send("C:" + p.cL + "," + p.cR);
        websocket.send("A:" + p.minL + "," + p.maxL + "," + p.minR + "," + p.maxR);
        lastSentParams = name;
        lastSentFreq = null;  // forteaza retrimiterea F: pentru noul preset
      }
      sendScaledFreq();
    }

    function sendScaledFreq() {
      if (currentMode !== 'joy') return;
      if (websocket.readyState !== 1) return;
      const p = PRESETS[activePreset];
      const f = ((p.freq * throttlePercent) / 100).toFixed(2);
      if (f === lastSentFreq) return;
      lastSentFreq = f;
      websocket.send("F:" + f);
    }

    function onThrottleChange() {
      throttlePercent = parseInt(document.getElementById('throttle').value, 10);
      document.getElementById('valThrottle').innerText = throttlePercent + "%";
      clearTimeout(throttleDebounce);
      throttleDebounce = setTimeout(sendScaledFreq, 50);
    }

    function joystickStop() {
      if (websocket.readyState !== 1) return;
      activePreset = 'hover';
      lastSentFreq = null;
      lastSentParams = null;
      websocket.send("F:0.0");
      document.getElementById('activePresetName').innerText = "STOP";
    }

    function releaseAllDpad() {
      if (pressedButton) {
        pressedButton.classList.remove('pressed');
        pressedButton = null;
      }
    }

    // pointer events = mouse + touchscreen
    document.addEventListener('DOMContentLoaded', function() {
      const dpad = document.getElementById('dpad');
      const buttons = dpad.querySelectorAll('button');

      buttons.forEach(btn => {
        const action = btn.getAttribute('data-action');

        btn.addEventListener('pointerdown', (e) => {
          e.preventDefault();
          try { btn.setPointerCapture(e.pointerId); } catch (_) {}
          releaseAllDpad();
          pressedButton = btn;
          btn.classList.add('pressed');

          if (action === 'stop') {
            joystickStop();
          } else {
            applyPreset(action);
          }
        });

        const release = (e) => {
          if (pressedButton !== btn) return;
          btn.classList.remove('pressed');
          pressedButton = null;
          // STOP nu revine la modul de planare; ramane oprit pana la apasarea unui buton nou
          if (action === 'stop') return;
          applyPreset('hover');
        };

        btn.addEventListener('pointerup', release);
        btn.addEventListener('pointercancel', release);
      });
    });
  </script>
</body>
</html>
)rawliteral";
