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
	2: '#ffffff',
	3: '#dfbd69',
	4: '#26de81',
};

// cell dimensions
const C_PAD_T = 150;
const C_PAD_L = 100;

const C_DIM = 40;
const C_DIM_R = 5;
const C_DIM_GAP = 45;

const M_DIM = 5;
const M_DIM_R = 2;
const M_DIM_GAP = 10;
const M_DIM_REL_PAD = 8;

const S_PAD_T = 600;
const S_PAD_TE = 626.5;

// text dimensions
const T_PAD_T = 176.5;
const T_PAD_L = 120;

// ===== DOC =====
let canvas;
let ctx;

// ===== DATA =====
let puzzle = '...465......2..7..9....76..6....234..15...2.9.4...8........6..17.1...9.3..9...5..';
let selected = 1;
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
	if (relX < 0 || relX > 8 || relY < 0 || relY > 8) return;
	if (hints[relY % 9][relX % 9]) return;

	console.log(relX, relY, btn);

	if (btn) {
		if (possibles[relY][relX][selected - 1])
			possibles[relY][relX][selected - 1] = false;
		else {
			possibles[relY][relX][selected - 1] = true;
			grid[relY][relX] = UNKNOWN;
		}
	}
	else {
		if (grid[relY][relX] === selected)
			grid[relY][relX] = UNKNOWN;
		else {
			grid[relY][relX] = selected;
			
			let sRowOffset = Math.floor(relY / 3) * 3;
			let sColOffset = Math.floor(relX / 3) * 3;
			
			for (let i = 0; i < 9; ++i) {
				possibles[relY][relX][i] = false;
				let sRow = sRowOffset + Math.floor(i / 3);
				let sCol = sColOffset + (i % 3);
	
				if (i !== relX)
					possibles[relY][i][selected - 1] = false;
				if (i !== relY)
					possibles[i][relX][selected - 1] = false;
				if (i !== sRow && i !== sCol)
					possibles[sRow][sCol][selected - 1] = false;
			}
		}
	}

	render();
};

// check if puzzle is complete
const isComplete = () => {
	for (let i = 0; i < 9; ++i) {
		let rCount = new Array(9).fill(0);
		let cCount = new Array(9).fill(0);
		let sCount = new Array(9).fill(0);

		let sRowOffset = Math.floor(i / 3) * 3;
		let sColOffset = i % 3 * 3;

		for (let j = 0; j < 9; ++j) {
			let sRow = sRowOffset + (j / 3);
			let sCol = sColOffset + (j % 3);

			rCount[grid[i][j] - 1]++;
			if (rCount[grid[i][j] - 1] == 2)
				return false;

			cCount[grid[j][i] - 1]++;
			if (cCount[grid[j][i] - 1] == 2)
				return false;

			sCount[grid[sRow][sCol] - 1]++;
			if (sCount[grid[sRow][sCol] - 1] == 2)
				return false;
		}
	}

	return true;
}

// render canvas
const render = (complete = false) => {
	ctx.clearRect(0, 0, canvas.width, canvas.height);

	for (let i = 0; i < 9; ++i) {
		let cellX = C_PAD_L + i * C_DIM_GAP;
		for (let j = 0; j < 9; ++j) {
			let highlightShift = (Math.floor(j / 3) % 2 === Math.floor(i / 3) % 2) * 1;
			let data = grid[j][i];
			if (data === selected)
				highlightShift = 2;
			if (complete)
				highlightShift = 4;

			let cellY = C_PAD_T + j * C_DIM_GAP;
			drawRoundRect(cellX, cellY, C_DIM_R, C_DIM, strokeMap[highlightShift]);

			// display cell value
			if (data !== UNKNOWN) {
				ctx.fillStyle = fillMap[hints[j][i] * 1];
				ctx.fillText(data, T_PAD_L + i * C_DIM_GAP, T_PAD_T + j * C_DIM_GAP);
			}

			// display possibles
			for (let k = 0; k < 3; ++k)
				for (let h = 0; h < 3; ++h)
					if (possibles[j][i][k * 3 + h])
						drawRoundRect(
							cellX + M_DIM_REL_PAD + h * M_DIM_GAP,
							cellY + M_DIM_REL_PAD + k * M_DIM_GAP,
							M_DIM_R,
							M_DIM,
							undefined,
							fillMap[1],
						);
		}

		drawRoundRect(cellX, S_PAD_T, C_DIM_R, C_DIM, strokeMap[(selected - 1 === i) * 3]);
		ctx.fillStyle = fillMap[0];
		ctx.fillText(i + 1, T_PAD_L + i * C_DIM_GAP, S_PAD_TE);
	}
};

// ===== AUX =====
const drawRoundRect = (x, y, radius, size, stroke, fill = false) => {
	ctx.beginPath();

	ctx.moveTo(x + radius, y);
	ctx.lineTo(x + size - radius, y);
	ctx.quadraticCurveTo(x + size, y, x + size, y + radius);
	ctx.lineTo(x + size, y + size - radius);
	ctx.quadraticCurveTo(x + size, y + size, x + size - radius, y + size);
	ctx.lineTo(x + radius, y + size);
	ctx.quadraticCurveTo(x, y + size, x, y + size - radius);
	ctx.lineTo(x, y + radius);
	ctx.quadraticCurveTo(x, y, x + radius, y);

	if (stroke) {
		ctx.strokeStyle = stroke;
		ctx.stroke();
	}

	if (fill) {
		ctx.fillStyle = fill;
		ctx.fill();
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

	window.addEventListener('keypress', (event) => {
		let key = parseInt(event.key)
		if (!isNaN(key) && key > 0) {
			selected = key;
			render();
		}
	})

	// set ctx styles
	ctx.lineWidth = 2;

	ctx.font = '20px Montserrat';
	ctx.textAlign = 'center';

	// initialise game
	init();
};
