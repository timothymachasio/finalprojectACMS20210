// ACMS20210 Final Project: Rubik's Cube Solver
// Team Members: Timothy Machasio, Nick Courtney and Colin Humble
// Instructor: Daniel Brake

//This programs finds the most efficient solution to any given state of the Rubik's cube. It may run for a while, but the solution it finds is guaranteed to have the least number of moves (tradeoffs, anyone? :))
//To see magic, enter a cube in which face 1 is bgbogrbgb, face 2 is wwywrywyy, face 3 is gbgrbogbg, face 4 is wwywoywyy, face 5 is rboryorgo, and face 6 is obrowrogr.

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>


using namespace std;
//this function directs the program user on how to input the state of the cube
void Directions(int i);
//this function checks whether everything the user inputs is in the allowed alphabetical form
bool AllAllowedCharacters(string s);
//this function converts a single digit (color code) and returns its corresponding color
string Number2Color(int number);
//this function converts each character of the string a user inputs into a digit which is fed into the String2Number function to create an integer equivalent of the user input
int Letter2Digit (char c);
//This function checks to ensure the user is entering the right face of the cube
bool IsCorrectFace (string userinput, int face);
//this function converts the string a user inputs into an integer which the program works with
//int String2Number (string s);


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
char solvedmatrix[6][9] = {{'g','g','g','g','g','g','g','g','g'},{'r','r','r','r','r','r','r','r','r'},{'b','b','b','b','b','b','b','b','b'},{'o','o','o','o','o','o','o','o','o'},{'y','y','y','y','y','y','y','y','y'},{'w','w','w','w','w','w','w','w','w'}};

//keeps the initial values of the user-input cube. Changes as the functions (R, U, D, F etc.) run.
char unsolvedmatrix[6][9];

//holds new values for the unsolved matrix as moves are performed on the cube.
char workingmatrix[6][9];

//keeps the values of the user-input cube intact, since the R(), U(), D() etc. functions distort the value of 'unsolvedmatrix' above.
char originalmatrix[6][9];

int main() {
    
    vector<int> solution;//keeps correctly ordered sequence of moves required to solve the cube
    
    ifstream fin;
    
    //THIS IS A FUNCTIONAL PIECE OF CODE THAT RECEIVES INPUT FROM THE USER AND VALIDATES IT
    
    
    for (int i=0; i<6; i++) {
        
        string userinput;//this string holds the value of user-friendly input (a string)
        cout<<"Please enter the colors on the face of the cube with a "<<Number2Color(i)<<" center: "<<endl;
        cout<<endl;
        Directions(i);//gives user directions on how to enter the state of the cube depending on which face is being asked for. Cool, right?
        //Example (For face with red center-> corresponding to i=1). The function will print:
        //Now hold the cube such that the green center is facing down and the red center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last.
        cout<<endl;
        cout<<"Please note that:"<<endl;
        cout<<endl;
        cout<<"g - Green"<<endl;
        cout<<"r - Red"<<endl;
        cout<<"b - Blue"<<endl;
        cout<<"o - Orange"<<endl;
        cout<<"y - Yellow"<<endl;
        cout<<"w - White"<<endl;
        cout<<endl;
        cout<<"Enter everything in lowercase. No spaces."<<endl;
        cin>>userinput;
        
        
        //step 1 of input validation: ensure that user only enters alphabetic characters
        while (!AllAllowedCharacters(userinput)) {
            cout<<"You entered some characters which are not allowed. You can only enter g, r, b, o, y or w. Please enter the colors again. Don't be creative!"<<endl;
            cin>>userinput;//overwrites invalid values entered
        }
        
        //step 2 of input validation: ensure that user has entered nine colors for each side of the cube
        while (!(userinput.size()==9)) {
            cout<<"What kind of Rubik's cube has a face with "<<userinput.size()<<" cubies? You must enter a series of nine characters, no more no less!"<<endl;
            cin>>userinput;//overwrites invalid values entered
        }
        
        
        //step 3 of input validation: to ensure the user has entered the correct face
        while (!IsCorrectFace(userinput,i)) {
            cout<<"You entered values for the wrong face. Please enter colors for the face with a "<<Number2Color(i)<<" center as per the instructions."<<endl;
            cin>>userinput;//overwrites the invalid values entered
        }
        
        //step 4 of input validation: ask user to confirm their input
        char valid;
        cout<<"You entered: "<<userinput<<endl;
        cout<<"Is this correct? \nIf it isn't, the program may run for a very long time!"<<endl;
        cout<<"Please enter n if not, or y (or anything else) if yes. Note that 'n' must be lowercase"<<endl;
        cin>>valid;
        while (valid=='n') {
            cout<<"Please enter the color values for the face with a "<<Number2Color(i)<<" center cubie again:"<<endl;
            cin>>userinput;//overwrites wrong input
        }
        
        //converts user input into a multidimensional array
        
        for (int j=0; j<9; j++) {
            unsolvedmatrix[i][j] = userinput.at(j);//this value will change as the functions run
            originalmatrix[i][j] = userinput.at(j);//this value will not
        }
    }
    
    
    //checks to see whether cube is solved. If it is, does nothing. If it isn't, starts searching for solution.
    if (IsSolved())
    {
        
        cout<<"The cube is already solved. There's no work for me here."<<endl;
        
    }
    
    else
        
    {//THIS IS THE 'MEAT' OF THE PROGRAM
        fin.open("moves.dat");//THIS IS A TEMPORARY INPUT FILE. THE TEAM IS DISCUSSING A WAY TO IMPLEMENT AN IDDFS right within the program to eliminate the need for this.
        
    cout<<"State of cube before algorithm: "<<endl;
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            cout<<unsolvedmatrix[i][j];
        }
        cout<<endl;
    }
    
    
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
        fin.close();//close input file stream
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
    if (IsSolved()) {
        cout<<"To solve the cube using the generated moves, hold the cube such that the green center is facing down and the red center is facing you at all times."<<endl;
    } else {
        cout<<"A solution was not found. The state of the cube you entered must have been invalid - otherwise a solution would have been found."<<endl;
    }
    
    
    return 0;
}

