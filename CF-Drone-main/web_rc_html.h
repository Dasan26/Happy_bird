// 网页遥控器客户端界面

#if WEB_RC_ENABLED

const char webRCIndexHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=no">
<title>CF Drone - Control web</title>
<style>
/*======== 响应式变量 ========*/
:root{
  --js-size:clamp(150px,min(50vw,62vh),420px);
  --knob-size:calc(var(--js-size)*0.25);
  --pad:clamp(6px,1.5vmin,15px);
  --gap:clamp(6px,1.5vmin,15px);
}
/*======== 通用样式 ========*/
*{margin:0;padding:0;box-sizing:border-box;-webkit-tap-highlight-color:transparent;touch-action:none;user-select:none}
body{font-family:'Roboto Mono',Arial,"Microsoft YaHei",sans-serif;background:#3c3c3c;color:#fff;overflow:hidden;height:100vh;height:100dvh;width:100vw}
.container{width:100%;height:100%;display:flex;flex-direction:column;padding:var(--pad);gap:var(--gap);max-width:1200px;margin:0 auto;overflow:hidden}

/*======== 顶部状态栏 ========*/
.header {
  text-align: center;
  padding: 6px 10px;
  background: rgba(30,30,30,.9);
  border-radius: 12px;
  border: 2px solid rgba(150,150,150,.3);
  box-shadow: 0 4px 16px rgba(0,0,0,.4);
  backdrop-filter: blur(10px)
}

.header h1 {
  font-size: 1.1rem;
  color: #ffffff;
  margin-bottom: 4px;
}

.status-bar {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: clamp(4px,1.2vmin,10px);
  flex-wrap: nowrap;
  overflow: hidden;
  margin-top: 4px;
}

.status-item {
  display: flex;
  align-items: center;
  gap: 4px;
  padding: 2px 5px;
  background: rgba(0,0,0,.3);
  border-radius: 6px;
  border: 1px solid rgba(255,255,255,.1);
  font-size: clamp(0.58rem,1.8vmin,0.7rem);
  white-space: nowrap;
  flex-shrink: 1;
  min-width: 0;
}

.status-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  display: inline-block;
}

