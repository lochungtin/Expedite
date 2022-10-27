// ===== CONST =====
// representation
const UNKNOWN = 0;

// color mapping
const fillMap = {
	0: '#ffffff',
	1: '#dfbd69',
};

const strokeMap = {
	0: '#000000',
	1: '#444444',
};

// cell dimensions
const C_PAD_T = 150;
const C_PAD_L = 100;

const C_DIM = 40;
const C_DIM_R = 5;
const C_DIM_GAP = 45;

// text dimensions
const T_PAD_T = 176.5;
const T_PAD_L = 120;

// ===== DOC =====
let canvas;
let ctx;

// ===== DATA =====
let puzzle = '...465......2..7..9....76..6....234..15...2.9.4...8........6..17.1...9.3..9...5..';
let grid;
let hints;
let possibles;

// ===== not so hidden =====
const FETCH_OPS = {
	method: 'GET',
	headers: {
		'X-RapidAPI-Key': '92e002b8a6msh1f08704cbc0faccp142010jsn36efc03d5bd5',
		'X-RapidAPI-Host': 'sudoku-generator1.p.rapidapi.com',
	},
};

// ===== FUNC =====
// initialise game
const init = () => {
	// fetch('https://sudoku-generator1.p.rapidapi.com/sudoku/generate?seed=1337', FETCH_OPS)
	// 	.then((res) => res.json())
	// 	.then((res) => {
	// 		puzzle = res.puzzle;
	// 		console.log(puzzle);
	// 		render();
	// 	})
	// 	.catch((err) => console.error(err));

	// initialise arrays
	grid = new Array(9);
	hints = new Array(9);
	possibles = new Array(9);
	for (let i = 0; i < 9; ++i) {
		grid[i] = new Array(9);
		hints[i] = new Array(9);
		possibles[i] = new Array(9);
		for (let j = 0; j < 9; ++j) possibles[i][j] = new Array(9).fill(false);
	}

	puzzle = puzzle.replace(/\./g, UNKNOWN.toString());

	// load puzzle into grid
	for (let i = 0; i < 9; ++i)
		for (let j = 0; j < 9; ++j) {
			let val = parseInt(puzzle[i * 9 + j]);
			grid[i][j] = val;
			hints[i][j] = val !== UNKNOWN;
		}

	render();
};

// handle click event
const click = (relX, relY, btn) => {
	console.log(relX, relY, btn);
	render();
};

// render canvas
const render = () => {
	ctx.clearRect(0, 0, canvas.width, canvas.height);

	for (let i = 0; i < 9; ++i)
		for (let j = 0; j < 9; ++j) {
			let highlightShift = (Math.floor(i / 3) % 2 === Math.floor(j / 3) % 2) * 1;
			let data = grid[i][j];

			drawRoundRect(
				C_PAD_L + j * C_DIM_GAP,
				C_PAD_T + i * C_DIM_GAP,
				undefined,
				strokeMap[highlightShift],
			);

			if (data !== UNKNOWN) {
				ctx.fillStyle = fillMap[hints[i][j] * 1];
				ctx.fillText(data, T_PAD_L + j * C_DIM_GAP, T_PAD_T + i * C_DIM_GAP);
			}
		}
};

// ===== AUX =====
// ===== DRAW =====
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
};

// ===== BACKGROUND =====
window.onload = () => {
	// get doc vars
	canvas = document.getElementById('canvas');
	ctx = canvas.getContext('2d');

	canvas.addEventListener('click', (event) => {
		event.preventDefault();

		let rect = canvas.getBoundingClientRect();
		click(
			Math.floor((event.clientX - rect.left - C_PAD_L) / C_DIM_GAP),
			Math.floor((event.clientY - rect.top - C_PAD_T) / C_DIM_GAP),
			0,
		);
	});

	canvas.addEventListener('contextmenu', (event) => {
		event.preventDefault();

		let rect = canvas.getBoundingClientRect();
		click(
			Math.floor((event.clientX - rect.left - C_PAD_L) / C_DIM_GAP),
			Math.floor((event.clientY - rect.top - C_PAD_T) / C_DIM_GAP),
			1,
		);
	});

	// set ctx styles
	ctx.lineWidth = 2;

	ctx.font = '20px Montserrat';
	ctx.textAlign = 'center';

	// initialise game
	init();
};
