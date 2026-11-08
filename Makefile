all:
	mkdir -p build
	gcc daemon/*.c -o build/ctld
	gcc cli/*.c -o build/ctl
