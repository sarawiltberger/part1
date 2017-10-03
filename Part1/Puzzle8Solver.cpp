#include "Puzzle8Solver.h"
#include "Puzzle8State.h"
#include "Puzzle8StateManager.h"
#include "Puzzle8Solver.h"
#include "Puzzle8PQ.h"

using namespace std;

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions) {
	cost = 0;
	expansions = 0;
    
    Puzzle8PQ openList;	// Initialize the priority queue.
    Puzzle8StateManager sm;	// gives me ID (index in a vector) and if generated
    //put the ids into the openList with their f value
    vector<shared_ptr<Puzzle8State>> idsToStates;
    Puzzle8StateManager closedList;
    vector<shared_ptr<Puzzle8State>> closedIdsToStates;
    Puzzle8StateManager openSet;
    //start with initial state and get id
    //make state from string
    shared_ptr<Puzzle8State> initialState(new Puzzle8State(puzzle));
    //check if goal
    if(initialState->isGoal()){
        cost = 0;
        expansions = 0;
        return;
    }
    int initialID = sm.GenerateState(*initialState);
    initialState->setG(0);
    idsToStates.push_back(initialState);
    int initialf = (double)initialState->getManhattanDistance() * w;
    openList.push(PQElement(initialID, initialf));
    while (!openList.empty()) {	// While the priority queue is not empty
        PQElement next = openList.top(); // The element with the minimum f-val.

        openList.pop();	// Remove the top element from the priority queue.
        //check if goal
        if(idsToStates[next.id]->isGoal()){
            cost = findCost(idsToStates[next.id]);
            return;
        }

        closedList.GenerateState(*idsToStates[next.id]);
        closedIdsToStates.push_back(idsToStates[next.id]);
        //TODO: check the open list?
        //get successors
        vector<string> successors = getBranches(idsToStates[next.id]->GetLinearizedForm());
        expansions ++;
        //for each child
        for(int i =0 ; i < successors.size(); i++){
            shared_ptr<Puzzle8State> childPuzzle(new Puzzle8State(successors[i]));
            //if it is not in the closed list
            if(!closedList.IsGenerated(*childPuzzle) ){
                //check not in open list
                if(!openSet.IsGenerated(*childPuzzle)){
                    childPuzzle->setParent(idsToStates[next.id]);
                    childPuzzle->setG(idsToStates[next.id]->getG() + 1);
                    int md = childPuzzle->getManhattanDistance();
                    int f = (double)childPuzzle->getG() + ((double)md * w);
                    int childID = sm.GenerateState(*childPuzzle);
                    idsToStates.push_back(childPuzzle);
                    //to check later if we generated it
                    openSet.GenerateState(*childPuzzle);
                    //add to priority queue
                    openList.push(PQElement(childID, f));
                }
                //when I impliment this, sometimes get through entire PQ without finding goal
                //break down of what was expanded and checking to make sure it is in closed list
                else{ //if it is in the open list,
                    //if the old child has a larger g, reinput with a new g
                    //this has a different id
                    int oldID = sm.GetStateId(*childPuzzle);
                    if(idsToStates[oldID]->getG() > idsToStates[next.id]->getG() +1){
                        childPuzzle->setParent(idsToStates[next.id]);
                        childPuzzle->setG(idsToStates[next.id]->getG() + 1);
                        int md = childPuzzle->getManhattanDistance();
                        int f = (double)childPuzzle->getG() + ((double)md * w);
                        int childID = sm.GenerateState(*childPuzzle);
                        //if idsToStates, update to the new childPuzzle with the smaller g
                        idsToStates[oldID] = childPuzzle;
                        openSet.GenerateState(*childPuzzle);
                        openList.push(PQElement(childID, f));
                    }
                }
                
            }
        }

    }
    
		
}

int findCost(shared_ptr<Puzzle8State> goalPuzzle){
    int cost = 0;
    while(goalPuzzle->getParent()!= nullptr){
        goalPuzzle = goalPuzzle->getParent();
        cost++;
    }
    return cost;
}

vector<string> getBranches(string state){
    int index0 = (int)state.find("0");
    pair<int, int> coordinates0 = stringIndexToPair(index0);
    //storing successors
    vector<string> branches;
    
    //change the coordinates and test
    int newr1 = coordinates0.first + 1;
    if(newr1 >=0 && newr1 <=2){
        //want to switch 0 at index0 with whatever is at this coordinate
        //get new index
        int newIndex = pairToStringIndex(make_pair(newr1, coordinates0.second));
        branches.push_back(switch0AndChar(state, index0, newIndex));
    }
    int newr2 = coordinates0.first - 1;
    if(newr2 >=0 && newr2 <=2){
        int newIndex = pairToStringIndex(make_pair(newr2, coordinates0.second));
        branches.push_back(switch0AndChar(state, index0, newIndex));
    }
    int newc1 = coordinates0.second + 1;
    if(newc1 >=0 && newc1 <=2){
        int newIndex = pairToStringIndex(make_pair(coordinates0.first, newc1));
        branches.push_back(switch0AndChar(state, index0, newIndex));
    }
    int newc2 = coordinates0.second - 1;
    if(newc2 >=0 && newc2 <=2){
        int newIndex = pairToStringIndex(make_pair(coordinates0.first, newc2));
        branches.push_back(switch0AndChar(state, index0, newIndex));
    }
    return branches;

}


std::pair<int, int> stringIndexToPair(int stringIndex){
    int c = stringIndex % 3;
    int r = stringIndex / 3;
    return make_pair(r, c);
}
int pairToStringIndex(std::pair<int, int> indexPair){
    return (indexPair.first * 3) + indexPair.second;
}

//given a string and two indexes, switch and return new string
string switch0AndChar(string state, int index1, int index2){
    
    int bigger = max(index1, index2);
    int smaller = min(index1,index2);
    char cSmall = state.at(smaller);
    char cBig = state.at(bigger);
    string newState = state.substr(0, smaller) + cBig + state.substr(smaller+1, bigger-smaller-1)+ cSmall;
    if(bigger<9){
        newState += state.substr(bigger+1, state.length()-bigger-1);
    }
    return newState;
    
}


