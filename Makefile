all:
	g++ -o app main.cpp source/graph_logic.cpp -O3 -ffast-math
run: all
	./app