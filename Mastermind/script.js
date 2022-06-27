// ===== CONST =====
let responseCodeOffset = [0, 1, 3, 6, 10];

let keyMap = {
    q: 0,
    w: 1,
    e: 2,
    r: 3,
    a: 4,
    s: 5,
    d: 6,
    f: 7,
};

let responseMap = {
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
    target = [rand(), rand(), rand(), rand()];
    tCount = count(target);

    rowIndex = 0;
    cellIndex = 0;

    rows = new Array(12);
    for (let i = 0; i < 12; ++i)
        rows[i] = new Array(4).fill(-1);

    response = new Array(12).fill('0000');
}

// mark guess cell
const mark = key => {
    let map = keyMap[key];
    if (map === undefined)
        return;
    rows[rowIndex][cellIndex] = map;
}

// evaluate guess
const guess = () => {
    // calculate response code
    let gCount = count(rows[rowIndex]);

    let colors = 8;
    for (let i = 0; i < 8; ++i)
        colors -= Math.abs(dataCount[i] - gCount[i]);
    colors /= 2;

    let correct = 0;
    for (let i = 0; i < 4; ++i)
        correct += (rows[rowIndex][i] == target[i]);

    // update response
    response[rowIndex] = responseMap[responseCodeOffset[colors] + correct];
}

// ===== AUX =====
// random number generator [0, 7]
const rand = () => {
    return Math.floor(Math.random() * 8);
}

// create count array
const count = arr => {
    let rt = [0, 0, 0, 0, 0, 0, 0, 0];
    arr.forEach(index => rt[index]++);
    return rt;
}

// ===== BACKGROUND =====
// key stroke listener
document.addEventListener('keydown', event => {
    switch (event.key) {
        case 'ArrowLeft':
            if (cellIndex === 0)
                return;
            return cellIndex--;

        case 'ArrowRight':
            if (cellIndex === 3)
                return;
            return cellIndex++;

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

    // initialise game
    init();
}
