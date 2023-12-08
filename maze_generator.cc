#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

// Màxim tamany que pot tenir el laberint a generar
#define MAX_ROWS 300 // Màxim nombre de files
#define MAX_COLUMNS 300 // Màxim nombre de columnes

typedef vector<vector<int>> Matrix;

// Definició d'una estructura per representar un punt en el laberint
struct Point {      
    int x;
    int y;

    Point operator+(const pair<int,int>& other) const {
        Point res;
        res.x = x + other.first;
        res.y = y + other.second;
        return res;
    }
};

// Funció per obtenir un camí aleatori dins del laberint per arribar de l'inici al final
vector<Point> getPath(const Matrix &maze, const Point &start_point, const Point &end_point) {

    int rows = maze.size();
    int columns = maze[0].size();

    vector<Point> path;
    Point current = start_point;
    pair<int,int> dir;
    while (current.x != end_point.x || current.y != end_point.y) {
        vector<pair<int,int>> possible_dir; 
        if (current.x-1 >= 0) possible_dir.push_back(pair<int,int>(-1,0));
        if (current.x+1 < rows) possible_dir.push_back(pair<int,int>(1,0));
        if (current.y-1 >= 0) possible_dir.push_back(pair<int,int>(0,-1));
        if (current.y+1 < columns) possible_dir.push_back(pair<int,int>(0,1));
        
        dir = possible_dir[rand()%possible_dir.size()];
        current.x += dir.first;
        current.y += dir.second;
        path.push_back(current);
    }

    return path;

}

// Funció per generar els laberints aleatoris
Matrix generateMaze(int rows, int cols, Point &start_point, Point &end_point) {
    // Inicialitzar un laberint amb tots els punts amb valor 0.
    Matrix maze(rows, vector<int>(cols, 0));

    // Generar el punt d'inici i final aleatoriament
    start_point.x = rand() % rows;
    start_point.y = rand() % cols;
    end_point.x = rand() % rows;
    end_point.y = rand() % cols;

    // Assegurar-se de que el punt d'inici i final no són els mateixos
    while (start_point.x == end_point.x && start_point.y == end_point.y) {
        end_point.x = rand() % rows;
        end_point.y = rand() % cols;
    }

    // Assignar en el laberint els punts d'inici i final
    maze[start_point.x][start_point.y] = 2; // 2 representa el punt d'inici
    maze[end_point.x][end_point.y] = 3;   // 3 representa el punt final

    // Obtenir un camí aleatori dins del laberint per arribar de l'inici al final
    vector<Point> path = getPath(maze, start_point, end_point);

    // Col·locar murs dins el laberint de forma aleatoria
    for (int i = 0; i < rows * cols / 4; ++i) {
        Point wall;
        wall.x = rand() % rows;
        wall.y = rand() % cols;

        int n = path.size();
        // Assegurar que el mur no es col·loca ni a l'inici ni final ni en el camí generat anteriorment                                        
        for (int j = 0; j < n; ++j) {
            while ((path[j].x == wall.x && path[j].y == wall.y) || (wall.x == start_point.x && wall.y == start_point.y) || (wall.x == end_point.x && wall.y == end_point.y)) {
                wall.x = rand() % rows;
                wall.y = rand() % cols;
            }
        }

        maze[wall.x][wall.y] = 1; // 1 representa un mur
    }

    return maze;
}

// Funció per imprimir el laberint i les coordenades dels punts d'inici i final
void printMaze(const Matrix &maze, const Point &start_point, const Point &end_point) {
    int rows = maze.size();
    int columns = maze[0].size();
    int n_paths = 0;
    int n = 0;
    for (int i = 0; i < rows; ++i) {
        cout << maze[i][0];
        for (int j = 1; j < columns; ++j) {
            cout << " " << maze[i][j];
            if (maze[i][j] == 0)
            {
                ++n;
                if (i > 0 and maze[i-1][j] != 1) ++n_paths;
                if (i < rows-1 and maze[i+1][j] != 1) ++n_paths;
                if (j > 0 and maze[i][j-1] != 1) ++n_paths;
                if (i < columns-1 and maze[i][j+1] != 1) ++n_paths;
            }
        }
        cout << endl;
    }
    double avg_paths_cell = static_cast<double>(n_paths)/n;
    cout << "Start point: (" << start_point.x << "," << start_point.y << ")\t";
    cout << "End point: (" << end_point.x << "," << end_point.y << ")\t";
    cout << "Manhattan distance: " << abs(start_point.x - end_point.x) + abs(start_point.y - end_point.y) << endl;
    cout << "Average paths/cell: " << avg_paths_cell << endl;
    cout << "Number of cells: " << n << endl;
}

int main() {

    auto currentTime = std::chrono::system_clock::now();
    auto duration = currentTime.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    srand(static_cast<unsigned>(millis));
    int rows = rand()%MAX_ROWS + 1;                          
    int cols = rand()%MAX_COLUMNS + 1;

    // Generar i imprimir el laberint i les dimensions d'aquest.
    Point start_point, end_point;
    Matrix maze = generateMaze(rows, cols, start_point, end_point);
    printMaze(maze, start_point, end_point);
    cout << "Number of rows: " << rows << "\tNumber of columns: " << cols << endl;

    return 0;
}
