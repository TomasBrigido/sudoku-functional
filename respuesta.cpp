#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>

using namespace std;

// Definir la cuadrícula como una matriz de optional<int> (para simular Maybe en C++)
using Grid = vector<vector<optional<int>>>;

// Setear un valor en una posición específica de la cuadrícula
Grid setValue(Grid grid, int row, int col, optional<int> value) {
    grid[row][col] = value;
    return grid;
}

// Chequear si un valor es válido en una posición dada
bool isValid(const Grid& grid, int row, int col, int n) {
    return validInRow(grid, row, n) && validInCol(grid, col, n) && validInSubgrid(grid, row, col, n);
}

// Verificar si un valor es válido en una fila
bool validInRow(const Grid& grid, int row, int n) {
    return find_if(grid[row].begin(), grid[row].end(), [n](optional<int> cell) { return cell == n; }) == grid[row].end();
}

// Verificar si un valor es válido en una columna
bool validInCol(const Grid& grid, int col, int n) {
    return none_of(grid.begin(), grid.end(), [col, n](const vector<optional<int>>& row) { return row[col] == n; });
}

// Verificar si un valor es válido en el subcuadro de 3x3
bool validInSubgrid(const Grid& grid, int row, int col, int n) {
    int subgridRowStart = (row / 3) * 3;
    int subgridColStart = (col / 3) * 3;
    for (int r = subgridRowStart; r < subgridRowStart + 3; ++r) {
        for (int c = subgridColStart; c < subgridColStart + 3; ++c) {
            if (grid[r][c] == n) return false;
        }
    }
    return true;
}

// Intentar valores en una posición dada para resolver el Sudoku
optional<Grid> tryValues(Grid grid, int row, int col, vector<int> values) {
    for (int n : values) {
        if (isValid(grid, row, col, n)) {
            auto result = solveSudoku(setValue(grid, row, col, n));
            if (result) return result;
        }
    }
    return nullopt;
}

// Resolver el Sudoku
optional<Grid> solveSudoku(Grid grid) {
    auto emptyCell = findEmptyCell(grid);
    if (!emptyCell) return grid;
    
    int row = emptyCell->first;
    int col = emptyCell->second;
    return tryValues(grid, row, col, {1, 2, 3, 4, 5, 6, 7, 8, 9});
}

// Encontrar una celda vacía (es decir, una celda con nullopt)
optional<pair<int, int>> findEmptyCell(const Grid& grid) {
    for (int r = 0; r < grid.size(); ++r) {
        for (int c = 0; c < grid[r].size(); ++c) {
            if (!grid[r][c]) return make_pair(r, c);
        }
    }
    return nullopt;
}

// Imprimir la cuadrícula de Sudoku utilizando std::for_each
void printGrid(const Grid& grid) {
    for_each(grid.begin(), grid.end(), [](const vector<optional<int>>& row) {
        for_each(row.begin(), row.end(), [](const optional<int>& cell) {
            if (cell) cout << *cell << " ";
            else cout << ". ";
        });
        cout << endl;
    });
}

// Ejemplo de cuadrícula de Sudoku con algunos valores iniciales
Grid initialGrid = {
    {5, nullopt, 1, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt},
    {nullopt, nullopt, nullopt, nullopt, 7, nullopt, nullopt, nullopt, nullopt},
    {6, nullopt, nullopt, nullopt, nullopt, nullopt, 3, nullopt, 8},
    {nullopt, 6, nullopt, nullopt, 1, 9, nullopt, nullopt, nullopt},
    {nullopt, nullopt, 9, 8, 5, nullopt, nullopt, 6, nullopt},
    {nullopt, nullopt, nullopt, 3, nullopt, nullopt, 4, nullopt, nullopt},
    {nullopt, nullopt, 8, 5, nullopt, 6, nullopt, nullopt, nullopt},
    {4, nullopt, nullopt, nullopt, nullopt, 3, nullopt, nullopt, 1},
    {nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, 5, nullopt}
};

// Función principal para ejecutar el solver y mostrar el resultado
int main() {
    cout << "Initial Sudoku grid:" << endl;
    printGrid(initialGrid);
    cout << "\nSolving..." << endl;
    auto solution = solveSudoku(initialGrid);
    if (solution) {
        cout << "\nSolved Sudoku grid:" << endl;
        printGrid(*solution);
    } else {
        cout << "No solution found." << endl;
    }
    return 0;
}
