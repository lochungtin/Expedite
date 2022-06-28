// ===== CONST =====
const fillMap = {
    9: '#dfbd69',
};

const strokeMap = {
    0: '#000000',
    1: '#dfbd69',
    2: '#dfbd69',
    3: '#dfbd69',
    4: '#dfbd69',
    5: '#dfbd69',
    6: '#dfbd69',
    7: '#dfbd69',
    8: '#dfbd69',
    10: '#ffffff',
    11: '#ffffff',
};

// cell dimensions
const C_PAD_T = 250;
const C_PAD_L = 100;

const C_DIM = 40;
const C_DIM_R = 5;
const C_DIM_GAP = 45;

// graphic dimensions
const G_PAD_T = 261;
const G_PAD_L = 111;

// text dimensions
const T_PAD_T = 276.5;
const T_PAD_L = 120;

// graphic path
const BOMB_PATH = 'M483.403,2.598c-27.733-4.267-56.533-3.2-83.2,4.267c-6.4,1.067-14.933,3.2-21.333,8.533c-8.533,6.4-12.8,13.867-17.067,20.267l-11.733,18.133l-33.067-21.333c-9.6-6.4-22.4-3.2-28.8,6.4l-27.733,42.667c-16-3.2-32-5.333-48-5.333c-71.467,0-137.6,35.2-177.067,94.933c-30.933,48-42.667,104.533-30.933,160s43.733,103.467,90.667,134.4c35.2,23.467,75.733,35.2,117.333,35.2c71.467,0,138.667-35.2,177.067-96c48-73.6,45.867-171.733-6.4-242.133l27.733-42.667c3.2-5.333,8.533-19.2-5.333-28.8l-21.333-13.867l12.8-20.267c2.133-3.2,4.267-7.467,6.4-8.533c2.133-1.067,6.4-2.133,7.467-2.133c21.333-5.333,44.8-6.4,66.133-3.2c10.667,2.133,21.333-6.4,23.467-17.067C502.603,15.398,495.136,4.731,483.403,2.598z M355.403,381.264c-32,48-85.333,76.8-142.933,76.8c-33.067,0-66.133-9.6-93.867-27.733c-38.4-24.533-64-62.933-73.6-107.733c-9.6-44.8-1.067-89.6,24.533-128c32-48,85.333-76.8,142.933-76.8c18.133,0,35.2,3.2,52.267,6.4c8.533,3.2,18.133-1.067,23.467-8.533l24.533-37.333l53.333,35.2l-24.533,37.333c-5.333,8.533-5.333,18.133,1.067,25.6C391.67,234.064,397.003,318.331,355.403,381.264z';
const FLAG_PATH = 'M389.183,10.118c-3.536-2.215-7.963-2.455-11.718-0.634l-50.653,24.559c-35.906,17.409-77.917,16.884-113.377-1.418c-38.094-19.662-83.542-18.72-120.789,2.487V20c0-11.046-8.954-20-20-20s-20,8.954-20,20v407.514c0,11.046,8.954,20,20,20s20-8.954,20-20V220.861c37.246-21.207,82.694-22.148,120.789-2.487c35.46,18.302,77.47,18.827,113.377,1.418l56.059-27.18c7.336-3.557,11.995-10.993,11.995-19.146V20.385C394.866,16.212,392.719,12.333,389.183,10.118z'

// ===== DOC =====
let canvas;
let ctx;

// ===== DATA =====
let grid;

// ===== FUNC =====
// initialise game
const init = () => {
    grid = new Array(9);
    for (let i = 0; i < 9; ++i) {
        grid[i] = new Array(9).fill(0);
    }

    for (let i = 0; i < 9; ++i)
        for (let j = 0; j < 9; ++j)
            grid[i][j] = Math.floor(Math.random() * 12);

    render();
}

// render canvas
const render = () => {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    for (let i = 0; i < 9; ++i)
        for (let j = 0; j < 9; ++j) {
            let data = grid[i][j];

            drawRoundRect(
                C_PAD_L + j * C_DIM_GAP,
                C_PAD_T + i * C_DIM_GAP,
                fillMap[data],
                strokeMap[data],
            );

            if (data > 0 && data < 9) {
                ctx.fillStyle = '#ffffff';
                ctx.fillText(
                    data,
                    T_PAD_L + j * C_DIM_GAP,
                    T_PAD_T + i * C_DIM_GAP
                );
            }

            if (data == 10)
                drawFlag(G_PAD_L + j * C_DIM_GAP, G_PAD_T + i * C_DIM_GAP);

            if (data == 11)
                drawBomb(G_PAD_L + j * C_DIM_GAP, G_PAD_T + i * C_DIM_GAP);
        }
}

// ===== AUX =====
// draw rounded rectangle
const drawRoundRect = (x, y, fill, stroke) => {
    ctx.beginPath();

    ctx.moveTo(x + C_DIM_R, y);
    ctx.lineTo(x + C_DIM - C_DIM_R, y);
    ctx.quadraticCurveTo(x + C_DIM, y, x + C_DIM, y + C_DIM_R);
    ctx.lineTo(x + C_DIM, y + C_DIM - C_DIM_R);
    ctx.quadraticCurveTo(x + C_DIM, y + C_DIM, x + C_DIM - C_DIM_R, y + C_DIM);
    ctx.lineTo(x + C_DIM_R, y + C_DIM);
    ctx.quadraticCurveTo(x, y + C_DIM, x, y + C_DIM - C_DIM_R);
    ctx.lineTo(x, y + C_DIM_R);
    ctx.quadraticCurveTo(x, y, x + C_DIM_R, y);

    if (fill) {
        ctx.fillStyle = fill;
        ctx.fill();
    }

    if (stroke) {
        ctx.strokeStyle = stroke;
        ctx.stroke();
    }

    ctx.closePath();
}

const drawBomb = (x, y) => {
    let p = new Path2D();
    p.addPath(new Path2D(BOMB_PATH), new DOMMatrix().translateSelf(x, y).scaleSelf(0.04, 0.04));

    ctx.fillStyle = '#ffffff';
    ctx.fill(p);
}

const drawFlag = (x, y) => {
    let p = new Path2D();
    p.addPath(new Path2D(FLAG_PATH), new DOMMatrix().translateSelf(x, y).scaleSelf(0.04, 0.04));

    ctx.fillStyle = '#dfbd69';
    ctx.fill(p);
}

// ===== BACKGROUND =====
window.onload = () => {
    // get doc vars
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');

    // set ctx styles
    ctx.lineWidth = 2;

    ctx.font = '20px Montserrat';
    ctx.textAlign = 'center';

    // initialise game
    init();
}