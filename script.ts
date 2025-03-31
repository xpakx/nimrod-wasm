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
    sendImage: (ptr: number, width: number, height: number) => number;
    malloc: (size: number) => number;
    onMouseClick: (button: number, mouseX: number, mouseY: number) => void;
    onMouseMove: (mouseX: number, mouseY: number) => void;
    onMouseUp: (button: number, mouseX: number, mouseY: number) => void;
    onMouseWheel: (deltaY: number) => void;
    onKeyDown: (key: number) => void;
    onKeyUp: (key: number) => void;
    createBuilding: (x: number, y: number, ...imgIndices: number[]) => void;
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
		const keyCode = translateKey(event.key);
		if (keyCode) nimrod.onKeyDown(keyCode);
	});


	canvas.addEventListener("click", (event) => {
		if (!canvas) return;
		if (!nimrod) return;
		const rect = canvas.getBoundingClientRect();

		const mouseX = event.clientX - rect.left;
		const mouseY = event.clientY - rect.top;
		const button = event.button;

		nimrod.onMouseClick(button, mouseX, mouseY);
	});

	canvas.addEventListener('mousemove', function(event) {
		if (!nimrod) return;
		if (!canvas) return;
		const rect = canvas.getBoundingClientRect();

		const mouseX = event.clientX - rect.left;
		const mouseY = event.clientY - rect.top;

		nimrod.onMouseMove(mouseX, mouseY);
	});

	canvas.addEventListener('wheel', function(event) {
		if (!nimrod) return;
		nimrod.onMouseWheel(event.deltaY);
	});



	function render() {
		if (!nimrod) return;
		nimrod.tick();
		window.requestAnimationFrame(render);
	}

	const loader = new ImageLoader();
	const indices = await loader.loadBuilding("img/house.png", 2, memory);
	const wellIndices = await loader.loadBuilding("img/well.png", 1, memory);
	nimrod.createBuilding(2, 2, ...indices);
	nimrod.createBuilding(2, 5, ...wellIndices);

	render();
}

class ImageLoader {
	offscreenCanvas: OffscreenCanvas;
	contextOff: OffscreenCanvasRenderingContext2D;

	constructor() {
		this.offscreenCanvas = new OffscreenCanvas(10, 10);
		this.contextOff = this.offscreenCanvas.getContext('2d', { willReadFrequently: true })!;
	}

	// TODO: create a building prototype
	async loadBuilding(filename: string, size: number, memory: any): Promise<number[]> {
		const img = new Image();
		img.src = filename;
		await img.decode();

		let indices = [];
		for (let i = 3; i <= 10; i++) {
			const imgWidth = 64 * size * 0.2 * i;
			const imgHeight = img.height*(imgWidth/img.width);

			this.offscreenCanvas.width = imgWidth;
			this.offscreenCanvas.height = imgHeight;

			this.contextOff.drawImage(img, 0, 0, imgWidth, imgHeight);
			const index = sendImageToNimrod(this.offscreenCanvas, memory);
			indices.push(index);
		}
		return indices;
	}
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

function encodeImage(offscreenCanvas: OffscreenCanvas, memory: any, base: number): number {
	const context = offscreenCanvas.getContext('2d')!;
	const imageData = context.getImageData(0, 0, offscreenCanvas.width, offscreenCanvas.height);
	const pixelData = imageData.data;

	const memoryView = new Uint8Array(memory.buffer);

	const pixelDataLength = pixelData.length;
	if (base + pixelDataLength > memoryView.length) {
		throw new Error('Not enough space in WASM memory to store the image.');
	}

	memoryView.set(pixelData, base);
	return pixelDataLength;
}

function sendImageToNimrod(offscreenCanvas: OffscreenCanvas, memory: any): number {
	if (!nimrod) return -1;
	const pixelDataLength = offscreenCanvas.width * offscreenCanvas.height * 4;
	const base = nimrod.malloc(pixelDataLength);
	encodeImage(offscreenCanvas, memory, base);
	const imgIndex = nimrod.sendImage(base, offscreenCanvas.width, offscreenCanvas.height);
	console.log(`Image ${imgIndex} added`);
	return imgIndex;
}

function translateKey(key: string): undefined | number {
	switch (key) {
		case 'ArrowLeft': case 'h':
			return 0;
		case 'ArrowRight': case 'l':
			return 1;
		case 'ArrowUp': case 'k':
			return 2;
		case 'ArrowDown': case 'j':
			return 3;
		}
	return undefined;
}
