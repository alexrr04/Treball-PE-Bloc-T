all: maze_generator solve_maze_DFS solve_maze_A_star

maze_generator: maze_generator.cc
	g++ -o maze_generator maze_generator.cc

solve_maze_DFS: solve_maze_DFS.cc
	g++ -o solve_maze_DFS solve_maze_DFS.cc

solve_maze_A_star: solve_maze_A_star.cc
	g++ -o solve_maze_A_star solve_maze_A_star.cc

tar: README maze_generator.cc solve_maze_DFS.cc solve_maze_A_star.cc MAZE_AUTO_EXECUTIONS.sh time_measures.csv Makefile
	tar zcfv maze_solver.gz.tar README maze_generator.cc solve_maze_DFS.cc solve_maze_A_star.cc MAZE_AUTO_EXECUTIONS.sh time_measures.csv Makefile

clean:
	rm -f *.o maze_generator solve_maze_DFS solve_maze_A_star