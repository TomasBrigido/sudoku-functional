#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <optional>
#include <tuple>

using namespace std;

/*
*  @brief   Defino el tipo Grid como un vector de vectores de optional int.
*  @details Uso Grid -> Alias para el tipo de matriz que necesito.
*           El dato optional<int> es similar a maybe en haskell.
*  
*  @param   vector<vector<optional<int>>>
*/
using Grid = vector<vector<optional<int>>>;

/*
*  @brief   Defino el arreglo de numeros del 1 al 9.
*  
*  @param   [1.2.3.4.5.6.7.8.9]
*/
const int numeros[9] = {1,2,3,4,5,6,7,8,9};
optional <Grid> tryValues(const Grid grid, pair<int, int> coords, const int numeros[9], int num = 0);


/*
*  @brief   Grilla inicial del sudoku.
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
*  @brief   Imepresion de la grilla.
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
*  @brief   Defino un valor en un lugar dado de la grilla.
*
*  @return  Una nueva grilla con el valor definido.
*/
Grid setValue(Grid grid, int row, int col, optional<int> value){
   grid[row][col] = value;
   return grid;
}

/*
*  @brief   Verifico si el valor dado es valido en la fila.
*  @details La funcion find busca el valor dado. Si no lo encuentra devueve grid.end()
*
*  @return  True o false dependiendo si el valor es valido o no.
*/
bool validInRow (const Grid& grid, int row, int value){
   return find(grid[row].begin(), grid[row].end(),value) == grid[row].end();
}

/*
*  @brief   Verifico si el valor dado es valido en la columna.
*  @details Funcion none_of() recorre cada fila y en el valor de la columna compara con value.
*           Retorna true si ningun elemento cumple la funcion lambda y false si al menos un
*           elemento lo cumple.
*
*  @return  True o false dependiendo si el valor es valido o no.
*/
bool validInCol(const Grid& grid, int col, int value){
   return none_of(grid.begin(), grid.end(), [col, value](const vector<optional<int>> row) {
      return row[col] == value; 
   });
}

/*
*  @brief   Verifico si el valor el valido en la sub grilla 3x3.
*  @details Primero defino los limites de la sub grilla.
*           Luego la recorro con el for_each() guardando los valores en subgrid.
*           Y con el find() controlo si el dato n esta en la subgrid.
*
*  @return  True o false dependiendo si el valor es valido o no.
*/
bool validInSubgrid (Grid& grid, int row, int col, int n){

   const int subgridRowStart = ((((row +1) -1)/3)*3);
   const int subgridColStart = ((((col +1) -1)/3)*3);

   const auto lineStart = grid.begin() + subgridRowStart;
   const auto lineFinish = grid.begin() + subgridRowStart + 3;

   
   vector <optional<int>>  subgrid;


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
*  @brief   Verifico que el valor sea valido en la fila, la columna y la sub grilla
*
*  @return  True o false dependiendo si el valor es valido o no.
*/
bool isValid(Grid& grid,int row, int col, int n){
   return validInRow(grid,row,n) && validInCol(grid,col,n) && validInSubgrid(grid,row,col,n);
}

/*
*  @brief   Encuentra las cordenadas de una celda vacia.
*  @details Utilizo una funcion recursiva para ir recorriendo linea por linea la matriz.
*           Utilizo find_if() para entonctrar la celda que no tiene valor.
*           Devuelvo el par con las cordenadas de la celda vacia.
*
*  @return  Par que indica la cordenanada de la celda vacia.
*/
optional<pair<int, int>> findEmptyCell(const Grid& grid, int row = 0) {

   //Condicion de corte de la recusividad
   if (row >= (int)(grid.size())) {
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
*  @brief   Resuelve la grilla del sudoku.
*  @details Busca celdas vacias de la grilla y prueba valores del 1 al 9.
*           Si no encuentra celdas vacias es porque ya esta resuelta la grilla.
*
*  @return  La grilla con el sudoku resuelto.
*/
optional <Grid> solveSudoku (const Grid& grid){

   auto emptyCell = findEmptyCell(grid);
   if(emptyCell == nullopt){
        return grid; //Solucion del sudoku
   }
    auto [row, col] = *emptyCell;
   return tryValues(grid,make_pair(row, col), numeros);
   
}

/*
*  @brief   Prueba valores en las celdas vacias.
*  @details Valida que el valor sea valido y si es asi, defino ese valor en la celda.
*
*  @return  Grilla con los valores definidos.
*/
optional <Grid> tryValues(Grid grid, pair<int, int> coords, const int numeros[9], int num){

   if (num >= 9) return nullopt;//Condicion para cunado se acaben los numeros.
   
   if(isValid(grid,coords.first,coords.second,numeros[num])){
      Grid newGrid = setValue(grid, coords.first, coords.second, numeros[num]); 
      auto solution = solveSudoku(newGrid);
      if(solution){
            return solution;
      }
   }

   return tryValues(grid, coords, numeros, num+1);
}

int main() {

   cout << "Initial Sudoku grid: \n\n";
   printGrid(grid);
   cout << "\nSolving...\n\n";

   const optional <Grid> solution = solveSudoku(grid);

   if(solution != nullopt){
      cout<<"Solved Sudoku grid: \n\n";
      printGrid(solution.value());
      cout<<"\n";
   }else{
      cout<<"No solution found. \n\n";
   }

   return 0;
}
