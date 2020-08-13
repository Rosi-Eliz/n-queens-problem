//
//  main.cpp
//  n-queens-problem
//
//  Created by Rosi-Eliz Dzhurkova on 08.08.20.
//  Copyright © 2020 Rosi-Eliz Dzhurkova. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <algorithm>
#include <random>

using namespace std;

struct Queen;
struct Location
{
    int row;
    int column;
    Queen* queen;
    Location(int row, int column, Queen* queen = nullptr) : row(row), column(column), queen(queen) {};
    bool operator==(const Location& location)
    {
        return row == location.row && column == location.column;
    }
};

typedef pair<int, int> Pair;
typedef tuple<int, int, int> Triplet;

const vector<Pair> translationCoefficients = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1,-1}, {0,-1}, {1, -1}};

struct Queen
{
    Location* currentLocation;
    Queen(Location* location) : currentLocation(location){};
};

Location* findPosition(vector<Location*> board, int row, int col)
{
    for(Location* loc : board)
    {
        if(loc->row == row && loc->column == col)
            return loc;
    }
    return nullptr;
    
}

vector<Location*> generateQueenMoves(Location* fromLocation, int boardSize, vector<Location*> board)
{
    vector<Location*> availableMoves;
    for(int i{0}; i < boardSize; i++)
    {
        //y-Axis
        if(i != fromLocation->row)
        {
            Location* location = findPosition(board, i, fromLocation->column);
            availableMoves.push_back(location);
        }
        
        //x-Axis
        if(i != fromLocation->column)
        {
            Location* location = findPosition(board, fromLocation->row, i);
            availableMoves.push_back(location);
        }
    }
    //Diagonals
    int row = fromLocation->row;
    int column = fromLocation->column;
    int count{0};
    while(row < boardSize || column < boardSize)
    {
        count++;
        row++;
        column++;
        
        Location* diagonal = findPosition(board, row, column);
        if(diagonal != nullptr)
            availableMoves.push_back(diagonal);
        int location270reflectionRow = row + (count * -2) ;
        diagonal = findPosition(board, location270reflectionRow, column);
        if(location270reflectionRow >= 0 && diagonal != nullptr)
            availableMoves.push_back(diagonal);
    }
    row = fromLocation->row;
    column = fromLocation->column;
    count = 0;
    while(row < boardSize || column >= 0)
    {
        count++;
        row++;
        column--;
        Location* diagonal = findPosition(board, row, column);
        if(diagonal != nullptr)
            availableMoves.push_back(diagonal);
        int location270reflectionRow = row  + (-2 * count);
        diagonal = findPosition(board, location270reflectionRow, column);
        if(location270reflectionRow >= 0 && diagonal != nullptr)
            availableMoves.push_back(diagonal);
    }
    return availableMoves;
}

bool locationIsValidFor(int row, int column, int boardSize)
{
    return row < boardSize && row >= 0 && column < boardSize && column >= 0;
}

bool hasConflictFor(Location *location, vector<Queen*>& queens, vector<Location *>& board) {
    int row = location->row;
    int column = location->column;
    if(queens.empty())
        return false;
    
    queue<Triplet> adjacentLocations;
    for (int i{0}; i < translationCoefficients.size(); i++)
    {
        Pair translationCoefficient = translationCoefficients[i];
        int newRow = row + translationCoefficient.first;
        int newColumn = column + translationCoefficient.second;
        adjacentLocations.push({newRow, newColumn, i});
    }
    
    while (!adjacentLocations.empty()) {
        Triplet location = adjacentLocations.front();
        adjacentLocations.pop();
        
        int row = get<0>(location);
        int column = get<1>(location);
        int scaleCoefficientsIndex = get<2>(location);
        
        if (!locationIsValidFor(row, column, sqrt(board.size()))) {
            continue;
        }
        
        if(findPosition(board, row, column)->queen != nullptr)
        {
            return true;
        }
        else
        {
            Pair scaleCoefficients = translationCoefficients[scaleCoefficientsIndex];
            adjacentLocations.push({row + scaleCoefficients.first, column + scaleCoefficients.second, scaleCoefficientsIndex});
        }
    }
    
    return false;
};

vector<Location*> availableMoves(vector<Queen*> queens, vector<Location*> board)
{
    if(queens.empty())
        return board;
    
    vector<Location*> availableMoves;
    for(Location* loc : board)
    {
        if(loc->queen == nullptr && !hasConflictFor(loc, queens, board))
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

bool placeQueens(vector<Queen*>& queens, int remainingQueens, vector<Location*>& board, vector<Location*> viableLocations)
{
    if(remainingQueens == 0)
        return true;
    
    shuffle(viableLocations.begin(), viableLocations.end(), random_device());
    for(Location* location : viableLocations)
    {
        if(location->row == 6 && location->column == 5 && queens.size() == 7) {
            cout<<"";
        }
        if(!hasConflictFor(location, queens, board))
        {
            
            Queen* newQueen = new Queen(location);
            location->queen = newQueen;
            queens.push_back(newQueen);
            newQueen->currentLocation = location;
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
                cout<<"[ ]";
            else cout<<"[Q]";
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

