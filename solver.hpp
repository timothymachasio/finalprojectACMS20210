//This is the hpp for the Solver class, which creates multiple Cube objects
//and performs every combination of moves at an increasing depth until a solution is found

#ifndef SOLVER_H //inclusion guards to protect against multiple inclusions of the libraries
#define SOLVER_H

#include "cube2.hpp"

class Solver{
	public:
		Solver(void)
        {};

        void Make(void);
		void solve_cube(void);

        //function takes in number representing move and prints out the corresponding move
        void printmove(char const& x);

	private:
		Cube original_cube;
};

void Solver::Make(){
	//Set up orignal cube
	original_cube.Create();
};

void Solver::solve_cube(){
	std::time_t start = std::time(NULL);//to keep track of how long the program runs for
    std::vector<Cube*> states;
    states.push_back(&original_cube);
    std::vector<Cube*> next_states;
    int depth = 0;
    while(true){
        std::cout<<"Testing depth "<<depth<<std::endl;
        for(int i=0;i<states.size();i++){ //Check if any cubes in current branch are solved
            if(states[i] -> IsSolved()){
                if(depth = 0){
                    std::cout<<"The cube is already solved. There's no work for me here."<<std::endl;
                }
                else{
                std::cout<<"Solved! Steps for solving: "<<std::endl;
                std::vector<char> path = states[i] -> Getpath();
                for(int i=0; i<path.size(); i++){
                    printmove(path[i]);
                    std::cout<<std::endl;
                }
                std::cout<<std::endl;
                std::cout<<"The search took "<<std::difftime(std::time(NULL),start)<<" seconds."<<std::endl;//print out how long it took the program to find solution to given cube state
                std::cout<<std::endl;
                std::cout<<"To solve the cube using the generated moves, hold the cube such that the green center is facing down and the red center is facing you."<<std::endl;
                std::exit(0);//from cstdlib. Pretty cool, right?
                return;
                }
            }
        }

        depth++;
        next_states.clear();
        std::cout<<"Generating cubes at depth "<<depth<<std::endl;
        for(int i=0;i<states.size();i++){ //if no cubes in current state are solved, loop through current depth
            for(int j='a';j<'s';j++){ //for each cube at current depth, generate all possible moves
                states[i] -> numbertomove(j);
                Cube* ptr = new Cube(states[i]);
                next_states.push_back(ptr);
                ptr -> Addtopath(j);
                states[i] -> undo_move(j);
           }
        }

        if(depth!=1){
            std::cout<<"Deleting states at depth "<<depth<<std::endl;
            for(int i=0; i<states.size(); i++){
                delete states[i];
            }
        }

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
