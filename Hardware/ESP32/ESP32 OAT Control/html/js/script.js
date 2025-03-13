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

// HTTP 요청 전송 함수
function sendRequest(endpoint, logMessage) {
    const logElement = document.getElementById('log');
    
    fetch(`http://localhost:${endpoint}`)
        .then(response => {
            if (!response.ok) {
                throw new Error('네트워크 응답이 올바르지 않습니다');
            }
            return response.text();
        })
        .then(data => {
            console.log('Success: ', logMessage, data);
            logElement.textContent = `Success: ${logMessage}(${data})`;
        })
        .catch(error => {
            console.error('Error: ', logMessage, error);
            logElement.textContent = `Error: ${logMessage}(${error})`;
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

    // div 클릭 이벤트 리스너 예시
    document.getElementById('btn_auto-home').addEventListener('click', () => {
        sendRequest('/auto-home', '자동 홈 이동');
    });
    document.getElementById('btn_home').addEventListener('click', () => {
        sendRequest('/home', '수동 홈 이동');
    });
});

