#pragma once

#include <ESPAsyncWebServer.h>

const char style_css[] PROGMEM = R"rawliteral(
* {
  margin: 0;
  padding: 0;
}

html, body {
  width: 100%%;
  height: 100%%;
  background-color: #000000;
  user-select: none;
  -webkit-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
}

.tc1 { /* Text Color 1 */
  color: #FF6200;
}

.tc2 { /* Text Color 2 */
  color: #FF0000;
}

.tc3 { /* Text Color 3 */
  color: #8D0000;
}

.bold { /* Text Bold */
  font-weight: bold;
}

h1, h2, h3, h4, h5, h6, p {
  text-align: center;
  font-weight: bold;
}

.tac { /* Text Center */
  text-align: center;
}

.tal { /* Text Left */
  text-align: left;
}

.tar { /* Text Right */
  text-align: right;
}

header {
  width: 100%%;
  height: 50px;
}

.box {
  border: 2px solid #8D0000;
  border-radius: 20px;
}

.control-pad {
  display: grid;
  grid-template-columns: repeat(3, 90px);
  grid-gap: 10px;
  grid-auto-rows: 90px;
  justify-content: center;
  margin-top: 10px;
}
.control-pad > .N {
  grid-column: 2;
  grid-row: 1;
}
.control-pad > .W {
  grid-column: 1;
  grid-row: 2;
}
.control-pad > .E {
  grid-column: 3;
  grid-row: 2;
}
.control-pad > .S {
  grid-column: 2;
  grid-row: 3;
}
.control-pad .triangle {
  fill: #8D0000;
  cursor: pointer;
}
.control-pad .triangle:hover {
  fill: #FF0000;
}
.control-pad .triangle:active {
  fill: #FF6200;
}

.number-control {
  display: flex;
  flex-direction: column;
  width: 90px;
}
.number-control .number-input {
  width: 100%%;
  text-align: center;
  border: 2px solid #8D0000;
  background-color: transparent;
  color: #8D0000;
  font-size: 1.5rem;
  cursor: pointer;
  -webkit-appearance: textfield;
          appearance: textfield;
  -moz-appearance: textfield;
}
.number-control .number-input::-webkit-inner-spin-button, .number-control .number-input::-webkit-outer-spin-button {
  -webkit-appearance: none;
  margin: 0;
}
.number-control button {
  display: none;
}

