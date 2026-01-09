build:
	gcc -Wall -std=c99 ./src/*.c \
		-I/opt/homebrew/opt/sdl2/include \
		-I/opt/homebrew/opt/sdl2/include/SDL2 \
		-I/opt/homebrew/opt/sdl2_image/include \
		-L/opt/homebrew/opt/sdl2/lib \
		-L/opt/homebrew/opt/sdl2_image/lib \
		-lSDL2 -lSDL2_image \
		-o game
run:
	./game
clean:
	rm game


