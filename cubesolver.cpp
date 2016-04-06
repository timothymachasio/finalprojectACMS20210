// ACMS20210 Final Project: Rubik's Cube Solver
// Team Members: Timothy Machasio, Nick Courtney and Colin Humble
// Instructor: Daniel Brake


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
//function turns the right face of the cube clockwise
void R(void);
//function turns the left face of the cube clockwise
void L(void);
//function turns the upper face of the cube clockwise
void U(void);
//function turns the lower face of the cube clockwise
void D(void);
//function turns the front face of the cube clockwise
void F(void);
//function turns the back face of the cube clockwise
void B(void);
//these functions turn the cube anticlockwise
void Ri(void);
void Li(void);
void Ui(void);
void Di(void);
void Fi(void);
void Bi(void);
//these functions turn the associated face of the cube twice
void R2(void);
void L2(void);
void U2(void);
void D2(void);
void F2(void);
void B2(void);
//function converts a one-digit number into a function corresponding to a move (R,L,U,D,F,B etc)
void numbertomove(int x);
//function prints the current state of the cube
void printcube(void);
//function takes in number representing move and prints out the corresponding move
void printmove(int x);
//function performs a set of moves defined in the void algorithm() function
void algorithm(void);
//function checks whether cube is solved
bool IsSolved(void);

//contains values for a solved cube
int solvedmatrix[6][9] = {{0,0,0,0,0,0,0,0,0},{1,1,1,1,1,1,1,1,1},{2,2,2,2,2,2,2,2,2},{3,3,3,3,3,3,3,3,3},{4,4,4,4,4,4,4,4,4},{5,5,5,5,5,5,5,5,5}};

//contains values for an unsolved cube (what the user inputs)
int unsolvedmatrix[6][9] = {{2,0,2,3,0,1,2,0,2},{5,5,4,5,1,4,5,4,4},{0,2,0,1,2,3,0,2,0},{5,5,4,5,3,4,5,4,4},{1,2,3,1,4,3,1,0,3},{3,2,1,3,5,1,3,0,1}};

//holds new values for the unsolved matrix as moves are performed on the cube.
int workingmatrix[6][9];

//keeps the values of the user-input cube intact, since the R(), U(), D() etc. functions distort the value of 'unsolvedmatrix' above
int originalmatrix[6][9] ={{2,0,2,3,0,1,2,0,2},{5,5,4,5,1,4,5,4,4},{0,2,0,1,2,3,0,2,0},{5,5,4,5,3,4,5,4,4},{1,2,3,1,4,3,1,0,3},{3,2,1,3,5,1,3,0,1}};

