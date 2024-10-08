#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <optional>

using namespace std;

/*
*  @brief   Define the Grid type as a 9x9 matrix of Int
*  @details using Grid -> Alias para el tipo de matriz que necesito
*  @details optional<int> similar a maybe en haskell
*  
*  @param vector<vector<optional<int>>>
*/
using Grid = vector<vector<optional<int>>>;

/*
*  @brief Example Sudoku grid with some initial values
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
*  @brief Pretty-print the Sudoku grid
*/
void printGrid(vector<vector <optional<int>> > grid){
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
*  @brief Set a value at a specific row and column in the Grid
*
*  @return A new Grid
*/
Grid setValue(Grid grid, int row, int col, optional<int> value){
   grid[row][col] = value;
   return grid;
}

/*
*  @brief Check if a value is valid in the row
*  @details Funcion find busca el valor dado, sino lo encuentra devueve grid.end()
*
*  @return valid     -> true;
*  @return not valid -> false;
*/
bool validInRow (Grid grid, int row, int value){
   return find(grid[row].begin(), grid[row].end(),value) == grid[row].end();
}

/*
*  @brief Check if a value is valid in the col
*  @details Funcion none_of() recorre cada fila y en el valor de la columna compara con value.
*           Retorna true si ningun elemento cumple la funcion lambda y false si al menos un
*           elemento lo cumple
*
*  @return valid     -> true;
*  @return not valid -> false;
*/
bool validInCol(Grid grid, int col, int value){
   return none_of(grid.begin(), grid.end(), [col, value](const vector<optional<int>> row) { return row[col] == value; });
}

/*
*  @brief Check if a value is valid in the subGrid (3x3)
*  @details 
*
*  @return valid     -> true;
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

int main() {

   cout << "Initial Sudoku grid: \n\n";
   printGrid(grid);
   cout << "\nSolving...\n\n";

   cout << validInSubgrid(grid,1,0,3);
   return 0;
}