OUTPUT_DIR ?= .
WASI ?= /opt/wasi-libc

comma := ,
FUNCNAMES := init tick updateSize sendImage malloc onMouseMove onMouseClick onMouseUp onMouseWheel onKeyDown onKeyUp createBuilding
FUNC := $(patsubst %,-Wl$(comma)--export=%,$(FUNCNAMES))

CLANG_ARGS := --target=wasm32-unknown-wasi -O3 -flto --sysroot $(WASI) -nodefaultlibs -lc -nostartfiles -Wl,--import-memory -Wl,--no-entry $(FUNC) -Wl,--lto-O3 -I./game

all: $(OUTPUT_DIR)/web.wasm

$(OUTPUT_DIR)/web.wasm: web.c game/*.c
	clang $(CLANG_ARGS) -o $(OUTPUT_DIR)/web.wasm web.c game/*.c