int main() {
    
    vector<int> solution;//keeps correctly ordered sequence of moves required to solve the cube
    
    ifstream fin("moves.dat");
    
    //these variables will hold the original values the user inputs
    
    /*
     //This piece of code will generate a file with numbers representing possible moves of length 6 or less (test case)
     
     ofstream fout("moves.dat");
     
     for (int i=1; i<7; i++) {
     for (int j=1; j<7; j++) {
     for (int k=1; k<7; k++) {
     for (int l=1; l<7; l++) {
     for (int m=1; m<7; m++) {
     for (int n=1; n<7; n++ ) {
     cout<<i<<j<<k<<l<<m<<n<<endl;
     fout<<i<<j<<k<<l<<m<<n<<endl;
     
     }
     }
     }
     }
     }
     }
     
     */
    
    //THIS IS A FUNCTIONAL PIECE OF CODE THAT RECEIVES INPUT FROM THE USER
    
    /*
    for (int i=0; i<6; i++) {
        
        int facevalues;
        cout<<"Please enter face "<<i<<" of the Rubik's cube: "<<endl;
        cout<<"Please note that "<<endl;
        cout<<"0 - Face with green center"<<endl;
        cout<<"1 - Face with red center"<<endl;
        cout<<"2 - Face with blue center"<<endl;
        cout<<"3 - Face with orange center"<<endl;
        cout<<"4 - Face with yellow center"<<endl;
        cout<<"5 - Face with white center"<<endl;
        cin>>facevalues;
        
        //we will need to create a function which validates the length the integer to make sure it's 9. No more, no less.
        
        //converts series of numbers into discrete separate numbers in array
        
        for (int j=8; j>=0; j--) {
            unsolvedmatrix[i][j] = facevalues % 10 ;
            originalmatrix[i][j] = facevalues % 10 ;
            facevalues /= 10;
        }
    }
    
    
    cout<<"State of cube before algorithm: "<<endl;
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            cout<<unsolvedmatrix[i][j];
        }
        cout<<endl;
    }
    
     */
    
    cout<<"The state of the cube is as follows:"<<endl;
    cout<<endl;
    printcube();
    cout<<endl;
    cout<<"The cube is ";
    if (!IsSolved()) {
        cout<<"not";
    }
    cout<<" solved."<<endl;
    
    
    //'pushes' each line in the text file (which represents a combination of moves) in an attempt to solve the cube
    int moves;//variable stores the set of moves 'pushed' to it by the input stream
    int movesbuffer;//keeps the value of the original set of moves ('moves') above is manipulated by the 'for' loop below
    
    //NOTE: THIS IS INTENDED AS A TEST AND WILL ONLY WORK A RUBIK'S CUBE SCRAMBLED USING THE MOVES R',U',L',D',F' or B' (scrambling algorithm must be six moves long).
    //IT IS A BASIC DEPTH-FIRST SEARCH. THE SEARCH TREE HAS A DEPTH OF 6, AND A BRANCHING FACTOR OF 6
    //THE FINAL IMPLEMENTATION OF OUR PROJECT WILL ATTEMPT TO UTILIZE AN ITERATIVE DEEPENING DEPTH-FIRST SEARCH
    
    for (int i=0;i<46656;i++) {
        fin>>moves;//stores the current set of moves to be executed
        
        movesbuffer=moves;//captures a copy of the original value of 'moves' since the value of 'moves' will change as the 'for' loop below runs.
        for (int i=0; i<6; i++) {
            //NOTE::THIS WILL RUN THE MOVES IN REVERSE (from the last digit to the first digit)
            numbertomove(moves % 10);
            moves /= 10;
        }
        
        cout<<endl;
        
        if (IsSolved()) {
            cout<<"The cube has been solved! See for yourself: "<<endl;
            printcube();//print the cube then exit
            cout<<"The moves used to solve the cube were "<<endl;
            
            //loop stores discrete moves in vector and reverses order in which moves were executed (to match the order of the input sequence of numbers)
            for (int i=0;i<6;i++) {
                solution.push_back(movesbuffer % 10);
                movesbuffer /= 10;
            }
            
            //loops prints out each move in the correct order of implementation (from vector)
            for (int i=0;i<6;i++) {
                printmove(solution[i]);
                cout<<endl;
            }
            break;
            
            
        } else {
            
            
            cout<<"The cube has not yet been solved"<<endl;
            cout<<"Cube before reassignment"<<endl;
            printcube();
            for (int i=0;i<6;i++) {
                for (int j=0;j<9;j++) {
                    unsolvedmatrix[i][j]=originalmatrix[i][j];//reassign values of originalmatrix to unsolvedmatrix before trying again
                }
            }
            cout<<"Cube after reassignment"<<endl;
            printcube();
        }
        
    }
    
    
    //TEST: How many times the algorithm will run before the cube is solved
    /*
     
     int reps;
     
     while (!IsSolved()) {
     algorithm();
     reps++;
     }
     cout<<"The cube will be solved after the algorithm is repeated "<<reps+1<<" times."<<endl;
     
     */
    
    //TEST: Turning sequence of numbers into moves for implementation;
    
    /*
     
     int x[4]={1,2,3,4};
     
     for (int i=0;i<4;i++) {
     numbertomove(x[i]);
     cout<<"The state of the cube after move "<<i<<" is "<<endl;
     for (int i=0;i<6;i++) {
     for (int j=0;j<9;j++) {
     cout<<unsolvedmatrix[i][j];
     }
     cout<<endl;
     }
     cout<<"The cube is ";
     if (!IsSolved()) {
     cout<<"not";
     }
     cout<<" solved."<<endl;
     
     }
     
     */
    
    return 0;
}