.status-dot.connected { background: #0f8; box-shadow: 0 0 8px #0f8; }
.status-dot.disconnected { background: #f33; box-shadow: 0 0 8px #f33; }
.status-dot.warning { background: #ff9; box-shadow: 0 0 8px #ff9; }

/*======== 内容区 ========*/
.content{display:flex;flex:1;gap:var(--gap);overflow:hidden;min-height:0}
.joystick-container{flex:1;display:flex;flex-direction:column;justify-content:center;align-items:center;background:rgba(0,0,0,.4);border-radius:20px;padding:clamp(8px,1.5vmin,15px);border:2px solid rgba(255,255,255,0.15);box-shadow:inset 0 0 30px rgba(0,0,0,.5)}
.joystick-title{font-size:clamp(0.75rem,2.2vmin,1.1rem);color:#cccccc}
.joystick-wrapper{width:100%;height:var(--js-size);display:flex;justify-content:center;align-items:center;position:relative;margin-top:clamp(4px,2vh,20px);}
.joystick{width:var(--js-size);height:var(--js-size);background:radial-gradient(circle at 30% 30%,rgba(255,255,255,.1),rgba(0,0,0,.3));border-radius:50%;position:relative;border:2px solid rgba(255,255,255,0.2);box-shadow:inset 0 0 20px rgba(0,0,0,.5),0 8px 25px rgba(0,0,0,.5);overflow:hidden}
.joystick::before{content:'';position:absolute;top:50%;left:50%;width:2px;height:100%;background:linear-gradient(to bottom,transparent,rgba(255,255,255,.15),transparent);transform:translate(-50%,-50%)}
.joystick::after{content:'';position:absolute;top:50%;left:50%;width:100%;height:2px;background:linear-gradient(to right,transparent,rgba(255,255,255,.15),transparent);transform:translate(-50%,-50%)}
.joystick-knob{width:var(--knob-size);height:var(--knob-size);background:radial-gradient(circle at 30% 30%,#fff,#cccccc);border-radius:50%;position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);border:2px solid rgba(255,255,255,.7);box-shadow:0 4px 15px rgba(0,0,0,.5),inset 0 0 10px rgba(255,255,255,.5);cursor:move;transition:transform .1s ease-out;z-index:10}

/*======== 按钮区 ========*/
.buttons-container{flex:.55;display:flex;flex-direction:column;gap:10px;padding:12px;background:rgba(0,0,0,.4);border-radius:20px;border:2px solid rgba(150,150,150,.3);box-shadow:inset 0 0 20px rgba(0,0,0,.5)}
.buttons-grid{display:grid;grid-template-columns:repeat(6,1fr);grid-template-rows:repeat(2,1fr);gap:8px;flex:1}
.buttons-grid>button{grid-column:span 2}
.buttons-grid>button:nth-child(4),.buttons-grid>button:nth-child(5),.buttons-grid>button:nth-child(6){grid-column:span 2}
.button{background:linear-gradient(145deg,#484848,#383838);border:none;border-radius:10px;color:#fff;font-size:.85rem;font-weight:bold;display:flex;flex-direction:column;justify-content:center;align-items:center;text-align:center;padding:10px 5px;cursor:pointer;transition:all .15s cubic-bezier(.4,0,.2,1);box-shadow:0 3px 10px rgba(0,0,0,.3),inset 0 1px 0 rgba(255,255,255,.1);position:relative}
.button:hover{background:linear-gradient(145deg,#565656,#464646);transform:translateY(-1px)}
.button.active{background:linear-gradient(145deg,#1a73e8,#0d47a1);box-shadow:0 0 15px rgba(26,115,232,.6),inset 0 1px 0 rgba(255,255,255,.2);transform:scale(.95)}
.button:active{transform:scale(.92)}
.button-icon{font-size:1.1rem;margin-bottom:4px}

/*======== 调试控制台 ========*/
.console-panel{background:rgba(10,10,10,.95);border-radius:12px;border:1px solid rgba(100,100,100,.4);padding:10px;flex-shrink:0;max-height:min(300px,45vh);display:flex;flex-direction:column;gap:6px}
.console-output{flex:1;overflow-y:auto;font-family:'Courier New',monospace;font-size:0.7rem;color:#00ff88;min-height:80px;max-height:130px;word-break:break-all;-webkit-overflow-scrolling:touch}
.console-output div{padding:1px 0;border-bottom:1px solid rgba(255,255,255,.03)}
.chart-view{display:flex;flex-direction:column;gap:3px;min-height:0;touch-action:auto}
.chart-title{font-size:.65rem;color:#ddd;display:flex;justify-content:space-between;gap:8px}
.chart-view canvas{display:block;width:100%;height:78px;background:rgba(255,255,255,.025);border-radius:5px}
.chart-summary{font: .62rem 'Courier New',monospace;color:#9cc;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}

/*======== 动画 ========*/
@keyframes pulse{0%{box-shadow:0 0 0 0 rgba(67,97,238,.7)}70%{box-shadow:0 0 0 12px rgba(67,97,238,0)}100%{box-shadow:0 0 0 0 rgba(67,97,238,0)}}
.joystick.active{animation:pulse 1.5s infinite}
@keyframes fadeIn{from{opacity:0;transform:translateY(20px)}to{opacity:1;transform:translateY(0)}}
.container>*{animation:fadeIn .5s ease-out}

/*======== 竖屏自适应 ========*/
@media (orientation:portrait){
  :root{--js-size:clamp(120px,40vw,240px);--knob-size:calc(var(--js-size)*0.25)}
  .content{
    display:grid;
    grid-template-columns:repeat(2,minmax(0,1fr));
    grid-template-rows:auto auto;
    height:auto;
    flex:none;
    overflow:visible;
  }
  .buttons-container{grid-column:1/3;grid-row:1}
  .content>.joystick-container:first-child{grid-column:1;grid-row:2;min-width:0;overflow:hidden}
  .content>.joystick-container:last-child{grid-column:2;grid-row:2;min-width:0;overflow:hidden}
  .header h1{font-size:clamp(0.85rem,3.5vw,1.1rem)}
  .status-bar{gap:5px;flex-wrap:wrap;justify-content:center}
  .status-item{font-size:clamp(0.6rem,2.5vw,0.7rem);padding:2px 5px}
}

/*======== 小屏横屏自适应（高度≤420px）========*/
@media (max-height:420px) and (orientation:landscape){
  :root{--js-size:clamp(120px,min(42vw,44vh),240px);--knob-size:calc(var(--js-size)*0.25)}
  .joystick-title{font-size:0.72rem}
  .header h1{font-size:0.82rem}
  .header{padding:3px 8px}
  .header h1{margin-bottom:2px}
  .status-bar{margin-top:2px;gap:4px}
  .status-item{font-size:0.58rem;padding:2px 4px}
  .button{font-size:0.68rem;padding:5px 3px}
  .button-icon{font-size:0.88rem;margin-bottom:2px}
}
/*======== 版权页脚 ========*/
.footer{text-align:center;font-size:0.5rem;color:rgba(255,255,255,.25);padding:0;flex-shrink:0;line-height:0.8;}
.footer a{color:rgba(255,255,255,.3);text-decoration:none}
.footer a:hover{color:rgba(255,255,255,.55)}
</style>
</head>
<body>
<div class="container">
  <!-- 顶部状态栏 -->
  <div class="header">
    <h1>CF Drone - Control web</h1>
    <div class="status-bar">
      <div class="status-item"><span class="status-dot" id="status-dot"></span><span id="connection-text">Conectando...</span></div>
      <div class="status-item" id="armed-status-item" style="background:rgba(255,51,51,0.15)"><span id="armed-status" style="color:#ff6666">Bloqueado</span></div>
      <div class="status-item"><span>Modo</span><span id="flight-mode">Estabilizado</span></div>
      <div class="status-item"><span>Batería</span><span id="battery">-</span></div>
      <div class="status-item"><span>Latencia</span><span id="latency">-</span></div>
      <div class="status-item"><span>Pérdida</span><span id="packet-loss">0%</span></div>
      <div class="status-item"><span>Acelerador:</span><span id="left-y">0</span><span>%</span></div>
      <div class="status-item"><span>Guiñada:</span><span id="left-x">0</span></div>
      <div class="status-item"><span>Alabeo:</span><span id="right-x">0</span></div>
      <div class="status-item"><span>Cabeceo:</span><span id="right-y">0</span></div>
    </div>
  </div>

  <div class="content">
    <!-- 左摇杆 -->
    <div class="joystick-container">
      <div class="joystick-title">Palanca izquierda (acelerador/guiñada)</div>
      <div class="joystick-wrapper">
        <div class="joystick" id="joystick-left"><div class="joystick-knob" id="knob-left"></div></div>
      </div>
    </div>

    <!-- 按钮区 -->
    <div class="buttons-container">
      <div class="buttons-grid" id="buttons-container"></div>
    </div>

    <!-- 右摇杆 -->
    <div class="joystick-container">
      <div class="joystick-title">Palanca derecha (cabeceo/alabeo)</div>
      <div class="joystick-wrapper">
        <div class="joystick" id="joystick-right"><div class="joystick-knob" id="knob-right"></div></div>
      </div>
    </div>
  </div>

  <!-- 调试控制台面板（默认隐藏，点击调试按钮打开） -->
  <div id="console-panel" class="console-panel" style="display:none">
    <div id="console-output" class="console-output"></div>
    <div id="chart-view" class="chart-view" style="display:none">
      <div class="chart-title"><span>Altura (m): medida / objetivo</span><span id="chart-mode">Esperando DPS310</span></div>
      <canvas id="altitude-chart"></canvas>
      <div class="chart-title"><span>Velocidad vertical (m/s): medida / objetivo</span><span>Últimos 30 s</span></div>
      <canvas id="vertical-speed-chart"></canvas>
      <div id="chart-summary" class="chart-summary">Sin telemetría</div>
    </div>
    <div id="console-controls" style="display:flex;gap:6px;touch-action:auto">
      <input id="console-input" placeholder="Escribe un comando (ps/imu/rc/arm/disarm/help)..."
        style="flex:1;background:rgba(0,0,0,.6);border:1px solid rgba(100,100,100,.5);border-radius:6px;color:#0f8;padding:4px 8px;font-size:0.7rem;font-family:'Courier New',monospace;touch-action:auto">
      <button onclick="sendConsoleCmd()" style="background:#1a73e8;border:none;border-radius:6px;color:#fff;padding:4px 10px;font-size:0.7rem;cursor:pointer;touch-action:auto">Enviar</button>
    </div>
  </div>
  <!-- 版权页脚 -->
  <div class="footer"><a href="https://oshwhub.com/songge8/project_qqqyfdkm" target="_blank">Proyecto CF Drone de código abierto</a></div>
</div>

<script>
/*======================== 全局变量 ========================*/
let lastAnimationTime = 0;
let connectionOk = false;
let packetStats = { sent: 0, lost: 0 };
let latencyHistory = new Array(10).fill(0);
let latencyIndex = 0;
const SEND_INTERVAL = 50;  // ~20Hz 摇杆检测频率
const FORCE_SEND_INTERVAL = 200; // 静止时强制重发间隔（ms），保持飞控数据新鲜

const touches = new Map();
let leftStick  = {x:0, y:0, rawX:0, rawY:-100};
let rightStick = {x:0, y:0, rawX:0, rawY:0};

let lastSentValues = { throttle:0, roll:0, pitch:0, yaw:0 };
let lastForceSentTime = 0; // 上次强制重发的时间戳（performance.now()）
let currentValues  = { throttle:0, roll:0, pitch:0, yaw:0 };
const MIN_CHANGE_THRESHOLD = 0.5;

// 固定参数常量（替代前端参数调节面板，与后端 CONFIG_ 对应）
const DEADZONE = 3;   // 死区（已移至后端 stickDeadzone 统一处理，前端不再使用）
const EXPO    = 40;   // 指数曲线 40%
let consecutiveFails = 0; // 连续失败计数，>=3 才判定断连
let currentFlightMode = 2; // 当前飞行模式编号（与后端同步：2=自稳）

let buttonStates     = new Array(16).fill(false);
let lastButtonStates = new Array(16).fill(false);

let consolePollingTimer = null;
let consoleLastTotal    = 0;   // 增量拉取游标：已展示到第 N 行
let consoleFetchInFlight = false; // 防并发：上次 fetch 未返回时跳过本次
let consolePanelOpen = false;
let graphPanelOpen = false;
let graphPollTimer = null;
let graphSamples = [];
let graphFetchBusy = false;
const CONSOLE_BASE_POLL_MS = 500;
const CONSOLE_CATCHUP_POLL_MS = 80;
const CONSOLE_PAGE_LIMIT = 20;

/*======================== 按钮配置（2×3 六宫格）========================*/
const buttonConfigs = [
  {icon:"🔓",label:"Desbloquear", color:"#00ff88",desc:"Habilitar motores"},
  {icon:"🔒",label:"Bloquear", color:"#ff3333",desc:"Bloquear motores"},
  {icon:"🛑",label:"Parada", color:"#ff0055",desc:"Parada de emergencia"},
  {icon:"🔄",label:"Modo: Estabilizado/Altura", color:"#00cfff",desc:"Alterna sin pasar por Acrobático. En Altura: centro mantiene, arriba asciende y abajo desciende."},
  {icon:"🖥",label:"Consola", color:"#4a9eff",desc:"Consola de depuración"},
  {icon:"📈",label:"Gráficas", color:"#b58cff",desc:"Grafica altura y velocidad vertical para validar el control"}
];

/*======================== 初始化 ========================*/
function init() {
  initButtons();
  initNetwork();
  initPointerEvents();
  requestAnimationFrame(animationLoop);
  requestAnimationFrame(initKnobPositions);
}

function initKnobPositions() {
  // 根据初始 rawY 将旋鈕定位到正确位置（左摇杆油门在底部）
  const joystick = document.getElementById('joystick-left');
  const knob     = document.getElementById('knob-left');
  const rect     = joystick.getBoundingClientRect();
  if (rect.width === 0) { requestAnimationFrame(initKnobPositions); return; } // 布局未就绪则重试
  const radius = rect.width / 2 - 10;
  const dy = -leftStick.rawY / 100 * radius; // rawY=-100 → dy=radius（底部）
  knob.style.transform = `translate(calc(-50% + 0px), calc(-50% + ${dy}px))`;
}

function initButtons() {
  const container = document.getElementById('buttons-container');
  container.innerHTML = '';
  buttonConfigs.forEach((cfg, idx) => {
    const btn = document.createElement('button');
    btn.className = 'button';
    btn.id = `btn-${idx}`;
    btn.title = cfg.desc;
    btn.innerHTML = `<div class="button-icon">${cfg.icon}</div><div>${cfg.label}</div>`;
    btn.style.border = `2px solid ${cfg.color}55`;
    btn.addEventListener('pointerdown', e => {
      e.preventDefault();
      handleButton(idx);
      if (navigator.vibrate) navigator.vibrate(20);
    });
    container.appendChild(btn);
  });
}

function initNetwork() {
  updateConnectionStatus(true);
  setInterval(updateNetworkStatus, 2000);
}

function initPointerEvents() {
  [{ id:'joystick-left', side:'left' }, { id:'joystick-right', side:'right' }].forEach(js => {
    const el = document.getElementById(js.id);
    el.addEventListener('pointerdown', e => { e.preventDefault(); handlePointerStart(e, js.side); el.setPointerCapture(e.pointerId); });
    el.addEventListener('pointermove', e => { e.preventDefault(); handlePointerMove(e, js.side); });
    el.addEventListener('pointerup',   e => { e.preventDefault(); handlePointerEnd(e, js.side); });
    el.addEventListener('pointercancel', e => { e.preventDefault(); handlePointerEnd(e, js.side); });
    el.addEventListener('touchstart', e => e.preventDefault());
    el.addEventListener('touchmove',  e => e.preventDefault());
  });
}

/*======================== 动画循环 ========================*/
function animationLoop(timestamp) {
  if (timestamp - lastAnimationTime >= SEND_INTERVAL) {
    processJoystickInput();
    checkAndSendChanges();
    updateDisplayAll();
    lastAnimationTime = timestamp;
  }
  requestAnimationFrame(animationLoop);
}

/*======================== 摇杆曲线处理 ========================*/
// 仅做 expo 曲线，死区由后端 stickDeadzone 统一处理（对齐 SBUS 模式）
function applyCurve(value) {
  const absVal = Math.abs(value);
  const expoFactor = EXPO / 100;
  let curved = absVal * (1 - expoFactor) + Math.pow(absVal, 3) * expoFactor;
  return curved * (value >= 0 ? 1 : -1);
}

/*======================== 摇杆数据处理 ========================*/
function processJoystickInput() {
  // 发送原始值，后端统一完成映射（对齐SBUS/MAVLink模式，避免双重映射）
  // 油门：rawY∈[-100,+100]，后端 processThrottle: (raw+100)/(2*RAW_MAX)*100 → 0~100%
  // 姿态轴：归一化到[-1,1]做指数曲线后还原×100，后端除以RAW_MAX得[-1,1]
  currentValues.throttle = leftStick.rawY;
  currentValues.yaw   = applyCurve(leftStick.rawX  / 100) * 100;  // 右推=顺时针，与MAVLink一致
  currentValues.pitch = applyCurve(rightStick.rawY / 100) * 100;  // 上推=前进，与MAVLink一致
  currentValues.roll  = applyCurve(rightStick.rawX / 100) * 100;  // 右推=右滚，与MAVLink一致
  leftStick.x  = currentValues.yaw;
  leftStick.y  = leftStick.rawY;
  rightStick.x = currentValues.roll;
  rightStick.y = currentValues.pitch;
}

function hasSignificantChange(nv) {
  return Math.abs(nv.throttle - lastSentValues.throttle) > MIN_CHANGE_THRESHOLD ||
         Math.abs(nv.roll     - lastSentValues.roll)     > MIN_CHANGE_THRESHOLD ||
         Math.abs(nv.pitch    - lastSentValues.pitch)    > MIN_CHANGE_THRESHOLD ||
         Math.abs(nv.yaw      - lastSentValues.yaw)      > MIN_CHANGE_THRESHOLD;
}

function checkAndSendChanges() {
  const now = performance.now();
  // 有变化立即发；或超过强制重发间隔时也发一次（保持飞控侧数据新鲜，避免超时断连）
  if (hasSignificantChange(currentValues) || (now - lastForceSentTime >= FORCE_SEND_INTERVAL)) {
    sendJoystickData();
    lastForceSentTime = now;
  }
  for (let i = 0; i < buttonStates.length; i++) {
    if (buttonStates[i] !== lastButtonStates[i]) {
      sendButtonData(i, buttonStates[i]);
      lastButtonStates[i] = buttonStates[i];
    }
  }
}

/*======================== 数据发送函数 ========================*/
function sendJoystickData() {
  sendToESP('/web_rc', { t:1, th:Math.round(currentValues.throttle), r:Math.round(currentValues.roll),
    p:Math.round(currentValues.pitch), y:Math.round(currentValues.yaw), ts:performance.now() });
  lastSentValues = {...currentValues};
  packetStats.sent++;
}

function sendButtonData(buttonIndex, state) {
  sendToESP('/web_rc', { t:2, b:buttonIndex, s:state ? 1 : 0, ts:performance.now() });
}

/*======================== 显示更新 ========================*/
function updateDisplayAll() {
  document.getElementById('left-y').textContent  = Math.round((currentValues.throttle + 100) / 2);
  document.getElementById('left-x').textContent  = Math.round(leftStick.x);
  document.getElementById('right-x').textContent = Math.round(rightStick.x);
  document.getElementById('right-y').textContent = Math.round(rightStick.y);
}

/*======================== Pointer Events 处理 ========================*/
function handlePointerStart(e, side) {
  touches.set(e.pointerId, side);
  document.getElementById(`joystick-${side}`).classList.add('active');
  updateJoystickPosition(side, e.clientX, e.clientY);
}

function handlePointerMove(e, side) {
  if (touches.get(e.pointerId) === side) updateJoystickPosition(side, e.clientX, e.clientY);
}

function handlePointerEnd(e, side) {
  if (touches.get(e.pointerId) !== side) return;
  touches.delete(e.pointerId);
  const knob     = document.getElementById(`knob-${side}`);
  const joystick = document.getElementById(`joystick-${side}`);
  joystick.classList.remove('active');
  // 计算归位目标：左摇杆Y轴非ALTHOLD时归底，其他归中
  const targetRawY = (side === 'left' && currentFlightMode !== 3) ? -100 : 0;
  const radius = joystick.getBoundingClientRect().width / 2 - 10;
  const targetDy = -targetRawY / 100 * radius;
  knob.style.transition = 'transform 0.2s ease-out';
  knob.style.transform  = `translate(calc(-50% + 0px), calc(-50% + ${targetDy}px))`;
  setTimeout(() => { knob.style.transition = ''; }, 200);
  if (side === 'left')  leftStick  = {x:0, y:0, rawX:0, rawY: targetRawY};
  else                  rightStick = {x:0, y:0, rawX:0, rawY:0};
  processJoystickInput();
  sendJoystickData();
}

function updateJoystickPosition(side, clientX, clientY) {
  const joystick = document.getElementById(`joystick-${side}`);
  const knob     = document.getElementById(`knob-${side}`);
  const rect     = joystick.getBoundingClientRect();
  const cx = rect.width / 2, cy = rect.height / 2;
  const radius = cx - 10;
  let dx = (clientX - rect.left) - cx;
  let dy = (clientY - rect.top)  - cy;
  const dist = Math.sqrt(dx*dx + dy*dy);
  if (dist > radius) { dx = dx/dist*radius; dy = dy/dist*radius; }
  knob.style.transform = `translate(calc(-50% + ${dx}px), calc(-50% + ${dy}px))`;
  if (side === 'left')  { leftStick.rawX  =  dx/radius*100; leftStick.rawY  = -dy/radius*100; }
  else                  { rightStick.rawX =  dx/radius*100; rightStick.rawY = -dy/radius*100; }
}

/*======================== 网络处理 ========================*/
function sendToESP(url, data) {
  const t0 = performance.now();
  fetch(url, { method:'POST', headers:{'Content-Type':'application/json'}, body:JSON.stringify(data) })
    .then(r => { if (!r.ok) throw new Error(); updateLatency(performance.now() - t0); return r.json(); })
    .then(resp => {
      consecutiveFails = 0;
      updateConnectionStatus(true);
      const names = ['Directo','Acrobático','Estabilizado','Altura','Automático'];

      // 模式切换结果
      if (resp.m !== undefined) {
        if (resp.m !== currentFlightMode) {
          if (!resp.warn) showToast('✅ Modo: ' + (names[resp.m] || 'Desconocido'));
          const leftTouched = [...touches.values()].includes('left');
          if (!leftTouched) resetLeftStick(resp.m === 3 ? 0 : -100);
        }
        currentFlightMode = resp.m;
        document.getElementById('flight-mode').textContent = names[resp.m] || 'Estabilizado';
      }

      // ARM 状态更新（所有响应都同步显示）
      if (resp.arm !== undefined) {
        const el   = document.getElementById('armed-status');
        const item = document.getElementById('armed-status-item');
        el.textContent = resp.arm ? 'Activado' : 'Bloqueado';
        item.style.background = resp.arm ? 'rgba(0,255,136,0.15)' : 'rgba(255,51,51,0.15)';
        el.style.color = resp.arm ? '#00ff88' : '#ff6666';
      }

      // 按钮松开确认 toast（rt=2, bs=0）
      if (resp.rt === 2 && resp.bs === 0) {
        if (!resp.warn) {
          if (resp.bi === 0)
            showToast(resp.arm ? '✅ Motores activados' : '❌ No se pudieron activar');
          else if (resp.bi === 1)
            showToast(resp.arm ? '⚠️ No se pudieron bloquear' : '🔒 Motores bloqueados');
          else if (resp.bi === 2)
            showToast('🛑 Motores detenidos');
        }
        if (resp.warn) showToast('⚠️ ' + resp.warn);
      }
      // 心跳包携带的系统警告（低电自动上锁等），不与按钮 toast 冲突
      if (resp.rt === 4 && resp.warn) showToast('⚠️ ' + resp.warn);
    })
    .catch(() => {
      if (++consecutiveFails >= 3) updateConnectionStatus(false);
    });
}

function updateLatency(latency) {
  latencyHistory[latencyIndex] = latency;
  latencyIndex = (latencyIndex + 1) % latencyHistory.length;
  const avg = latencyHistory.reduce((a,b)=>a+b) / latencyHistory.length;
  document.getElementById('latency').textContent = Math.round(avg) + 'ms';
  const dot = document.querySelector('.status-dot');
  dot.className = avg > 200 ? 'status-dot warning' : avg > 100 ? 'status-dot warning' : 'status-dot connected';
}

function updateNetworkStatus() {
  const lr = packetStats.sent > 0 ? (packetStats.lost/packetStats.sent*100).toFixed(1) : '0';
  document.getElementById('packet-loss').textContent = lr + '%';
  fetch('/web_rc/status').then(r=>r.json()).then(d => {
    document.getElementById('battery').textContent = (d.voltage !== undefined && d.voltage !== null && d.voltage > 0.5) ? parseFloat(d.voltage).toFixed(2) + 'V' : '-';
  }).catch(()=>{ document.getElementById('battery').textContent = '-'; });
}

function updateConnectionStatus(connected) {
  connectionOk = connected;
  const dot  = document.getElementById('status-dot');
  const text = document.getElementById('connection-text');
  if (connected) { dot.className='status-dot connected'; text.textContent='Conectado'; text.style.color='#0f8'; }
  else           { dot.className='status-dot disconnected'; text.textContent='Desconectado'; text.style.color='#f33'; }
}
/*======================== 油门位置重置 ========================*/
function resetLeftStick(targetRawY) {
  const knob     = document.getElementById('knob-left');
  const joystick = document.getElementById('joystick-left');
  const rect     = joystick.getBoundingClientRect();
  if (rect.width === 0) return;
  const radius = rect.width / 2 - 10;
  const dy = -targetRawY / 100 * radius;
  knob.style.transition = 'transform 0.3s ease-out';
  knob.style.transform  = `translate(calc(-50% + 0px), calc(-50% + ${dy}px))`;
  setTimeout(() => { knob.style.transition = ''; }, 300);
  leftStick = {x:0, y:0, rawX:0, rawY: targetRawY};
  processJoystickInput();
  sendJoystickData();
}
/*======================== 按钮处理 ========================*/
function handleButton(idx) {
  if (idx === 4) { toggleConsole(); return; }
  if (idx === 5) { toggleCharts(); return; }
  if (idx === 3) {
    // Alterna directamente STAB y ALTHOLD; no pasa por ACRO durante el vuelo.
    let nextBit = currentFlightMode === 2 ? 8 : 6;
    sendButtonData(nextBit, 1);
    setTimeout(() => sendButtonData(nextBit, 0), 100);
    if (navigator.vibrate) navigator.vibrate(30);
    return;
  }
  // 解锁/上锁/急停：先发按下（state=1），100ms后发松开（state=0）
  // 后端响应中携带 rt/bi/bs，前端用这些字段判断 toast，无需 lastPressedButton
  if (idx === 0 || idx === 1 || idx === 2) {
    if (idx === 0)      showToast('🔓 Activando motores...');
    else if (idx === 1) showToast('🔒 Bloqueando motores...');
    else if (idx === 2) showToast('🛑 Enviando parada de emergencia...');
    sendButtonData(idx, 1);
    setTimeout(() => sendButtonData(idx, 0), 100);
    if (navigator.vibrate) navigator.vibrate(30);
  }
}

/*======================== Toast 提示 ========================*/
function showToast(msg) {
  let t = document.getElementById('toast-msg');
  if (!t) {
    t = document.createElement('div');
    t.id = 'toast-msg';
    t.style.cssText = 'position:fixed;top:50%;left:50%;transform:translate(-50%,-50%);background:rgba(0,0,0,.85);color:#fff;padding:12px 20px;border-radius:10px;font-size:14px;z-index:9999;pointer-events:none;text-align:center;max-width:80vw;border:1px solid rgba(255,255,255,.2);transition:opacity .3s';
    document.body.appendChild(t);
  }
  t.textContent = msg; t.style.opacity = '1';
  clearTimeout(t._timer);
  t._timer = setTimeout(() => { t.style.opacity = '0'; }, 2000);
}

/*======================== 调试控制台 ========================*/
function toggleConsole() {
  const panel = document.getElementById('console-panel');
  const btn   = document.getElementById('btn-5');
  const open  = !consolePanelOpen;
  graphPanelOpen = false;
  clearInterval(graphPollTimer);
  graphPollTimer = null;
  consolePanelOpen = open;
  panel.style.display = open ? 'flex' : 'none';
  document.getElementById('chart-view').style.display = 'none';
  document.getElementById('console-output').style.display = open ? 'block' : 'none';
  document.getElementById('console-controls').style.display = open ? 'flex' : 'none';
  const graphBtn = document.getElementById('btn-6');
  if (graphBtn) graphBtn.classList.remove('active');
  if (open) { panel.style.flexDirection = 'column'; }
  if (btn) { open ? btn.classList.add('active') : btn.classList.remove('active'); }
  if (open) {
    document.getElementById('console-output').innerHTML = '';
    consoleLastTotal = 0;
    fetch('/console/enable', {method:'POST'}).catch(()=>{});
    fetchConsoleLogs();
  } else {
    fetch('/console/disable', {method:'POST'}).catch(()=>{});
    clearTimeout(consolePollingTimer);
    consolePollingTimer = null;
  }
}

function toggleCharts() {
  const panel = document.getElementById('console-panel');
  const open = !graphPanelOpen;
  if (consolePanelOpen) fetch('/console/disable', {method:'POST'}).catch(()=>{});
  consolePanelOpen = false;
  graphPanelOpen = open;
  panel.style.display = open ? 'flex' : 'none';
  document.getElementById('console-output').style.display = 'none';
  document.getElementById('console-controls').style.display = 'none';
  document.getElementById('chart-view').style.display = open ? 'flex' : 'none';
  const consoleBtn = document.getElementById('btn-5');
  const graphBtn = document.getElementById('btn-6');
  if (consoleBtn) consoleBtn.classList.remove('active');
  if (graphBtn) open ? graphBtn.classList.add('active') : graphBtn.classList.remove('active');
  clearInterval(graphPollTimer);
  graphPollTimer = null;
  if (open) {
    graphSamples = [];
    fetchGraphTelemetry();
    graphPollTimer = setInterval(fetchGraphTelemetry, 250);
  }
}

function fetchGraphTelemetry() {
  if (!graphPanelOpen || graphFetchBusy) return;
  graphFetchBusy = true;
  fetch('/telemetry', {cache:'no-store'}).then(r=>r.json()).then(d => {
    if (!graphPanelOpen) return;
    graphSamples.push({ms:d.ms, z:d.z, target:d.target, vz:d.vz, vzTarget:d.vzTarget, modeId:d.modeId, baro:d.baro, armed:d.armed});
    const cutoff = d.ms - 30000;
    graphSamples = graphSamples.filter(s => s.ms >= cutoff).slice(-160);
    drawFlightPlots();
    document.getElementById('chart-mode').textContent = d.baro ? (d.mode + (d.armed ? ' · armado' : ' · bloqueado')) : 'DPS310 sin datos';
    const holdSamples = graphSamples.filter(s => s.baro && s.armed && s.modeId === 3);
    const rmse = (key, targetKey) => holdSamples.length
      ? Math.sqrt(holdSamples.reduce((sum,s) => sum + Math.pow(s[key]-s[targetKey],2),0) / holdSamples.length).toFixed(2)
      : '--';
    document.getElementById('chart-summary').textContent = d.baro
      ? `z ${d.z.toFixed(2)} m · vz ${d.vz.toFixed(2)} m/s · LQR ${d.az.toFixed(2)} m/s² · RMSE z ${rmse('z','target')} m · RMSE vz ${rmse('vz','vzTarget')} m/s (${holdSamples.length} muestras ALTHOLD armado)`
      : 'Sin datos válidos del DPS310; no se puede graficar la altura.';
  }).catch(()=>{
    const status = document.getElementById('chart-mode');
    if (status) status.textContent = 'Error leyendo telemetría';
  }).finally(()=>{ graphFetchBusy = false; });
}

function drawFlightPlots() {
  drawLineChart('altitude-chart', [
    {key:'z', label:'EKF', color:'#42d392'},
    {key:'target', label:'Objetivo', color:'#ffbb55'}
  ], 'm');
  drawLineChart('vertical-speed-chart', [
    {key:'vz', label:'EKF', color:'#42d392'},
    {key:'vzTarget', label:'Objetivo', color:'#ffbb55'}
  ], 'm/s');
}

function drawLineChart(id, series, unit) {
  const canvas = document.getElementById(id);
  if (!canvas) return;
  const rect = canvas.getBoundingClientRect();
  if (rect.width < 10 || rect.height < 10) return;
  const dpr = window.devicePixelRatio || 1;
  canvas.width = Math.round(rect.width * dpr);
  canvas.height = Math.round(rect.height * dpr);
  const ctx = canvas.getContext('2d');
  ctx.setTransform(dpr,0,0,dpr,0,0);
  const w = rect.width, h = rect.height, left = 38, right = 8, top = 5, bottom = 16;
  ctx.clearRect(0,0,w,h);
  if (graphSamples.length < 2) { ctx.fillStyle='#aaa'; ctx.font='10px sans-serif'; ctx.fillText('Esperando muestras…',left, h/2); return; }
  let vals = [];
  series.forEach(s => graphSamples.forEach(p => { if (Number.isFinite(p[s.key])) vals.push(p[s.key]); }));
  let minY = Math.min(...vals), maxY = Math.max(...vals);
  const span = Math.max(maxY-minY, unit === 'm' ? 0.3 : 0.2);
  minY -= span * .12; maxY += span * .12;
  const x0 = graphSamples[0].ms, xSpan = Math.max(1, graphSamples[graphSamples.length-1].ms - x0);
  const plotW = w-left-right, plotH = h-top-bottom;
  ctx.font='9px sans-serif'; ctx.textAlign='right'; ctx.textBaseline='middle'; ctx.fillStyle='#9aa';
  for (let i=0;i<=2;i++) {
    const y=top+plotH*i/2, value=maxY-(maxY-minY)*i/2;
    ctx.strokeStyle='rgba(255,255,255,.12)'; ctx.beginPath(); ctx.moveTo(left,y); ctx.lineTo(w-right,y); ctx.stroke();
    ctx.fillText(value.toFixed(1),left-4,y);
  }
  series.forEach(s => {
    ctx.strokeStyle=s.color; ctx.lineWidth=1.7; ctx.beginPath();
    let started=false;
    graphSamples.forEach(p => {
      const value=p[s.key]; if (!Number.isFinite(value)) return;
      const x=left+(p.ms-x0)/xSpan*plotW, y=top+(maxY-value)/(maxY-minY)*plotH;
      if (!started) {ctx.moveTo(x,y);started=true;} else ctx.lineTo(x,y);
    });
    ctx.stroke();
  });
  ctx.textAlign='left'; ctx.textBaseline='alphabetic';
  let lx=left;
  series.forEach(s => { ctx.fillStyle=s.color; ctx.fillRect(lx, h-9, 8, 2); ctx.fillStyle='#ccc'; ctx.fillText(s.label,lx+11,h-6); lx+=58; });
  ctx.textAlign='right'; ctx.fillStyle='#9aa'; ctx.fillText('ahora',w-right,h-6);
}

function scheduleConsolePoll(delayMs) {
  if (!consolePanelOpen) return;
  clearTimeout(consolePollingTimer);
  consolePollingTimer = setTimeout(fetchConsoleLogs, delayMs);
}

function fetchConsoleLogs() {
  if (!consolePanelOpen) return;
  if (consoleFetchInFlight) {
    scheduleConsolePoll(CONSOLE_CATCHUP_POLL_MS);
    return;
  }

  consoleFetchInFlight = true;
  fetch('/console?since=' + consoleLastTotal + '&limit=' + CONSOLE_PAGE_LIMIT).then(r=>r.json()).then(data => {
    const out = document.getElementById('console-output');
    if (data.lines && data.lines.length > 0) {
      const frag = document.createDocumentFragment();
      data.lines.forEach(l => {
        const div = document.createElement('div');
        div.textContent = l;
        frag.appendChild(div);
      });
      out.appendChild(frag);
      out.scrollTop = out.scrollHeight;
      // 限制 DOM 行数，避免长时间运行内存泄漏
      while (out.children.length > 200) out.removeChild(out.firstChild);
    }

    if (typeof data.next === 'number') consoleLastTotal = data.next;
    else if (typeof data.total === 'number') consoleLastTotal = data.total;

    scheduleConsolePoll(data.has_more ? CONSOLE_CATCHUP_POLL_MS : CONSOLE_BASE_POLL_MS);
  }).catch(()=>{
    scheduleConsolePoll(CONSOLE_BASE_POLL_MS);
  }).finally(() => {
    consoleFetchInFlight = false;
  });
}

function sendConsoleCmd() {
  const input = document.getElementById('console-input');
  const cmd = input.value.trim();
  if (!cmd) return;
  input.value = '';
  fetch('/console/cmd', {method:'POST', headers:{'Content-Type':'text/plain'}, body:cmd})
    .then(r => r.json())
    .then(resp => {
      if (!resp.ok) {
        if (resp.e === 'queue full') showToast('⚠️ Consola ocupada; inténtalo de nuevo');
        return;
      }
      fetchConsoleLogs();
    }).catch(()=>{});
}

// 回车发送 + ↑/↓ 命令历史
(function(){
  const cmdHistory = [];
  let historyIdx   = -1;
  document.getElementById('console-input').addEventListener('keydown', function(e) {
    if (e.key === 'Enter') {
      e.preventDefault();
      const v = this.value.trim();
      if (v) { cmdHistory.unshift(v); if (cmdHistory.length > 20) cmdHistory.pop(); }
      sendConsoleCmd();
      historyIdx = -1;
    } else if (e.key === 'ArrowUp') {
      e.preventDefault();
      if (historyIdx < cmdHistory.length - 1) { historyIdx++; this.value = cmdHistory[historyIdx]; }
    } else if (e.key === 'ArrowDown') {
      e.preventDefault();
      if (historyIdx > 0) { historyIdx--; this.value = cmdHistory[historyIdx]; }
      else { historyIdx = -1; this.value = ''; }
    }
  });
})();

/*======================== 事件绑定 ========================*/
document.addEventListener('DOMContentLoaded', init);
document.addEventListener('contextmenu', e => e.preventDefault());

// 心跳：2000ms，连续3次失败才判定断连
setInterval(() => {
  if (connectionOk) sendToESP('/web_rc/heartbeat', {t:4, ts:performance.now()});
}, 2000);
</script>
</body>
</html>
)rawliteral";

#endif