main {
  width: 60%%;
  display: flex;
  gap: 10px;
  margin: 0 auto;
  padding-top: 30px;
}
main > div:nth-child(1) {
  flex: 3;
  display: flex;
  flex-direction: column;
  gap: 10px;
}
main > div:nth-child(1) > div:nth-child(1) { /* TARGET */
  flex: 1;
}
main > div:nth-child(1) > div:nth-child(1) > div {
  display: flex;
  margin-top: 30px;
}
main > div:nth-child(1) > div:nth-child(1) > div > div {
  display: grid;
  grid-template-columns: repeat(3, 100px);
  grid-template-rows: 50px 30px 32px 30px;
  margin: 0 auto;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > h2 {
  grid-column: 1/4;
  grid-row: 1;
  line-height: 50px;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div {
  display: grid;
  grid-template-columns: repeat(3, 100px);
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div:nth-child(2) {
  grid-row: 2;
  color: #8D0000;
  line-height: 30px;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div:nth-child(3) {
  grid-row: 3;
  line-height: 50px;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div:nth-child(4) {
  grid-row: 4;
  color: #FF6200;
  line-height: 30px;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div > div {
  margin: 0 auto;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div > div:nth-child(1) {
  grid-column: 1;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div > div:nth-child(2) {
  grid-column: 2;
}
main > div:nth-child(1) > div:nth-child(1) > div > div > div > div:nth-child(3) {
  grid-column: 3;
}
main > div:nth-child(1) > div:nth-child(1) > button {
  width: 120px;
  height: 40px;
  background-color: black;
  border-radius: 0px;
  font-size: 1rem;
  margin: 0px auto;
  cursor: pointer;
  display: block;
  margin: 0 auto;
  margin-top: 30px;
}
main > div:nth-child(1) > div:nth-child(1) > button:hover {
  background-color: #610000;
}
main > div:nth-child(1) > div:nth-child(1) > button:active {
  background-color: #FF6200;
  color: #FF0000;
}
main > div:nth-child(1) > div:nth-child(1) > button.click_pls {
  animation: pulse 1.5s infinite;
  position: relative;
}
@keyframes pulse {
  0%% {
    box-shadow: 0 0 0 0 rgba(255, 98, 0, 0.7);
  }
  70%% {
    box-shadow: 0 0 0 10px rgba(255, 98, 0, 0);
  }
  100%% {
    box-shadow: 0 0 0 0 rgba(255, 98, 0, 0);
  }
}
main > div:nth-child(1) > div:nth-child(2) {
  flex: 1;
  display: flex;
  gap: 10px;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(1) { /* RA / DEC*/
  flex: 2;
  width: 100%%;
  aspect-ratio: 1/1;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(2) { /* AZ / ALT*/
  flex: 2;
  width: 100%%;
  aspect-ratio: 1/1;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(3) { /* STOP */
  flex: 1.5;
  width: 100%%;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(3) > div {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100%%;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(3) > div > div {
  width: 80%%;
  aspect-ratio: 1/1;
  border: 3px solid #FF0000;
  border-radius: 50%%;
  display: flex;
  justify-content: center;
  align-items: center;
  cursor: pointer;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(3) > div > div:hover {
  background-color: #610000;
}
main > div:nth-child(1) > div:nth-child(2) > div:nth-child(3) > div > div:active {
  background-color: #FF6200;
}
main > div:nth-child(2) {
  flex: 1;
}
main > div:nth-child(2) > div:nth-child(1) { /* CONFIG */ }
main > div:nth-child(2) > div:nth-child(1) > div {
  display: flex;
  margin-top: 15px;
  justify-content: space-around;
}
main > div:nth-child(2) > div:nth-child(1) > div > button {
  width: 40%%;
  height: 40px;
  background-color: black;
  border-radius: 0px;
  font-size: 1rem;
  cursor: pointer;
}
main > div:nth-child(2) > div:nth-child(1) > div > button:hover {
  background-color: #610000;
}
main > div:nth-child(2) > div:nth-child(1) > div > button:active {
  background-color: #FF6200;
  color: #FF0000;
}
main > div:nth-child(2) > div:nth-child(2) { /* STAR */
  margin-top: 15px;
}
main > div:nth-child(2) > div:nth-child(2) > div {
  display: flex;
  flex-direction: column;
  height: 100%%;
}
main > div:nth-child(2) > div:nth-child(2) > div > hr {
  width: 90%%;
  margin: 0 auto;
  border: 1px solid #610000;
}
main > div:nth-child(2) > div:nth-child(2) > div > div {
  width: 90%%;
  margin: 0 auto;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul { /* STAR LIST */
  height: 500px;
  list-style: none;
  overflow-y: scroll;
  scrollbar-width: thin;
  scrollbar-color: #610000 transparent;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul::-webkit-scrollbar {
  width: 6px;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul::-webkit-scrollbar-track {
  background: transparent;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul::-webkit-scrollbar-thumb {
  background-color: #610000;
  border-radius: 3px;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul > li {
  padding: 5px 0px 5px 10px;
  color: #8D0000;
  border-bottom: 1px solid #610000;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul > li:hover {
  color: #FF0000;
}
main > div:nth-child(2) > div:nth-child(2) > div > div > ul > li.selected {
  color: #FF6200;
}
main > div:nth-child(2) > div:nth-child(2) > div > button { /* TRACKING */
  width: 120px;
  height: 40px;
  background-color: black;
  border-radius: 0px;
  font-size: 1rem;
  margin: 10px auto;
  cursor: pointer;
}
main > div:nth-child(2) > div:nth-child(2) > div > button:hover {
  background-color: #610000;
}
main > div:nth-child(2) > div:nth-child(2) > div > button:active {
  background-color: #FF6200;
  color: #FF0000;
}

footer {
  width: 100%%;
  height: auto;
  display: flex;
  justify-content: space-between;
  align-items: flex-end;
  position: fixed;
  bottom: 0;
  left: 0;
  padding: 0 10px 10px 10px;
  box-sizing: border-box;
}
footer > div:nth-child(1) {
  flex: 1;
}
footer > div:nth-child(2) {
  display: flex;
  flex-direction: column;
  gap: 10px;
  color: #FF0000;
  position: absolute;
  left: 50%%;
  transform: translateX(-50%%);
}
footer > div:nth-child(2) > div {
  display: flex;
  color: #8D0000;
  border: 1px solid #8D0000;
  justify-content: space-between;
}
footer > div:nth-child(2) > div > h3 {
  width: 50px;
  border: 1px solid #8D0000;
}
footer > div:nth-child(2) > div > h3:hover {
  background-color: #610000;
  color: #FF0000;
  border: 1px solid #FF0000;
}
footer > div:nth-child(2) > div > h3:active {
  background-color: #8D0000;
}
footer > div:nth-child(2) > div > h3.selected {
  background-color: #FF6200;
  color: #FF0000;
  border: 1px solid #FF0000;
}
footer > div:nth-child(3) {
  flex: 1;
  text-align: right;
}/*# sourceMappingURL=style.css.map */
)rawliteral";

const char script_js[] PROGMEM = R"rawliteral(
/*
Home 버튼이나 Slew 버튼을 눌르면 Target의 RA와 DEC 조정하는 값이 자동으로 바뀌어서 그거에서 트래킹이 되도록 하기
단 화살표로 조정하는 RA / DEC은 눌러도 Target의 조정 값이 변하지 않도록 하기

js에서는 그냥 

전역 변수 설정하기
*/

class TargetRA {
    constructor(h = 0, m = 0, s = 0) {
        this._h = h;
        this._m = m;
        this._s = s;

        this.get = {
            get h() {
                return this._h;
            },
            get m() {
                return this._m;
            },
            get s() {
                return this._s;
            }
        };

        this.set = {
            set h(value) {
                this._h = value;
            },
            set m(value) {
                this._m = value;
            },
            set s(value) {
                this._s = value;
            }
        };
    }
    
    get() {
        return {
            h: this._h,
            m: this._m,
            s: this._s
        };
    }
    set(h, m, s) {
        if (h !== undefined) this._h = h;
        if (m !== undefined) this._m = m;
        if (s !== undefined) this._s = s;
    }
}

class TargetDEC {
    constructor(d = 0, m = 0, s = 0) {
        this._d = d; // °
        this._m = m;
        this._s = s;

        this.get = {
            get d() {
                return this._d;
            },
            get m() {
                return this._m;
            },
            get s() {
                return this._s;
            }
        };

        this.set = {
            set d(value) {
                this._d = value;
            },
            set m(value) {
                this._m = value;
            },
            set s(value) {
                this._s = value;
            }
        };
    }
    
    get() {
        return {
            d: this._d,
            m: this._m,
            s: this._s
        };
    }
    set(d, m, s) {
        if (d !== undefined) this._d = d;
        if (m !== undefined) this._m = m;
        if (s !== undefined) this._s = s;
    }
}

let targetRA = new TargetRA();
let targetDEC = new TargetDEC();

// 개발 모드 설정
const DEV_MODE = false;

// HTTP 요청 전송 함수
function sendRequest(endpoint, logMessage) {
    const logElement = document.getElementById('log');
    
    if (DEV_MODE) {
        // 개발 모드일 때는 실제 요청을 보내지 않고 로그만 출력
        console.log('[DEV] Request:', endpoint, logMessage);
        logElement.textContent = `[DEV] ${logMessage}(${endpoint})`;
        return;
    }
    
    fetch(`http://192.168.4.1:${endpoint}`)
        .then(response => {
            if (!response.ok) {
                throw new Error('네트워크 응답이 올바르지 않습니다');
            }
            return response.text();
        })
        .then(data => {
            console.log('Success: ', logMessage, data);
            logElement.textContent = `Success: ${logMessage} (${data})`;
        })
        .catch(error => {
            console.error('Error: ', logMessage, error);
            logElement.textContent = `Error: ${logMessage} (${error})`;
        });
}

// Log에 문자열 출력
function log(message) {
    const logElement = document.getElementById('log');
    logElement.textContent = message;
}

// 별 목록 데이터 (나중에 ESP32에서 가져오는 방법으로 변경할 예정, 별 위치들도 가져오도록 할 것임, 별 가까운 순위로 정렬도 되게)
const starList = [
    "Sirius", "Canopus", "Arcturus", "Vega", "Capella", 
    "Rigel", "Procyon", "Betelgeuse", "Altair", "Aldebaran", 
    "Antares", "Spica", "Pollux", "Fomalhaut", "Deneb", 
    "Regulus", "Castor", "Polaris", "Mimosa", "Bellatrix"
];

document.addEventListener('DOMContentLoaded', () => {
    const starListElement = document.getElementById('star-list');
    let selectedStar = null; // 선택된 별을 저장할 변수
    
    // 별 목록 추가
    starList.forEach((star, index) => {
        const li = document.createElement('li');
        li.textContent = star;
        
        li.addEventListener('click', () => {
            if (li.classList.contains('selected')) {
                li.classList.remove('selected');
                selectedStar = null;
            } else {
                document.querySelectorAll('#star-list li').forEach(item => {
                    item.classList.remove('selected');
                });
                li.classList.add('selected');
                selectedStar = star;
            }
            console.log('선택된 별:', selectedStar);
            log(`선택된 별: ${selectedStar}`);
        });
        
        starListElement.appendChild(li);
    });
    
    // 모든 number-control 요소에 대해 이벤트 설정
    document.querySelectorAll('.number-control').forEach(control => {
        const input = control.querySelector('.number-input');
        const increment = control.querySelector('.increment');
        const decrement = control.querySelector('.decrement');
        
        let isDragging = false;
        let startY = 0;
        let startValue = 0;
        
        // 버튼 클릭
        increment.addEventListener('click', () => {
            input.value = Math.min(parseInt(input.value) + 1, parseInt(input.max));
        });
        
        decrement.addEventListener('click', () => {
            input.value = Math.max(parseInt(input.value) - 1, parseInt(input.min));
        });
        
        // 마우스 드래그
        input.addEventListener('mousedown', (e) => {
            isDragging = true;
            startY = e.clientY;
            startValue = parseInt(input.value);
            e.stopPropagation(); // 이벤트 전파 중단
        });
        
        document.addEventListener('mousemove', (e) => {
            if (!isDragging) return;
            
            const diff = startY - e.clientY;
            const newValue = startValue + Math.round(diff / 5);
            
            input.value = Math.max(Math.min(newValue, parseInt(input.max)), parseInt(input.min));
        });
        
        document.addEventListener('mouseup', () => {
            isDragging = false;
        });
        
        // 스크롤
        input.addEventListener('wheel', (e) => {
            e.preventDefault();
            if (e.deltaY < 0) {
                input.value = Math.min(parseInt(input.value) + 1, parseInt(input.max));
            } else {
                input.value = Math.max(parseInt(input.value) - 1, parseInt(input.min));
            }
        });
        
        // 직접 입력
        input.addEventListener('change', () => {
            const value = parseInt(input.value);
            input.value = Math.max(Math.min(value, parseInt(input.max)), parseInt(input.min));
        });
    });

    // 버튼 클릭 이벤트 리스너 예시
    document.getElementById('btn_stop').addEventListener('click', () => {
        sendRequest('/stop', '긴급 정지');
    });

    // RA/DEC 컨트롤 패드 이벤트
    const raDecControl = document.getElementById('ra-dec-control');
    const raDecTriangles = raDecControl.querySelectorAll('.triangle');
    let activeRADECTriangle = null;
    
    raDecTriangles.forEach(triangle => {
        triangle.addEventListener('mousedown', () => {
            activeRADECTriangle = triangle;
            const direction = triangle.parentElement.classList[0];
            sendRequest(`/ra-dec/control?direction=${direction.toLowerCase()}`, `RA/DEC ${direction.toLowerCase()}`);
        });
    });
    
    // AZ/ALT 컨트롤 패드 이벤트  
    // AZ/ALT의 경우 눌를 때 마다 한번씩만 이동하기 때문에 떼는건 감지 안해도 됨
    const azAltControl = document.getElementById('az-alt-control');
    const azAltTriangles = azAltControl.querySelectorAll('.triangle');
    let activeAZALTTriangle = null;
    
    azAltTriangles.forEach(triangle => {
        triangle.addEventListener('mousedown', () => {
            activeAZALTTriangle = triangle;
            const direction = triangle.parentElement.classList[0];
            sendRequest(`/az-alt/control?direction=${direction.toLowerCase()}`, `AZ/ALT ${direction.toLowerCase()}`);
        });
    });
    
    // 전역 마우스 업 이벤트 - RA/DEC
    document.addEventListener('mouseup', () => {
        if (activeRADECTriangle) {
            const direction = activeRADECTriangle.parentElement.classList[0];
            sendRequest(`/ra-dec/stop?direction=${direction.toLowerCase()}`, `RA/DEC STOP ${direction.toLowerCase()}`);
            activeRADECTriangle = null;
        }
    });
});


)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>

<html lang="en">
<head>
<meta charset="utf-8"/>
<meta content="width=device-width, initial-scale=1.0" name="viewport"/>
<title>ESP32 OAT CONTROL</title>
<style>%STYLE_PLACEHOLDER%</style>
</head>
<body>
<header>
<h1 class="tc1">ESP32 OAT CONTROL</h1>
</header>
<main>
<div>
<div class="box">
<h2 class="tc2">TARGET</h2>
<div>
<div>
<h2 class="tc2">RA</h2>
<div>
<h3>0</h3>
<h3>0</h3>
<h3>0</h3>
</div>
<div>
<div class="number-control">
<button class="increment"></button>
<input class="number-input" id="ra-h" max="23" min="0" step="1" type="number" value="0"/>
<button class="decrement"></button>
</div>
<div class="number-control">
<button class="increment"></button>
<input class="number-input" id="ra-m" max="59" min="0" step="1" type="number" value="0"/>
<button class="decrement"></button>
</div>
<div class="number-control">
<button class="increment"></button>
<input class="number-input" id="ra-s" max="59" min="0" step="1" type="number" value="0"/>
<button class="decrement"></button>
</div>
</div>
<div>
<h2>H</h2>
<h2>M</h2>
<h2>S</h2>
</div>
</div>
<div>
<h2 class="tc2">DEC</h2>
<div>
<h3>0</h3>
<h3>0</h3>
<h3>0</h3>
</div>
<div>
<div class="number-control">
<button class="increment"></button>
<input class="number-input" id="dec-d" max="90" min="-90" step="1" type="number" value="0"/>
<button class="decrement"></button>
</div>
<div class="number-control">
<button class="increment"></button>
<input class="number-input" id="dec-m" max="59" min="0" step="1" type="number" value="0"/>
<button class="decrement"></button>
</div>
<div class="number-control">
<button class="increment"></button>
<input class="number-input" id="dec-s" max="59" min="0" step="1" type="number" value="0"/>
<button class="decrement"></button>
</div>
</div>
<div>
<h2>°</h2>
<h2>M</h2>
<h2>S</h2>
</div>
</div>
</div>
<button class="box tc1 click_pls" id="btn_target_tracking">TRACKING</button>
</div>
<div>
<div class="box">
<h2 class="tc2">RA / DEC</h2>
<div class="control-pad" id="ra-dec-control">
<svg class="N" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="50 15, 100 100, 0 100"></polygon>
</svg>
<svg class="W" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="15 50, 100 0, 100 100"></polygon>
</svg>
<svg class="E" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="85 50, 0 0, 0 100"></polygon>
</svg>
<svg class="S" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="50 85, 0 0, 100 0"></polygon>
</svg>
</div>
</div>
<div class="box">
<h2 class="tc2">AZ / ALT</h2>
<div class="control-pad" id="az-alt-control">
<svg class="N" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="50 15, 100 100, 0 100"></polygon>
</svg>
<svg class="W" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="15 50, 100 0, 100 100"></polygon>
</svg>
<svg class="E" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="85 50, 0 0, 0 100"></polygon>
</svg>
<svg class="S" height="80" viewbox="0 0 100 100" width="80">
<polygon class="triangle" points="50 85, 0 0, 100 0"></polygon>
</svg>
</div>
</div>
<div class="box">
<h2 class="tc2">STOP</h2>
<div>
<div id="btn_stop">
<h2 class="tc2">STOP</h2>
</div>
</div>
</div>
</div>
</div>
<div class="box">
<div>
<h2 class="tc2">CONFIG</h2>
<div>
<button class="box tc1" id="btn_auto-home">AUTO HOME</button>
<button class="box tc1" id="btn_home">HOME</button>
</div>
</div>
<div>
<h2 class="tc2">STAR</h2>
<div>
<hr/>
<div>
<ul id="star-list">
</ul>
</div>
<button class="box tc1" id="btn_star_tracking">TRACKING</button>
</div>
</div>
</div>
</main>
<footer>
<div>
<p class="tc2 bold tal">%GPSINFO%</p>
<p class="tc2 bold tal">%FIRMWAREMODEL%</p>
<p class="tc2 bold tal">%FIRMWAREVERSION%</p>
</div>
<div>
<h3>TRACKING RATE</h3>
<div>
<h3 class="selected">1</h3>
<h3>2</h3>
<h3>3</h3>
<h3>4</h3>
<h3>5</h3>
</div>
</div>
<div>
<p class="tc2 bold tar" id="log">LOG</p>
</div>
</footer>
<script>%SCRIPT_PLACEHOLDER%</script>
</body>
</html>
)rawliteral";
