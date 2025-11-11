Platformer: src/examples/platformer/platformer.cpp
	@make --no-print-directory compile
	mkdir -p build
	g++ src/examples/platformer/platformer.cpp -o build/platformer -lSDL3 -lSDL3_image -lSDL3_mixer
	cd build && echo && ./platformer

compile:
	@echo "-- Compiling file --"

clean:
	@echo "-- Removing build directory --"
	rm -rf build/