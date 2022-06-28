// ===== CONST =====
const responseCodeOffset = [0, 1, 3, 6, 10];

const keyMap = {
    q: 0,
    w: 1,
    e: 2,
    r: 3,
    a: 4,
    s: 5,
    d: 6,
    f: 7,
};

const responseMap = {
    0: '0000',
    1: '1000',
    2: '2000',
    3: '1100',
    4: '2100',
    5: '2200',
    6: '1110',
    7: '2110',
    8: '2210',
    9: '2220',
    10: '1111',
    11: '2111',
    12: '2211',
    13: '2221',
    14: '2222',
};

const colorMap = {
    0: '#fc5c65',
    1: '#fed330',
    2: '#26de81',
    3: '#4b7bec',
    4: '#fd9644',
    5: '#a55eea',
    6: '#d1d8e0',
    7: '#778ca3',
};

const responseColorMap = {
    '1': '#ffffff',
    '2': '#dfbd69',
}

// label dimensions
const L_PAD_L = 40;
const LT_PAD_T = 38;
const LT_PAD_L = 70;

// cell dimensions
const C_PAD_T = 2;
const C_PAD_L = 157.5;

const C_DIM = 60;
const C_DIM_R = 10;
const C_DIM_GAP = 75;

// response dimensions
const R_PAD_T = 17;
const R_PAD_L = 482.5;

const R_DIM = 25;
const R_DIM_R = 5;
const R_DIM_GAP = 30;

// ===== DOC =====
let canvas;
let ctx;

// ===== DATA =====
let target;
let tCount;

let rowIndex;
let cellIndex
let rows;

let response;

// ===== FUNC =====
// initialise game
const init = () => {
    // setup variables
    target = [];
    
    let nums = [0, 1, 2, 3, 4, 5, 6, 7];
    for (let i = 0; i < 4; ++i)
        target.push(nums.splice(rand(nums.length), 1)[0]);

    tCount = count(target);

    rowIndex = 0;
    cellIndex = 0;

    rows = new Array(12);
    for (let i = 0; i < 12; ++i)
        rows[i] = new Array(4).fill(8);

    response = new Array(12).fill('0000');

    console.log(target);

    // render canvasz
    render();
}

// mark guess cell
const mark = key => {
    let map = keyMap[key];
    if (map === undefined)
        return;
    rows[rowIndex][cellIndex] = map;

    render();
}

// evaluate guess
const guess = () => {
    if (rowIndex == 12 || response[rowIndex - 1] === '2222')
        return init();

    for (let i = 0; i < 4; ++i)
        if (rows[rowIndex][i] == 8)
            return;

    // calculate response code
    let gCount = count(rows[rowIndex]);

    let colors = 8;
    for (let i = 0; i < 8; ++i)
        colors -= Math.abs(tCount[i] - gCount[i]);
    colors /= 2;

    let correct = 0;
    for (let i = 0; i < 4; ++i)
        correct += (rows[rowIndex][i] == target[i]);

    // update response
    response[rowIndex] = responseMap[responseCodeOffset[colors] + correct];

    rowIndex++;
    cellIndex = 0;
    render();
}

// render canvas
const render = () => {
    ctx.clearRect(150, 0, canvas.width, canvas.height);

    for (let i = 0; i < 12; ++i) {
        for (let j = 0; j < 4; ++j) {
            drawRoundRect(
                C_PAD_L + j * C_DIM_GAP,
                C_PAD_T + (11 - i) * C_DIM_GAP,
                C_DIM,
                C_DIM_R,
                colorMap[rows[i][j]],
                i == rowIndex && j == cellIndex
            );

            drawRoundRect(
                R_PAD_L + j * R_DIM_GAP,
                R_PAD_T + (11 - i) * C_DIM_GAP,
                R_DIM,
                R_DIM_R,
                responseColorMap[response[i][j]],
            );
        }
    }
}

// ===== AUX =====
// random number generator [0, n)
const rand = n => {
    return Math.floor(Math.random() * n);
}

// create count array
const count = arr => {
    let rt = [0, 0, 0, 0, 0, 0, 0, 0];
    arr.forEach(index => rt[index]++);
    return rt;
}

// draw rounded rectangle
const drawRoundRect = (x, y, dim, radius, color, select, outline = false) => {
    ctx.beginPath();

    ctx.moveTo(x + radius, y);
    ctx.lineTo(x + dim - radius, y);
    ctx.quadraticCurveTo(x + dim, y, x + dim, y + radius);
    ctx.lineTo(x + dim, y + dim - radius);
    ctx.quadraticCurveTo(x + dim, y + dim, x + dim - radius, y + dim);
    ctx.lineTo(x + radius, y + dim);
    ctx.quadraticCurveTo(x, y + dim, x, y + dim - radius);
    ctx.lineTo(x, y + radius);
    ctx.quadraticCurveTo(x, y, x + radius, y);

    if (outline) {
        ctx.strokeStyle = color;
        ctx.stroke();
    }
    else {
        if (color) {
            ctx.fillStyle = color;
            ctx.fill();
        }

        ctx.strokeStyle = select ? '#ffffff' : '#181818';
        ctx.stroke();
    }

    ctx.closePath();
}

// ===== BACKGROUND =====
// key stroke listener
document.addEventListener('keydown', event => {
    switch (event.key) {
        case 'ArrowLeft':
            if (cellIndex === 0)
                return;

            cellIndex--;
            return render();

        case 'ArrowRight':
            if (cellIndex === 3)
                return;

            cellIndex++;
            return render();

        case '1':
            cellIndex = 0;
            return render();

        case '2':
            cellIndex = 1;
            return render();

        case '3':
            cellIndex = 2;
            return render();

        case '4':
            cellIndex = 3;
            return render();

        case 'Enter':
            return guess();

        default:
            return mark(event.key);
    }
});

window.onload = () => {
    // get doc vars
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');

    // set ctx styles
    ctx.lineWidth = 3;
    ctx.font = '20px Montserrat';
    ctx.fillStyle = '#ffffff';
    ctx.textAlign = "center";

    // draw labels
    Object.entries(keyMap).forEach(([key, val], index) => {
        drawRoundRect(
            L_PAD_L,
            C_PAD_T + index * C_DIM_GAP,
            C_DIM,
            C_DIM_R,
            colorMap[index],
            false,
            true,
        );

        ctx.fillText(key.toUpperCase(), LT_PAD_L, LT_PAD_T + index * C_DIM_GAP);
    });

    // initialise game
    init();
}
