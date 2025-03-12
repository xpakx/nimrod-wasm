.PHONY: clean serve wat wasm js test

all: js wasm

wasm: 
	make -C wasm OUTPUT_DIR=../dist

js: dist/script.js dist/index.html

wat: dist/web.wat

dist/web.wat: dist/web.wasm
	wasm2wat dist/web.wasm > dist/web.wat

dist/script.js: script.ts
	npx tsc

dist/index.html: index.html img/ maps/ styles.css
	cp -r index.html img/ maps/ styles.css dist

clean:
	rm -rf dist/*

serve:
	python3 -m http.server --directory dist 8080

test:
	npx jest
