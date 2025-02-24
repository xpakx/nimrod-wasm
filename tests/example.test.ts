const fs = require('fs');
const path = require('path');

interface NimrodWasmInputs {
    init: (width: number, height: number) => number;
    updateSize: (width: number, height: number) => number;
    tick: () => void;
    sendImage: (ptr: number, width: number, height: number) => void;
    malloc: (size: number) => number;
    onMouseClick: (button: number, mouseX: number, mouseY: number) => void;
    onMouseMove: (mouseX: number, mouseY: number) => void;
    onMouseUp: (button: number, mouseX: number, mouseY: number) => void;
    onMouseWheel: (deltaY: number) => void;
    onKeyDown: (key: number) => void;
    onKeyUp: (key: number) => void;
}

interface NimrodWasmOutputs {
    jsprintf: (base: any) => void;
    drawCanvas: (ptr: number, length: number) => void;
}

interface Wasi {
    fd_close: () => void;
    fd_seek: () => void;
    fd_write: () => void;
}

async function initializeWasm(): Promise<NimrodWasmInputs> {
	let wasi: Wasi = {
		fd_close: function() { },
		fd_seek: function() { },
		fd_write: function() { }
	};

	let io_wasm: NimrodWasmOutputs = {
		jsprintf: function(_base: any) {},
		drawCanvas: function(_ptr: number, _length: number) {}
	};

	const wasmPath = path.resolve(__dirname, '../dist/web.wasm');
	const memory = new WebAssembly.Memory({ initial: 2 });
	const buffer = fs.readFileSync(wasmPath);
	// @ts-expect-error: wasm 
	const { instance } = await WebAssembly.instantiate(buffer, { 
		env: { memory }, 
		wasi_snapshot_preview1: wasi, 
		io_wasm: io_wasm 
	});
	return instance.exports as unknown as NimrodWasmInputs;
}


describe('Wasm Module Tests', () => {
	let nimrod: NimrodWasmInputs;

	beforeAll(async () => {
		nimrod = await initializeWasm();
	});

	test('Wasm initialized correctly', async () => {
		expect(nimrod.init(10, 10)).toBe(0);
	});
});
