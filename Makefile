build:
	make -j$(shell nproc) -C doomgeneric build

clean:
	make -C doomgeneric clean
