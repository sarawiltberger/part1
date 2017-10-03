#ifndef STATE_8_PUZZLE_H
#define STATE_8_PUZZLE_H

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

// Represents an 8-puzzle state as a 3x3 array of chars. Each char can take values in range '0'-'9' (chars, not integers).
// '0' represents the blank tile.
// Provides GetKey to uniquely represent the puzzle as an integer (for hashing).
// You can extend this class to generate successors and get heuristic distance to '012345678', which is always the goal state.

class Puzzle8State {
public:
	Puzzle8State(std::string s = "012345678") {
		assert(s.length() == 9);
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				tiles[r][c] = s[r*3 + c];
	}
    ~Puzzle8State () {}

	// Key generated as an integer for the hash function in Puzzle8StateManager.
	int GetKey() {
		int key = 0;
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				key = key*10 + int(tiles[r][c] - '0');
        
        return key;
	}
    
    bool isGoal(){
        std::string currState = GetLinearizedForm();
        if(currState == "012345678"){
            return true;
        }
        return false;
    }

	// Return the linearized form as a string. (You don't need to use this.)
	std::string GetLinearizedForm () {
		std::string s = "";
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				s += tiles[r][c];
		return s;
	}

	// Print the puzzle in a 3x3 layout. (You don't need to use this.)
	void Print(std::ostream & out = std::cout) {
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				out<<tiles[r][c]<<" ";
			}
			out<<std::endl;
		}
		out<<GetKey()<<std::endl;
	}
    
    //find heuristic to end goal
    int getManhattanDistance(){
        std::unordered_map<char,int> rChars = {
            {'0', 0},
            {'1', 0},
            {'2', 0},
            {'3', 1},
            {'4', 1},
            {'5', 1},
            {'6', 2},
            {'7', 2},
            {'8', 2},
            
        };	// row of each char in solution
        std::unordered_map<int,int> cChars = {
            {'0', 0},
            {'3', 0},
            {'6', 0},
            {'1', 1},
            {'4', 1},
            {'7', 1},
            {'2', 2},
            {'5', 2},
            {'8', 2},
            
        };	// column of each char in solution
        int manhattanSum = 0;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                char currChar = tiles[r][c];
                /*
                if (currChar == '0'){
                    //std::cout << "it equals 0" << std::endl;
                    continue;
                }*/
                int currSum = std::abs(c - cChars[currChar]) + std::abs(r - rChars[currChar]);
                manhattanSum += currSum;
            }
        }
        return manhattanSum;
    }
    void setG(int i){
        g=i;
    }
    int getG(){
        return g;
    }
    void setParent(std::shared_ptr<Puzzle8State> p){
        parent = p;
    }
    std::shared_ptr<Puzzle8State> getParent(){
        if(parent)
            return parent;
        return NULL; 
    }
    

private:

	// tiles[r][c] is the tile (or blank) at row r (0-2) and column c (0-2)
	// 0th row is the top row, and 0th column is the leftmost column.
	char tiles[3][3];
    int g;
    std::shared_ptr<Puzzle8State> parent;

};

#endif
