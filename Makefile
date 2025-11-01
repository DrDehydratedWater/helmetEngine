Test: src/examples/collisionTest.cpp
	@make --no-print-directory compile
	mkdir -p build
	g++ src/examples/collisionTest.cpp -o build/collisionTest -lSDL3 -lSDL3_image
	cd build && echo && ./collisionTest

compile:
	@echo "-- Compiling file --"

clean:
	@echo "-- Removing build directory --"
	rm -rf build/