program:main.cpp
	g++ -o program main.cpp glad.o -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lSDL2
