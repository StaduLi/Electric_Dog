let coordinate = {
    x: 0,
    y: 0
};

const STOP = 0;
const FORWARD = 1;
const BACKWARD = 2;
const RIGHT = 3;
const LEFT = 4;

let direction = STOP;
let direction_temp = direction;


/**********************************/
/***********数据上传处理************/
/**********************************/
const RUN = "http://192.168.5.1/run";
const GET_DOG_STATE = "http://192.168.5.1/getDogState";
const ACTION = "http://192.168.5.1/action";



//查询摇杆坐标解析并发送
setInterval(() => {
    paraseCoordinate();
    if (direction_temp != direction) {
        sendRunCode();
        direction_temp = direction;
    }
}, 1);

function sendRunCode() {
    let run_code = direction;
    let sendRequest = new XMLHttpRequest();
    sendRequest.open("GET", RUN + '?run_code=' + run_code, true);
    sendRequest.send(null);

    sendRequest.onreadystatechange = function () {
        if (sendRequest.readyState === 4 && sendRequest.status === 200) {

        }
    };
}

function paraseCoordinate() {
    if (coordinate.x > 0) {
        if (-coordinate.x < coordinate.y && coordinate.y < coordinate.x)
            direction = RIGHT;
        else if (-coordinate.x >= coordinate.y)
            direction = FORWARD;
        else if (coordinate.y >= coordinate.x)
            direction = BACKWARD;
    } else if (coordinate.x < 0) {
        if (-coordinate.x > coordinate.y && coordinate.y > coordinate.x)
            direction = LEFT;
        else if (coordinate.x >= coordinate.y)
            direction = FORWARD;
        else if (coordinate.y >= -coordinate.x)
            direction = BACKWARD;

    } else {
        if (coordinate.y > 0)
            direction = BACKWARD;
        else if (coordinate.y < 0)
            direction = FORWARD;
        else
            direction = STOP;
    }
}


/**********************************/
/***********相对位置处理************/
/**********************************/

// 获取元素
const joystickContainer = document.getElementById('joystickContainer');
const joystickBackground = document.getElementById('joystickBackground');
const joystick = document.getElementById('joystick');

// 初始位置
let joystickX = joystickContainer.offsetWidth / 2 - joystick.offsetWidth / 2;
let joystickY = joystickContainer.offsetHeight / 2 - joystick.offsetHeight / 2;
joystick.style.left = joystickX + 'px';
joystick.style.top = joystickY + 'px';

let isDragging = false;
let startX, startY;

joystick.addEventListener('touchstart', startDragTouch);
joystick.addEventListener('mousedown', startDrag);

function startDragTouch(e) {
    e.preventDefault();
    isDragging = true;
    startX = e.touches[0].clientX - joystickContainer.getBoundingClientRect().left;
    startY = e.touches[0].clientY - joystickContainer.getBoundingClientRect().top;
    document.addEventListener('touchmove', dragJoystickTouch);
    document.addEventListener('touchend', stopDragTouch);
}

function startDrag(e) {
    e.preventDefault();
    isDragging = true;
    startX = e.clientX - joystickContainer.getBoundingClientRect().left;
    startY = e.clientY - joystickContainer.getBoundingClientRect().top;
    document.addEventListener('mousemove', dragJoystick);
    document.addEventListener('mouseup', stopDrag);
}

function dragJoystickTouch(e) {
    if (!isDragging) return;
    e.preventDefault();
    let containerRect = joystickContainer.getBoundingClientRect();
    let mouseX = e.touches[0].clientX - containerRect.left;
    let mouseY = e.touches[0].clientY - containerRect.top;

    let maxDistance = joystickContainer.offsetWidth / 2 - joystick.offsetWidth / 2;
    let distance = Math.sqrt(Math.pow(mouseX - joystickContainer.offsetWidth / 2, 2) + Math.pow(mouseY - joystickContainer.offsetHeight / 2, 2));
    if (distance > maxDistance) {
        let ratio = maxDistance / distance;
        mouseX = joystickContainer.offsetWidth / 2 + (mouseX - joystickContainer.offsetWidth / 2) * ratio;
        mouseY = joystickContainer.offsetHeight / 2 + (mouseY - joystickContainer.offsetHeight / 2) * ratio;
    }

    joystick.style.left = mouseX - joystick.offsetWidth / 2 + 'px';
    joystick.style.top = mouseY - joystick.offsetHeight / 2 + 'px';

    coordinate.x = (mouseX - joystickContainer.offsetWidth / 2) / maxDistance;
    coordinate.y = (mouseY - joystickContainer.offsetHeight / 2) / maxDistance;
}

