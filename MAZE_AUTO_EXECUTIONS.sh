#!/bin/bash

time_measures="time_measures.csv"

echo -e "Número de mostra \t Tamany del laberint \t Punt d'inici \t Punt final \t Distància de Manhattan \t Avg. paths/cell \t Num. cells \t Temps DFS(micros) \t Temps A*(micros)" > "$time_measures"

for i in {1..120}
do 
    maze_generation=$(./maze_generator)

    maze=$(echo "$maze_generation" | grep -oP '^[0123]+')

    rows=$(echo "$maze_generation" | grep -oP 'Number of rows: \K\d+')
    cols=$(echo "$maze_generation" | grep -oP 'Number of columns: \K\d+')
    start_point=$(echo "$maze_generation" | grep -oP 'Start point: \(\K[^)]+')
    end_point=$(echo "$maze_generation" | grep -oP 'End point: \(\K[^)]+')
    
    start_point_x=$(echo "$start_point" | cut -d',' -f1)
    start_point_y=$(echo "$start_point" | cut -d',' -f2)
    end_point_x=$(echo "$end_point" | cut -d',' -f1)
    end_point_y=$(echo "$end_point" | cut -d',' -f2)

    d_manhattan=$(echo "$maze_generation" | grep -oP 'Manhattan distance: \K\d+')
    avg_paths_cell=$(echo "$maze_generation" | grep -oP 'Average paths/cell: \K\d+(\.\d+)?')
    n_cells=$(echo "$maze_generation" | grep -oP 'Number of cells: \K\d+')
    
    time_DFS=$(./solve_maze_DFS "$rows" "$cols" "$start_point_x" "$start_point_y" "$end_point_x" "$end_point_y" "$maze" | grep -oP 'Total time: \K\d+')
    time_A_star=$(./solve_maze_A_star "$rows" "$cols" "$start_point_x" "$start_point_y" "$end_point_x" "$end_point_y" "$maze" | grep -oP 'Total time: \K\d+')

    echo -e "$i\t$rows X $cols\t($start_point_x,$start_point_y)\t($end_point_x,$end_point_y)\t$d_manhattan\t$avg_paths_cell\t$n_cells\t$time_DFS\t$time_A_star" >> "$time_measures"    
    sleep 0.001
done 