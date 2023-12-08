#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <chrono>
#include <sstream>
using namespace std;

typedef vector<vector<int>> Matrix;

// Definició d'una estructura per representar un punt en el laberint
struct Point {
    int x, y;
    int cost;
    int heuristic;

    // Constructors
    Point() : cost(0), heuristic(0) {}

    Point(int x, int y) : x(x), y(y), cost(0), heuristic(0) {}

    // Funció per obtenir el cost total (acumulat)
    int getTotalCost() const {
        return cost + heuristic;
    }

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

// Estructura per comparar punts basant-se en el seu cost total
struct ComparePoints {
    bool operator()(const Point& a, const Point& b) const {
        return a.getTotalCost() > b.getTotalCost();
    }
};

const vector<pair<int,int>> DIR = {{-1,0}, {1,0}, {0,-1}, {0,1}};
const int WALL = 1;
int rows, cols;

// Funció per imprimir un missatge d'ús incorrectes
void usage (int error_code)
{
    if (error_code == 0) {
        cerr << "Incorrect number of arguments. Format must follow: Rows Columns StartPoint EndPoint IntegerMatrix(with its elements separated with spaces)" << endl;
        exit(-1);
    } 
}

// Funció per calcular l'heurística entre dos punts
int calculateHeuristic(const Point& current, const Point& end_point) {
    return abs(current.x - end_point.x) + abs(current.y - end_point.y);
}

// Funció per verificar si una posició és vàlida dins el laberint
bool pos_ok (Point& pos)
{
    return pos.x >= 0 and pos.y >= 0 and pos.x < rows and pos.y < cols;
}

// Funció A* per resoldre el laberint
bool aStar(const Matrix& maze, const Point& start_point, const Point& end_point)
{
    Matrix cost(rows, vector<int>(cols, -1));
    priority_queue<Point, vector<Point>, ComparePoints> pq;
    pq.push(start_point);
    while (not pq.empty()) {
        Point current = pq.top();
        pq.pop();
        if (current == end_point) {
            return true;
        }
        for (int i = 0; i < 4; ++i) {
            Point next = current + DIR[i];
            if (pos_ok(next) and maze[next.x][next.y] != WALL) {
                next.cost = current.cost + 1;
                if (next.cost < cost[next.x][next.y] or cost[next.x][next.y] == -1) {
                    cost[next.x][next.y] = next.cost;
                    next.heuristic = calculateHeuristic(next, end_point);
                    pq.push(next);
                }
            }
        }
    }
    return false;
}

// Funció per convertir una cadena de caràcters en una matriu
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

    start_point.heuristic = calculateHeuristic(start_point, end_point);

    string mazeString = argv[7];
    Matrix maze = parseMaze(mazeString, rows, cols);

    auto start_time = std::chrono::high_resolution_clock::now();
    bool maze_solved = aStar(maze, start_point, end_point);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    if (not maze_solved) {
        cerr << "The given maze has no solution" << endl; 
        exit(1);
    }
    cout << "Total time: " << duration.count() << endl;
}
