interface Wasi {
    fd_close: () => void;
    fd_seek: () => void;
    fd_write: () => void;
}

let wasi: Wasi = {
    fd_close: function() { },
    fd_seek: function() { },
    fd_write: function() { }
};

interface NimrodWasmOutputs {
    jsprintf: (base: any) => void;
    drawCanvas: (ptr: number, length: number) => void;
}

interface NimrodWasmInputs {
    init: (width: number, height: number) => number;
    updateSize: (width: number, height: number) => number;
    tick: () => void;
    keyboard_action: (key: number) => void;
    click: (mouseX: number, mouseY: number) => void;
}

let canvas: (HTMLCanvasElement | null) = null;
let context: (CanvasRenderingContext2D | null) = null;
let nimrod: (NimrodWasmInputs | null) = null
const width = 640;
const height = 480;

window.onload = async () => {
	console.debug('Started app');
	canvas = document.getElementById('gameCanvas') as (HTMLCanvasElement | null);
	if (!canvas) {
		console.error('No canvas elem');
		return;
	}
	context = canvas.getContext('2d');
	if (!context) {
		console.error("No context");
		return;
	}
	canvas.width = width;
	canvas.height = height;
	init();
}

async function init() {
	if (!canvas) return;
	const memory = new WebAssembly.Memory({ initial: 2 });

	let io_wasm: NimrodWasmOutputs = {
	    jsprintf: function(base: any) {
		    const view = new Uint8Array(memory.buffer);
		    const text = decode(view, base);
		    const timestamp = new Date().toISOString();
		    const msg = `[${timestamp}] [LOG]: ${text}`;
		    console.log(msg);
	    },
	    drawCanvas: function(ptr: number, length: number) {
		    if (!context) return;
		    if (!canvas) return;
		    const imageData = new Uint8ClampedArray(memory.buffer, ptr, length);
		    const data = new ImageData(imageData, canvas.width, canvas.height);
		    context.putImageData(data, 0, 0);
	    }
	};

	const { instance } = await WebAssembly.instantiateStreaming(
		fetch("./web.wasm"),
		// @ts-expect-error: wasm thingies
		{ env: { memory }, wasi_snapshot_preview1: wasi, io_wasm: io_wasm } 
	);

	nimrod = instance.exports as unknown as NimrodWasmInputs;
	const returnCode = nimrod.init(canvas.width, canvas.height);
	console.log("Return code:", returnCode);
	if(returnCode != 0) {
		return;
	}

	document.addEventListener('keydown', function(event) {
		if (!nimrod) return;
		switch (event.key) {
			case 'ArrowLeft': case 'h':
				nimrod.keyboard_action(0);
			break;
			case 'ArrowRight': case 'l':
				nimrod.keyboard_action(1);
			break;
			case 'ArrowUp': case 'k':
				nimrod.keyboard_action(2);
			break;
			case 'ArrowDown': case 'j':
				nimrod.keyboard_action(3);
			break;
			case '+':
				if (!canvas) return;
				canvas.width = 800;
				canvas.height = 600;
				const returnCode = nimrod.updateSize(800, 600);
				if(returnCode != 0) {
					console.error("UM");
				}
			break;
		}
	});


	canvas.addEventListener("click", (event) => {
		if (!canvas) return;
		if (!nimrod) return;
		const rect = canvas.getBoundingClientRect();

		const mouseX = event.clientX - rect.left;
		const mouseY = event.clientY - rect.top;
		nimrod.click(mouseX, mouseY);
	});

	function render() {
		if (!nimrod) return;
		nimrod.tick();
		window.requestAnimationFrame(render);
	}

	render();
}

function encode(memory: any, base: number, string: string) {
	for (let i = 0; i < string.length; i++) {
		memory[base + i] = string.charCodeAt(i);
	}

	memory[base + string.length] = 0;
};

function decode(memory: any, base: number) {
	let cursor = base;
	let result = '';

	while (memory[cursor] !== 0) {
		result += String.fromCharCode(memory[cursor++]);
	}

	return result;
};
