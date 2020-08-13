//
//  main.cpp
//  n-queens-problem
//
//  Created by Rosi-Eliz Dzhurkova on 08.08.20.
//  Copyright © 2020 Rosi-Eliz Dzhurkova. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct Queen;
struct Location
{
    int first;
    int second;
    Queen* queen;
    Location(int first, int second,Queen* queen = nullptr) : first(first), second(second), queen(queen) {};
    bool operator==(const Location& location)
    {
        return first == location.first && second == location.second;
    }
};

struct Queen
{
    Location* currentLocation;
    Queen(Location* location) : currentLocation(location){};
};

Location* findPosition(vector<Location*> board, int row, int col)
{
    for(Location* loc : board)
    {
        if(loc->first == row && loc->second == col)
            return loc;
    }
    throw runtime_error("Position not found.");
}

vector<Location*> generateQueenMoves(Location* fromLocation, int boardSize, vector<Location*> board)
{
    vector<Location*> availableMoves;
    for(int i{0}; i < boardSize; i++)
    {
        //y-Axis
        if(i != fromLocation->first)
        {
            Location* location = findPosition(board, i, fromLocation->second);
            availableMoves.push_back(location);
        }
           
        //x-Axis
        if(i != fromLocation->second)
        {
            Location* location = findPosition(board, fromLocation->first, i);
            availableMoves.push_back(location);
        }
    }
    //Diagonals
    int row = fromLocation->first;
    int column = fromLocation->second;
    int count{0};
    while(row < boardSize - 1 && column < boardSize - 1)
    {
        count++;
        row++;
        column++;
        availableMoves.push_back(findPosition(board, row, column));
        int location270reflectionRow = row + (count * -2) ;
        if(location270reflectionRow >= 0)
           availableMoves.push_back(findPosition(board, location270reflectionRow, column));
    }
    row = fromLocation->first;
    column = fromLocation->second;
    count = 0;
    while(row < boardSize - 1 && column > 0)
      {
          count++;
          row++;
          column--;
          availableMoves.push_back(findPosition(board, row, column));
          int location270reflectionRow = row  + (-2 * count);
          if(location270reflectionRow >= 0)
             availableMoves.push_back(findPosition(board, location270reflectionRow, column));
      }
    return availableMoves;
}

bool fieldIsInConflict(vector<Queen*> queens, Location* location, vector<Location*> board)
{
    if(queens.empty())
        return false;
    
    for(Queen* q : queens)
    {
        vector<Location*> currentQueensAvailableMoves = generateQueenMoves(q->currentLocation, sqrt(board.size()), board);
        if(find(currentQueensAvailableMoves.begin(), currentQueensAvailableMoves.end(), location) != currentQueensAvailableMoves.end())
            return true;
    }
    return false;
}

vector<Location*> availableMoves(vector<Queen*> queens, vector<Location*> board)
{
    if(queens.empty())
        return board;
    
    vector<Location*> availableMoves;
    for(Location* loc : board)
    {
        if(loc->queen == nullptr && !fieldIsInConflict(queens, loc, board))
        {
            availableMoves.push_back(loc);
        }
    }
return availableMoves;
}

vector<Location*> initializeBoard(int dimension)
{
    vector<Location*> board;
    for(int row{0}; row < dimension; row++)
       {
           for(int col{0}; col < dimension; col++)
           {
               Location* newLocation = new Location(row, col, nullptr);
               board.push_back(newLocation);
           }
       }
    return board;
}

bool newBoardHasConflict(vector<Queen*> queens, vector<Location*> board)
{
    if(queens.size() == 1)
        return false;
    
    for(Location* loc : board)
    {
        if(fieldIsInConflict(queens, loc, board))
            return true;
    }
    return false;
}

bool placeQueens(vector<Queen*>& queens, int remainingQueens, vector<Location*>& board, vector<Location*> viableLocations)
{
    if(remainingQueens == 0)
        return true;
    for(Location* location : viableLocations)
    {
       
        Queen* newQueen = new Queen(location);
        location->queen = newQueen;
        queens.push_back(newQueen);
        newQueen->currentLocation = location;
        if(!fieldIsInConflict(queens, location, board))
        {
            vector<Location*> newViableLocations = availableMoves(queens, board);
            bool result = placeQueens(queens, remainingQueens - 1, board, newViableLocations);
            

            if (result)
            {
                return true;
            }
           
            else
            {
                queens.pop_back();
                location->queen = nullptr;
                delete newQueen;
            }
        }
        else
        {
            queens.pop_back();
            location->queen = nullptr;
            delete newQueen;
        }
        
      
    }
    return false;
    
}

void printBoard(vector<Location*> board)
{
    for(int row{0}; row < sqrt(board.size()); row++)
       {
           for(int col{0}; col <  sqrt(board.size()); col++)
           {
               if(findPosition(board, row, col)->queen == nullptr)
                   cout<<" _ ";
               else cout<<" Q ";
           }
           cout<<endl;
       }
}

int main(int argc, const char * argv[]) {
    int boardSize{0};
    cout<<"Enter your desired board size: ";
    cin>>boardSize;
//    for(int i{0}; i < 92; i++)
//    {
        vector<Location*> board = initializeBoard(boardSize);
        vector<Queen*> queens;
        int queensToPlace = boardSize;
        placeQueens(queens, queensToPlace, board, availableMoves(queens, board));
        printBoard(board);
        cout<<endl;
//    }
     return 0;
}

