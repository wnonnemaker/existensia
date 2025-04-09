default:
	gcc -std=c++20 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main main.cpp
