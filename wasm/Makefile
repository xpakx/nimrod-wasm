OUTPUT_DIR ?= .

all: $(OUTPUT_DIR)/web.wasm $(OUTPUT_DIR)/web.wat

$(OUTPUT_DIR)/web.wasm: web.c
	clang --target=wasm32-unknown-wasi -O3 -flto --sysroot /opt/wasi-libc -nodefaultlibs -lc -nostartfiles -Wl,--import-memory -Wl,--no-entry -Wl,--export=init -Wl,--export=tick -Wl,--export=keyboard_action -Wl,--export=click -Wl,--lto-O3 -o $(OUTPUT_DIR)/web.wasm web.c

$(OUTPUT_DIR)/web.wat: $(OUTPUT_DIR)/web.wasm
	wasm2wat $(OUTPUT_DIR)/web.wasm > $(OUTPUT_DIR)/web.wat
