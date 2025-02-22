OUTPUT_DIR ?= .
WASI ?= /opt/wasi-libc

comma := ,
FUNCNAMES := init tick keyboard_action click updateSize sendImage malloc
FUNC := $(patsubst %,-Wl$(comma)--export=%,$(FUNCNAMES))

CLANG_ARGS := --target=wasm32-unknown-wasi -O3 -flto --sysroot $(WASI) -nodefaultlibs -lc -nostartfiles -Wl,--import-memory -Wl,--no-entry $(FUNC) -Wl,--lto-O3 

all: $(OUTPUT_DIR)/web.wasm $(OUTPUT_DIR)/web.wat

$(OUTPUT_DIR)/web.wasm: web.c
	clang $(CLANG_ARGS) -o $(OUTPUT_DIR)/web.wasm web.c

$(OUTPUT_DIR)/web.wat: $(OUTPUT_DIR)/web.wasm
	wasm2wat $(OUTPUT_DIR)/web.wasm > $(OUTPUT_DIR)/web.wat
