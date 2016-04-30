// ACMS20210 Final Project: Optimal Rubik's Cube Solver
// Team Members: Timothy Machasio, Nick Courtney and Colin Humble
// Instructor: Daniel Brake
//This programs will find the most efficient solution to any given state of the Rubik's cube (if one exists). Beware, it is bound to run for extremely long periods beyond depth ~8. The team will continue working on ways to improve runtime at large depths.
//A suggestion for a cube state you could input (whose optimal solution involves six moves) is one in which face 1 is bgbogrbgb, face 2 is wwywrywyy, face 3 is gbgrbogbg, face 4 is wwywoywyy, face 5 is rboryorgo, and face 6 is obrowrogr.

#include "solver.hpp" //solver class
#include "cube2.hpp"   //cube class  

int main() {//We would just like to mention that we're so proud of how short the main() function is.
    
    Solver Rubix; //Creating an instance of the Solver class. We know it is spelled RUBIK'S.
        
    Rubix.Make();//Self-contained function allows user to input state of the cube, and validates the input before the rest of the code runs.
    
    Rubix.solve_cube();//initiates the iterative deepening depth-first search starting from Depth 1. A heuristic function to estimate the lower bound of the depth at which the IDDFS should start running would very much be in order right about here.

return 0;
}
