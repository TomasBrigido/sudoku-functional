module Main where

import Data.Matrix (Matrix, fromLists, toList, getElem, setElem, nrows, ncols, mapPos, toLists)
import Data.Maybe (catMaybes)
import Data.List (elemIndex)

-- Define the Grid type as a 9x9 matrix of Maybe Int
type Grid = Matrix (Maybe Int)

-- Set a value at a specific row and column in the Grid
setValue :: Grid -> Int -> Int -> Maybe Int -> Grid
setValue grid row col value = setElem value (row, col) grid

-- Check if a value can be placed in a given position
-- Valor valido en la fila && columna && subgrilla
isValid :: Grid -> Int -> Int -> Int -> Bool
isValid grid row col n =
    validInRow grid row n && validInCol grid col n && validInSubgrid grid row col n

-- Check if a value is valid in the row
-- El valor no sea un elemnto de la fila (notElem)
-- catMaybes -> devuelve una lista de a 
validInRow :: Grid -> Int -> Int -> Bool
validInRow grid row n = n `notElem` catMaybes [getElem row c grid | c <- [1..ncols grid]]

-- Check if a value is valid in the column
-- Valor valido en la columna, similar al validInRow
validInCol :: Grid -> Int -> Int -> Bool
validInCol grid col n = n `notElem` catMaybes [getElem r col grid | r <- [1..nrows grid]]

-- Check if a value is valid in the 3x3 subgrid
-- catMaybas de la lista de maybes devulve unicamente los just
validInSubgrid :: Grid -> Int -> Int -> Int -> Bool
validInSubgrid grid row col n =
    let subgridRowStart = (row - 1) `div` 3 * 3 + 1
        subgridColStart = (col - 1) `div` 3 * 3 + 1
        subgrid = [getElem r c grid |   r <- [subgridRowStart..subgridRowStart+2],
                                        c <- [subgridColStart..subgridColStart+2]]
    in n `notElem` catMaybes subgrid

-- Try values in a given position and solve the Sudoku
tryValues :: Grid -> Int -> Int -> [Int] -> Maybe Grid
tryValues grid row col [] = Nothing
tryValues grid row col (n:ns) =
    if isValid grid row col n
    then case solveSudoku (setValue grid row col (Just n)) of
                Just solution -> Just solution
                Nothing -> tryValues grid row col ns
    else tryValues grid row col ns

-- Solve the Sudoku puzzle
solveSudoku :: Grid -> Maybe Grid
solveSudoku grid =
    case findEmptyCell grid of
        Nothing -> Just grid -- No empty cells, solution found
        Just (row, col) -> tryValues grid row col [1..9]

-- Find an empty cell (i.e., a cell with Nothing)
findEmptyCell :: Grid -> Maybe (Int, Int)
findEmptyCell grid = 
    let cells = [((r, c), getElem r c grid) | r <- [1..nrows grid], c <- [1..ncols grid]]
    in fmap fst $ foldr (\((r, c), v) acc -> if v == Nothing then Just ((r, c), v) else acc) Nothing cells

-- Example Sudoku grid with some initial values
initialGrid :: Grid
initialGrid = fromLists
    [ [Just 5, Nothing, Just 1, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing]
    , [Nothing, Nothing, Nothing, Nothing, Just 7, Nothing, Nothing, Nothing, Nothing]
    , [Just 6, Nothing, Nothing, Nothing, Nothing, Nothing, Just 3, Nothing, Just 8]
    , [Nothing, Just 6, Nothing, Nothing, Just 1, Just 9, Nothing, Nothing, Nothing]
    , [Nothing, Nothing, Just 9, Just 8, Just 5, Nothing, Nothing, Just 6, Nothing]
    , [Nothing, Nothing, Nothing, Just 3, Nothing, Nothing, Just 4, Nothing, Nothing]
    , [Nothing, Nothing, Just 8, Just 5, Nothing, Just 6, Nothing, Nothing, Nothing]
    , [Just 4, Nothing, Nothing, Nothing, Nothing, Just 3, Nothing, Nothing, Just 1]
    , [Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Just 5, Nothing]
    ]

-- Pretty-print the Sudoku grid
printGrid :: Grid -> IO ()
printGrid grid = mapM_ (putStrLn . formatRow) (toLists grid )
    where
    formatRow = unwords . map formatCell
    formatCell Nothing  = "."
    formatCell (Just n) = show n

-- Main function to run the solver and print the result
main :: IO ()
main = do
    putStrLn "Initial Sudoku grid:"
    printGrid initialGrid
    putStrLn "\nSolving..."
    case solveSudoku initialGrid of
        Nothing -> putStrLn "No solution found."
        Just solution -> do
            putStrLn "\nSolved Sudoku grid:"
            printGrid solution
