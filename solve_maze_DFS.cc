#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
using namespace std;

typedef vector<vector<int>> Matrix;
typedef vector<vector<bool>> Matrix_Bool;

// Definició d'una estructura per representar un punt en el laberint
struct Point {
    int x, y;

    // Operador de comparació per verificar si dos punts són iguals
    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }

    // Operador d'addició per sumar un parell de coordenades a un punt
    Point operator+(const pair<int,int>& other) const {
        Point res;
        res.x = x + other.first;
        res.y = y + other.second;
        return res;
    }
};

const vector<pair<int,int>> DIR = {{-1,0}, {1,0}, {0,-1}, {0,1}};
const int WALL = 1;
int rows, cols;

// Funció per imprimir un missatge d'ús incorrecte
void usage (int error_code)
{
    if (error_code == 0) {
        cerr << "Incorrect number of arguments. Format must follow: Rows Columns StartPoint EndPoint IntegerMatrix(with its elements separated with spaces)" << endl;
        exit(-1);
    } 
}

// Funció per verificar si una posició és vàlida dins el laberint
bool pos_ok (Point& pos)
{
    return pos.x >= 0 and pos.y >= 0 and pos.x < rows and pos.y < cols;
}

// Funció DFS per resoldre el laberint
void dfs (const Matrix& maze, Matrix_Bool& visited, Point act_point, const Point& end_point, bool& found)
{
    if (act_point == end_point) found = true;
    else if (not found) {
        visited[act_point.x][act_point.y] = true;
        for (const auto& dir : DIR) {
            Point next = act_point + dir;
            if (pos_ok(next) and maze[next.x][next.y] != WALL and not visited[next.x][next.y]) 
                dfs(maze, visited, next, end_point, found);
        }
    }
}

// Funció DFS per resoldre el laberint i dir si s'ha resolt 
bool dfs (const Matrix& maze, const Point& start_point, const Point& end_point)
{
    Matrix_Bool visited(rows, vector<bool>(cols, false));
    bool found = false;
    dfs(maze, visited, start_point, end_point, found);
    return found;
}

// Funció per convertir una cadena de caràcters en una matriu d'enters
Matrix parseMaze(const string& mazeString, int rows, int cols) {
    Matrix maze(rows, std::vector<int>(cols, 0));
    istringstream iss(mazeString);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            iss >> maze[i][j];
        }
    }
    return maze;
}

// Funció main
int main (int argc, char **argv)
{
    Point start_point, end_point;

    if (argc != 8) usage(0);
    rows = stoi(argv[1]);
    cols = stoi(argv[2]);
    start_point.x = stoi(argv[3]);
    start_point.y = stoi(argv[4]);
    end_point.x = stoi(argv[5]);
    end_point.y = stoi(argv[6]);

    string mazeString = argv[7];
    Matrix maze = parseMaze(mazeString, rows, cols);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    bool maze_solved = dfs(maze, start_point, end_point);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    if (not maze_solved) {
        cerr << "The given maze has no solution" << endl; 
        exit(1);
    }
    cout << "Total time: " << duration.count() << endl;
}