//function corresponding to rotating left layer of cube (white facing left) clockwise
void R() {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[3][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[3][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[3][8];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[0][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[0][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[0][8];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[1][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[1][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[1][8];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[2][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[2][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[2][8];
    workingmatrix[4][0]=unsolvedmatrix[4][6];
    workingmatrix[4][1]=unsolvedmatrix[4][3];
    workingmatrix[4][2]=unsolvedmatrix[4][0];
    workingmatrix[4][3]=unsolvedmatrix[4][7];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][1];
    workingmatrix[4][6]=unsolvedmatrix[4][8];
    workingmatrix[4][7]=unsolvedmatrix[4][5];
    workingmatrix[4][8]=unsolvedmatrix[4][2];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[5][6];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[5][8];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

//function corresponding to rotating left layer of cube (yellow facing right) clockwise
void L() {
    workingmatrix[0][0]=unsolvedmatrix[1][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[1][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[1][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[2][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[2][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[2][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[3][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[3][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[3][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[0][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[0][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[0][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[3][8];
    workingmatrix[4][0]=unsolvedmatrix[4][0];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[4][2];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[5][6];
    workingmatrix[5][1]=unsolvedmatrix[5][3];
    workingmatrix[5][2]=unsolvedmatrix[5][0];
    workingmatrix[5][3]=unsolvedmatrix[5][7];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][1];
    workingmatrix[5][6]=unsolvedmatrix[5][8];
    workingmatrix[5][7]=unsolvedmatrix[5][5];
    workingmatrix[5][8]=unsolvedmatrix[5][2];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};
//function corresponding to rotating upper layer of cube (blue facing up) clockwise
void U() {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[4][0];
    workingmatrix[1][1]=unsolvedmatrix[4][1];
    workingmatrix[1][2]=unsolvedmatrix[4][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[2][6];
    workingmatrix[2][1]=unsolvedmatrix[2][3];
    workingmatrix[2][2]=unsolvedmatrix[2][0];
    workingmatrix[2][3]=unsolvedmatrix[2][7];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][1];
    workingmatrix[2][6]=unsolvedmatrix[2][8];
    workingmatrix[2][7]=unsolvedmatrix[2][5];
    workingmatrix[2][8]=unsolvedmatrix[2][2];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[5][2];
    workingmatrix[3][7]=unsolvedmatrix[5][1];
    workingmatrix[3][8]=unsolvedmatrix[5][0];
    workingmatrix[4][0]=unsolvedmatrix[3][8];
    workingmatrix[4][1]=unsolvedmatrix[3][7];
    workingmatrix[4][2]=unsolvedmatrix[3][6];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[1][0];
    workingmatrix[5][1]=unsolvedmatrix[1][1];
    workingmatrix[5][2]=unsolvedmatrix[1][2];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[5][6];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[5][8];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

//function corresponding to rotating lower layer of cube (with green facing down) clockwise
void D() {
    workingmatrix[0][0]=unsolvedmatrix[0][6];
    workingmatrix[0][1]=unsolvedmatrix[0][3];
    workingmatrix[0][2]=unsolvedmatrix[0][0];
    workingmatrix[0][3]=unsolvedmatrix[0][7];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][1];
    workingmatrix[0][6]=unsolvedmatrix[0][8];
    workingmatrix[0][7]=unsolvedmatrix[0][5];
    workingmatrix[0][8]=unsolvedmatrix[0][2];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[5][6];
    workingmatrix[1][7]=unsolvedmatrix[5][7];
    workingmatrix[1][8]=unsolvedmatrix[5][8];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[4][8];
    workingmatrix[3][1]=unsolvedmatrix[4][7];
    workingmatrix[3][2]=unsolvedmatrix[4][6];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[3][8];
    workingmatrix[4][0]=unsolvedmatrix[4][0];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[4][2];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[1][6];
    workingmatrix[4][7]=unsolvedmatrix[1][7];
    workingmatrix[4][8]=unsolvedmatrix[1][8];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[3][2];
    workingmatrix[5][7]=unsolvedmatrix[3][1];
    workingmatrix[5][8]=unsolvedmatrix[3][0];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

//function corresponding to rotating front of cube (red) clockwise
void F() {
    workingmatrix[0][0]=unsolvedmatrix[4][6];
    workingmatrix[0][1]=unsolvedmatrix[4][3];
    workingmatrix[0][2]=unsolvedmatrix[4][0];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[1][6];
    workingmatrix[1][1]=unsolvedmatrix[1][3];
    workingmatrix[1][2]=unsolvedmatrix[1][0];
    workingmatrix[1][3]=unsolvedmatrix[1][7];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][1];
    workingmatrix[1][6]=unsolvedmatrix[1][8];
    workingmatrix[1][7]=unsolvedmatrix[1][5];
    workingmatrix[1][8]=unsolvedmatrix[1][2];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[5][8];
    workingmatrix[2][7]=unsolvedmatrix[5][5];
    workingmatrix[2][8]=unsolvedmatrix[5][2];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[3][8];
    workingmatrix[4][0]=unsolvedmatrix[2][6];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[4][2];
    workingmatrix[4][3]=unsolvedmatrix[2][7];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[2][8];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[0][0];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[0][1];
    workingmatrix[5][6]=unsolvedmatrix[5][6];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[0][2];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

//function corresponding to rotating back of cube (orange) clockwise
void B() {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[5][0];
    workingmatrix[0][7]=unsolvedmatrix[5][3];
    workingmatrix[0][8]=unsolvedmatrix[5][6];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[4][2];
    workingmatrix[2][1]=unsolvedmatrix[4][5];
    workingmatrix[2][2]=unsolvedmatrix[4][8];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[3][6];
    workingmatrix[3][1]=unsolvedmatrix[3][3];
    workingmatrix[3][2]=unsolvedmatrix[3][0];
    workingmatrix[3][3]=unsolvedmatrix[3][7];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][1];
    workingmatrix[3][6]=unsolvedmatrix[3][8];
    workingmatrix[3][7]=unsolvedmatrix[3][5];
    workingmatrix[3][8]=unsolvedmatrix[3][2];
    workingmatrix[4][0]=unsolvedmatrix[4][0];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[0][8];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[0][7];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[0][6];
    workingmatrix[5][0]=unsolvedmatrix[2][2];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[2][1];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[2][0];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[5][8];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

//these functions turn the cube anticlockwise
void Ri(void) {
    R();
    R();
    R();
};
void Li(void) {
    L();
    L();
    L();
};
void Ui(void) {
    U();
    U();
    U();
};
void Di(void) {
    D();
    D();
    D();
};
void Fi(void) {
    F();
    F();
    F();
};
void Bi(void) {
    B();
    B();
    B();
};
//these functions turn the associated face of the cube twice
void R2(void) {
    R();
    R();
};
void L2(void) {
    L();
    L();
};
void U2(void) {
    U();
    U();
};
void D2(void) {
    D();
    D();
};
void F2(void) {
    F();
    F();
};
void B2(void) {
    B();
    B();
};

bool IsSolved() {
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            if (unsolvedmatrix[i][j]!=solvedmatrix[i][j]) {
                return false;
            }
        }
    }
    return true;
};

//this is the algorithm that was used to scramble the cube into the state the test program solves
void algorithm() {
    
    Bi();
    Fi();
    Di();
    Ui();
    Ri();
    Li();
};

//function to convert series of moves (in number form) to functions
void numbertomove(int x) {
    
    switch (x) {
        case 1: {
            L();
            break;
        } case 2: {
            R();
            break;
        } case 3: {
            U();
            break;
        } case 4: {
            D();
            break;
        } case 5: {
            F();
            break;
        } case 6: {
            B();
            break;
        } default: {
            cout<<"This should never print because all numbers used will be between 1 and 6"<<endl;
        }
    }
    
};

void printmove(int x) {
    
    switch(x) {
        case 1: {
            cout<<" L "<<endl;
            break;
        } case 2 : {
            cout<<" R "<<endl;
            break;
        } case 3 : {
            cout<<" U "<<endl;
            break;
        } case 4 : {
            cout<<" D "<<endl;
            break;
        } case 5 : {
            cout<<" F "<<endl;
            break;
        } case 6 : {
            cout<<" B "<<endl;
            break;
        } default : {
            cout<<"This should never print because all numbers used will be betweeen 1 and 6"<<endl;
        }
    }
}

void printcube() {
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            cout<<unsolvedmatrix[i][j];
        }
        cout<<endl;
    }
};

