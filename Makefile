Test: src/test.c++
	@make --no-print-directory compile
	mkdir -p build
	g++ src/test.c++ -o build/test -lSDL3 -lSDL3_image
	cd build && echo && ./test

compile:
	@echo "-- Compiling file --"

clean:
	@echo "-- Removing build directory --"
	rm -rf build/