#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <optional>
#include <tuple>

using namespace std;

/*
*  @brief   Define the Grid type as vector of vectors
*  @details Uso Grid -> Alias para el tipo de matriz que necesito.
*           El dato optional<int> es similar a maybe en haskell.
*  
*  @param   vector<vector<optional<int>>>
*/
using Grid = vector<vector<optional<int>>>;
const int numeros[9] = {1,2,3,4,5,6,7,8,9};
optional <Grid> tryValues(const Grid grid, optional<pair<int, int>> coords, const int numeros[9], int num = 0);


/*
*  @brief   Example Sudoku grid with some initial values
*/
const Grid grid = {
      {5,nullopt,1,nullopt,nullopt,nullopt,nullopt,nullopt,nullopt},
      {nullopt,nullopt,nullopt,nullopt,7,nullopt,nullopt,nullopt,nullopt},
      {6,nullopt,nullopt,nullopt,nullopt,nullopt,3,nullopt,8},
      {nullopt,6,nullopt,nullopt,1,9,nullopt,nullopt,nullopt},
      {nullopt,nullopt,9,8,5,nullopt,nullopt,6,nullopt},
      {nullopt,nullopt,nullopt,3,nullopt,nullopt,4,nullopt,nullopt},
      {nullopt,nullopt,8,5,nullopt,6,nullopt,nullopt,nullopt},
      {4,nullopt,nullopt,nullopt,nullopt,3,nullopt,nullopt,1},
      {nullopt,nullopt,nullopt,nullopt,nullopt,nullopt,nullopt,5,nullopt}
   };

/*
*  @brief   Pretty-print the Sudoku grid
*/
void printGrid(Grid grid){
   for_each(grid.begin(), grid.end(),[](vector<optional<int>> line){
         for_each(line.begin(),line.end(),[](optional<int> x){
            if(x == nullopt){
               cout << ". ";
            }else{
               cout << x.value() << " ";
            };
         });
         cout << endl;});
}

/*
*  @brief   Set a value at a specific row and column in the Grid
*
*  @return  A new Grid
*/
Grid setValue(Grid grid, int row, int col, optional<int> value){
   grid[row][col] = value;
   return grid;
}

/*
*  @brief   Check if a value is valid in the row
*  @details Funcion find busca el valor dado. Si no lo encuentra devueve grid.end()
*
*  @return  valid     -> true;
*  @return  not valid -> false;
*/
bool validInRow (Grid grid, int row, int value){
   return find(grid[row].begin(), grid[row].end(),value) == grid[row].end();
}

/*
*  @brief   Check if a value is valid in the col
*  @details Funcion none_of() recorre cada fila y en el valor de la columna compara con value.
*           Retorna true si ningun elemento cumple la funcion lambda y false si al menos un
*           elemento lo cumple
*
*  @return  valid     -> true;
*  @return  not valid -> false;
*/
bool validInCol(Grid grid, int col, int value){
   return none_of(grid.begin(), grid.end(), [col, value](const vector<optional<int>> row) { return row[col] == value; });
}

/*
*  @brief   Check if a value is valid in the subGrid (3x3)
*  @details Primero defino los limites de la subgrid.
*           Luego la recorro con el for_each() guardando los valores en subgrid.
*           Y con el find() controlo si el dato n esta en la subgrid.
*
*  @return  valid -> true;
*/
bool validInSubgrid (Grid grid, int row, int col, int n){

   const int subgridRowStart = ((((row +1) -1)/3)*3);
   const int subgridColStart = ((((col +1) -1)/3)*3);

   const auto lineStart = grid.begin() + subgridRowStart;
   const auto lineFinish = grid.begin() + subgridRowStart + 3;

   
   vector <int>  subgrid;


   for_each(lineStart, lineFinish,[&subgrid,subgridColStart](vector<optional<int>> &line){
   
      const auto colStart = line.begin() + subgridColStart;
      const auto colFinish = line.begin() + subgridColStart + 3;
   
      for_each(colStart,colFinish,[&subgrid](const optional<int> &x){
            if(x == nullopt){
            
         }else{
            subgrid.push_back(x.value());
         };
      });
   });
   
   return find(subgrid.begin(),subgrid.end(),n) == subgrid.end();
}

/*
*  @brief   Check if a value is valid in row col and subgrid
*
*  @return  valid -> true;
*/
bool isValid(Grid grid,int row, int col, int n){

   if(validInRow(grid,row,n) && validInCol(grid,col,n) && validInSubgrid(grid,row,col,n)){
      return true;
   }
   return false;
}

/*
*  @brief   Find the coords of an empy cell
*  @details Utilizo una funcion recursiva para ir recorriendo linea por linea la matriz.
*           Utilizo find_if() para entonctrar la celda que no tiene valor.
*           Devuelvo el par con las cordenadas de la celda vacia.
*
*  @return  Pair of an empty cell coords;
*/
optional<pair<int, int>> findEmptyCell(const Grid grid, int row = 0) {

   //Condicion de corte de la recusividad
   if (row >= grid.size()) {
      return nullopt;
   }

   auto data = find_if(grid[row].begin(), grid[row].end(), [](const optional<int>& cell) {
      return !cell.has_value();
   });

   if (data != grid[row].end()) {
      int col = distance(grid[row].begin(), data);
      return make_pair(row, col);
   }

   return findEmptyCell(grid, row + 1);
}

/*
*  @brief   Find the coords of an empy cell
*  @details Utilizo una funcion recursiva para ir recorriendo linea por linea la matriz.
*           Utilizo find_if() para entonctrar la celda que no tiene valor.
*           Devuelvo el par con las cordenadas de la celda vacia.
*
*  @return  Pair of an empty cell coords;
*/
optional <Grid> solveSudoku (const Grid grid){
   if(findEmptyCell(grid) == nullopt){
      return grid; //Solucion del sudoku
   }else{
      tryValues(grid,findEmptyCell(grid),numeros);
   }
}
/*
*  @brief   Find the coords of an empy cell
*  @details Utilizo una funcion recursiva para ir recorriendo linea por linea la matriz.
*           Utilizo find_if() para entonctrar la celda que no tiene valor.
*           Devuelvo el par con las cordenadas de la celda vacia.
*
*  @return  Pair of an empty cell coords;
*/

optional <Grid> tryValues(const Grid grid, optional<pair<int, int>> coords, const int numeros[9], int num){
   
   if(isValid(grid,coords->first,coords->second,numeros[num])){

      const optional <Grid> solution = solveSudoku(setValue(grid,coords->first,coords->second,numeros[0]));

      if(solution != nullopt){
         return solution;
      }else{
         tryValues(grid,coords,numeros,num+1);
      }
   }else{
      tryValues(grid,coords,numeros,num+1);
   }
   
}

int main() {

   cout << "Initial Sudoku grid: \n\n";
   printGrid(grid);
   cout << "\nSolving...\n\n";

   const optional <Grid> solution = solveSudoku(grid);
   if(solution != nullopt){
      cout<<"Solved Sudoku grid: \n\n";
      printGrid(solution.value());
      cout<<"No solution found. \n\n";

   }
   return 0;
}