// ===== CONST =====
const C_PAD_T = 250;
const C_PAD_L = 100;

const C_DIM = 40;
const C_DIM_R = 5;
const C_DIM_GAP = 45;

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

    render();
}

// render canvas
const render = () => {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    for (let i = 0; i < 9; ++i)
        for (let j = 0; j < 9; ++j)
            drawRoundRect(
                C_PAD_L + j * C_DIM_GAP,
                C_PAD_T + i * C_DIM_GAP,
                C_DIM,
                C_DIM_R,
            );
}

// ===== AUX =====
// draw rounded rectangle
const drawRoundRect = (x, y, dim, radius) => {
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

    ctx.stroke();

    ctx.closePath();
}

// ===== BACKGROUND =====
window.onload = () => {
    // get doc vars
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');

    // set ctx stroke styles
    ctx.strokeStyle = '#444444';
    ctx.lineWidth = 2;

    ctx.font = '20px Montserrat';
    ctx.fillStyle = '#ffffff';
    ctx.textAlign = "center";

    // initialise game
    init();
}