function dragJoystick(e) {
    if (!isDragging) return;
    e.preventDefault();
    let containerRect = joystickContainer.getBoundingClientRect();
    let mouseX = e.clientX - containerRect.left;
    let mouseY = e.clientY - containerRect.top;

    let maxDistance = joystickContainer.offsetWidth / 2 - joystick.offsetWidth / 2;
    let distance = Math.sqrt(Math.pow(mouseX - joystickContainer.offsetWidth / 2, 2) + Math.pow(mouseY - joystickContainer.offsetHeight / 2, 2));
    if (distance > maxDistance) {
        let ratio = maxDistance / distance;
        mouseX = joystickContainer.offsetWidth / 2 + (mouseX - joystickContainer.offsetWidth / 2) * ratio;
        mouseY = joystickContainer.offsetHeight / 2 + (mouseY - joystickContainer.offsetHeight / 2) * ratio;
    }

    joystick.style.left = mouseX - joystick.offsetWidth / 2 + 'px';
    joystick.style.top = mouseY - joystick.offsetHeight / 2 + 'px';

    coordinate.x = (mouseX - joystickContainer.offsetWidth / 2) / maxDistance;
    coordinate.y = (mouseY - joystickContainer.offsetHeight / 2) / maxDistance;
}

function stopDragTouch() {
    isDragging = false;
    joystick.style.left = joystickX + 'px';
    joystick.style.top = joystickY + 'px';
    document.removeEventListener('touchmove', dragJoystickTouch);
    document.removeEventListener('touchend', stopDragTouch);

    coordinate.x = coordinate.y = 0;
}

function stopDrag() {
    isDragging = false;
    joystick.style.left = joystickX + 'px';
    joystick.style.top = joystickY + 'px';
    document.removeEventListener('mousemove', dragJoystick);
    document.removeEventListener('mouseup', stopDrag);

    coordinate.x = coordinate.y = 0;
}

/**********************************/
/***********查询是否充电************/
/**********************************/

setInterval(() => {
    let sendRequest = new XMLHttpRequest();
    sendRequest.open("GET", GET_DOG_STATE, true);
    sendRequest.send(null);

    console.log("Function test");

    sendRequest.onreadystatechange = function () {
        if (sendRequest.readyState === 4 && sendRequest.status === 200) {
            let recData = sendRequest.responseText;
            console.log(recData);

            if (recData == "CHARGING")
                location.assign('./eating.html');
            else if (recData == "LOW_BATTERY")
                location.assign('./need_charge.html');
        }
    };
}, 3000);

/**********************************/
/***********按键动作处理************/
/**********************************/

const lieDownButton = document.getElementById('lie-down');
const standUpButton = document.getElementById('stand-up');
const swingButton = document.getElementById('swing');
const shakeHandsButton = document.getElementById('shake-hands');
const stretchButton = document.getElementById('stretch');
const actCuteButton = document.getElementById('act-cute');
//0 1 2 3 4  5

lieDownButton.addEventListener("click", () => {
    sendActionCode(0);
});
standUpButton.addEventListener("click", () => {
    sendActionCode(1);
});
swingButton.addEventListener("click", () => {
    sendActionCode(2);
});
shakeHandsButton.addEventListener("click", () => {
    sendActionCode(3);
});
stretchButton.addEventListener("click", () => {
    sendActionCode(4);
});
actCuteButton.addEventListener("click", () => {
    sendActionCode(5);
});


function sendActionCode(action_code) {
    console.log(`Send action_code = ${action_code}`);

    let sendRequest = new XMLHttpRequest();
    sendRequest.open("GET", `${ACTION}?action_code=${action_code}`, true);
    sendRequest.send(null);

    sendRequest.onreadystatechange = function () {
        if (sendRequest.readyState === 4 && sendRequest.status === 200) {

        }
    };

}




