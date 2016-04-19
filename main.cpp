// ACMS20210 Final Project: Rubik's Cube Solver
// Team Members: Timothy Machasio, Nick Courtney and Colin Humble
// Instructor: Daniel Brake

//This programs finds the most efficient solution to any given state of the Rubik's cube. It may run for a while, but the solution it finds is guaranteed to have the least number of moves (tradeoffs, anyone? :))
//To see magic, enter a cube in which face 1 is bgbogrbgb, face 2 is wwywrywyy, face 3 is gbgrbogbg, face 4 is wwywoywyy, face 5 is rboryorgo, and face 6 is obrowrogr.

#include "cube.hpp"

int main() {//We would just like to mention that we're so proud of how short the main() function is.
    
    Cube Rubix;//Creating an instance of the Cube class. We know it is spelled RUBIK'S. Thanks, Bjarne!
    
    //THIS PART OF THE CODE RECEIVES INPUT FROM THE USER AND VALIDATES IT
    
    Rubix.Create();//Allows user to input state of the cube, and validates the input before the rest of the code runs.
    
    //2. CHECK WHETHER CUBE IS SOLVED. IF NOT, SOLVE IT AND RETURN SOLUTION TO USER.
    
    if (Rubix.IsSolved()) {
        std::cout<<"The cube is already solved. There's no work for me here."<<std::endl;   
    }
    else {
        Rubix.IDDFS(1);//initiates the iterative deepening depth-first search starting from Depth 1. A heuristic function to estimate the lower bound of the depth at which the IDDFS should start running would very much be in order right about here.
    }
    
    //the IDDFS function returns 0 and automatically ends the program if a solution is found. How, you ask? Scroll all the way to the end of the IDDFS function to find out!
}