//This function prints out directions on how the user should input the state of the cube. It works in conjunction with the loop in main() that asks for user input, face by face.
void Directions(int i) {
    switch (i) {
        case 0: {//corresponds to face with green center
            cout<<"Hold the cube such that the orange center is facing down and the green center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<endl;
            break;
        } case 1: {//corresponds to face with red center
            cout<<"Now hold the cube such that the green center is facing down and the red center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<endl;
            break;
        } case 2: {//corresponds to face with blue center
            cout<<"Now hold the cube such that the red center is facing down and the blue center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<endl;
            break;
        } case 3: {//corresponds to face with orange center
            cout<<"Now hold the cube such that the blue center is facing down and the orange center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<endl;
            break;
        } case 4: {//corresponds to face with yellow center
            cout<<"Now hold the cube such that the green center is facing down and the yellow center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<endl;
            break;
        } case 5: {//corresponds to face with white center
            cout<<"Now hold the cube such that the green center is facing down and the white center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<endl;
            break;
        } default: {//theoretically this should never print because the counter for the loop in main() only runs from 0 to 5. This function accepts i (the loop counter) as its parameter
            cout<<"WHAAAAAAAAAAAAAAAAAAAAAT?"<<endl;
            break;
        }
    }
};

//this function checks whether everything the user inputs is in the allowed alphabetical form
bool AllAllowedCharacters(string s) {
    for (int i=0;i<s.size();i++) {
        char c=s.at(i);
        if ((!isalpha(c))||!(c=='g'||c=='r'||c=='b'||c=='o'||c=='y'||c=='w')) {//checks whether character is alphabetic AND either g,r,b,o,y or w
            return false;
        }
    }
    return true;
};
//this function converts a single digit (color code) and returns its corresponding color
string Number2Color(int number) {
    switch (number) {
        case 0: {
            return "green";
            break;
        } case 1: {
            return "red";
            break;
        } case 2: {
            return "blue";
            break;
        } case 3: {
            return "orange";
            break;
        } case 4: {
            return "yellow";
            break;
        } case 5: {
            return "white";
            break;
        } default: {
            return "error";
        }
    }
};
//This function checks to ensure the user is entering the right face of the cube
bool IsCorrectFace (string userinput, int face) {
    if (Letter2Digit(userinput.at(4))==face) {//checks to see whether fourth element corresponds to the face the program is asking for
        return true;
    } else {
        return false;
    }
};
//this function converts a user-input string into an integer which the program operates on. LOOKS LIKE WE MAY NOT NEED IT AFTER ALL.
/*
int String2Number (string s) {
    int result(0);
    for (int i=8;i>=0;i--) {
        result+=((Letter2Digit(s.at(i)))*(pow(10,(8-i))));//multiplies the corresponding value of each digit with corresponding place to find place value, then adds everything together. Starts with last digit, adds 10*next digit, etc.
    }
    
    return result;
};
 */

//this function converts each character of the string a user inputs into a digit which is fed into the String2Number function to create an integer equivalent of the user input.

int Letter2Digit (char c) {
    
    switch (c) {
        case 'g': {
            return 0;
            break;
        } case 'r': {
            return 1;
            break;
        } case 'b': {
            return 2;
            break;
        } case 'o': {
            return 3;
            break;
        } case 'y': {
            return 4;
            break;
        } case 'w': {
            return 5;
            break;
        } default: {
            return 1000000000;//this should be a sign that the input has a problem. Theoretically, the program shouldn't ever have to return this
        }
    }
};

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

