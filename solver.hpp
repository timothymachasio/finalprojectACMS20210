//This is the hpp for the Solver class, which creates multiple Cube objects
//and performs every combination of moves at an increasing depth until a solution is found

#ifndef SOLVER_H //inclusion guards to protect against multiple inclusions of the libraries
#define SOLVER_H

#include "cube.hpp"

class Solver{
	public:
		Solver(void) //Default constructor
        {};

        //Runs the Create function in the Cube class, which provides the user with directions and takes a user input
        void Make(void); 
        //Runs the solving function
        void solve_cube(void);

        //function takes in number representing move and prints out the corresponding move
        void printmove(char const& x);

	private:
        //Creates an instance of the Cube class
		Cube original_cube;
};

//Refernces the Create function from the Cube class, which rnus the user input and validation sequence
void Solver::Make(){
	original_cube.Create();
};

//Runs the Cube solving sequence
void Solver::solve_cube(){
	std::time_t start = std::time(NULL);//to keep track of how long the program runs for
    std::vector<Cube*> states; //Vector of pointers to Cube objects
    states.push_back(&original_cube);
    std::vector<Cube*> next_states; //A buffer vector to store the pointers at each depth after the moves are performed on each Cube
    int depth = 0;

    if(states[0] -> IsSolved()){
        std::cout<<"The cube is already solved. There's no work for me here."<<std::endl;
        return;    
    }

    //THIS IS THE MAIN RECURSIVE SOLVING FUNCTION
    while(true){
        depth++;
        next_states.clear();
        std::cout<<"Testing cubes at depth "<<depth<<std::endl;
        for(int i=0;i<states.size();i++){ //if no cubes in current state are solved, loop through current depth
            for(int j='a';j<'s';j++){ //for each cube at current depth, generate all possible moves
                states[i] -> numbertomove(j); //Each iterations performs the next of the 18 possible moves
                Cube* ptr = new Cube(states[i]); //Creates a pointer to new state of the cube
                next_states.push_back(ptr); //Adds the newly created ptr to the next_staes vector
                ptr -> Addtopath(j);

                if(states[i] -> IsSolved()){
                    states[i] -> Addtopath(j);
                    std::cout<<"Solved! Steps for solving: "<<std::endl;
                    std::vector<char> path = states[i] -> Getpath();
                    for(int i=0; i<path.size(); i++){
                        printmove(path[i]); //Prints the path of moves it took to produce the solved cube
                        std::cout<<std::endl;
                    }
                    std::cout<<std::endl;
                    std::cout<<"The search took "<<std::difftime(std::time(NULL),start)<<" seconds."<<std::endl;//print out how long it took the program to find solution to given cube state
                    std::cout<<std::endl;
                    std::cout<<"To solve the cube using the generated moves, hold the cube such that the green center is facing down and the red center is facing you."<<std::endl;
                    std::exit(0);//from cstdlib. Pretty cool, right?
                    return;
                }
                states[i] -> undo_move(j); //If a solution is not found, the move is undone on states[i] so the next move can be tried
           }
        }
            //Deletes the memory allocated to pointers at each depth after it has been fully tested
            for(int i=0; i<states.size(); i++){
                delete states[i];
            }
        }

        //Copies next_states into states to be used at the next depth
        states.clear();
        states = next_states;

    }
};

//function takes in number representing move and prints out the corresponding move
void Solver::printmove(char const& x) {
    
    switch(x) {
        case 'a': {
            std::cout<<" Left face clockwise "<<std::endl;
            break;
        } case 'b' : {
            std::cout<<" Right face clockwise"<<std::endl;
            break;
        } case 'c' : {
            std::cout<<" Upper face clockwise"<<std::endl;
            break;
        } case 'd' : {
            std::cout<<" Lower face clockwise "<<std::endl;
            break;
        } case 'e' : {
            std::cout<<" Front face clockwise"<<std::endl;
            break;
        } case 'f' : {
            std::cout<<" Back face clockwise "<<std::endl;
            break;
        } case 'g' : {
            std::cout<<" Left face clockwise (twice) "<<std::endl;
            break;
        } case 'h' : {
            std::cout<<" Right face clockwise (twice) "<<std::endl;
            break;
        } case 'i' : {
            std::cout<<" Upper face clockwise (twice) "<<std::endl;
            break;
        } case 'j' : {
            std::cout<<" Lower face clockwise (twice) "<<std::endl;
            break;
        } case 'k' : {
            std::cout<<" Front face clockwise (twice) "<<std::endl;
            break;
        } case 'l' : {
            std::cout<<" Back face clockwise (twice) "<<std::endl;
            break;
        } case 'm': {
            std::cout<<" Left face anticlockwise"<<std::endl;
            break;
        } case 'n': {
            std::cout<<" Right face anticlockwise"<<std::endl;
            break;
        } case 'o': {
            std::cout<<" Upper face anticlockwise"<<std::endl;
            break;
        } case 'p': {
            std::cout<<" Lower face anticlockwise"<<std::endl;
            break;
        } case 'q': {
            std::cout<<" Front face anticlockwise"<<std::endl;
            break;
        } case 'r': {
            std::cout<<" Back face anticlockwise"<<std::endl;
            break;
        }default : {
            std::cout<<"If this prints, there's a problem where the 'printmove' function is called"<<std::endl;
        }
    }
}


#endif
