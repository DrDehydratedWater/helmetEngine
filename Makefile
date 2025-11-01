Test: src/test.cpp
	@make --no-print-directory compile
	mkdir -p build
	g++ src/test.cpp -o build/test -lSDL3 -lSDL3_image
	cd build && echo && ./test

compile:
	@echo "-- Compiling file --"

clean:
	@echo "-- Removing build directory --"
	rm -rf build/