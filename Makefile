all:
	g++ -o app main.cpp source/graph_logic.cpp -O3 -ffast-math
# 	g++ -o app main.cpp source/graph_logic.cpp -O1 -Wall -Wextra -Wno-unused-result -Wdangling-pointer=1 -Werror=uninitialized -Werror=return-type -Werror=vla
run: all
	./app