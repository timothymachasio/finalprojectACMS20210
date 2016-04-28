// ACMS20210 Final Project: Rubik's Cube Solver
// Team Members: Timothy Machasio, Nick Courtney and Colin Humble
// Instructor: Daniel Brake

//THIS CODE WILL BE SEPARATED INTO A MAIN.CPP FILE AND A CUBE.HPP FILE before the final submission

//This programs finds the most efficient solution to any given state of the Rubik's cube. It may run for a while, but the solution it finds is guaranteed to have the least number of moves (tradeoffs, anyone? :))
//To see magic, enter a cube in which face 1 is bgbogrbgb, face 2 is wwywrywyy, face 3 is gbgrbogbg, face 4 is wwywoywyy, face 5 is rboryorgo, and face 6 is obrowrogr.

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib>//this was included solely for the exit() function within the IDDFS function
#include <ctime>

//This is the Cube class. Technically we could just have had a bunch of free functions operating on global variables, but we thought this would be neater
class Cube {
    
    //MEMBER VARIABLES
    
    //contains values for a solved cube. Never changes, hence 'const'
    char const solvedmatrix[6][9] = {{'g','g','g','g','g','g','g','g','g'},{'r','r','r','r','r','r','r','r','r'},{'b','b','b','b','b','b','b','b','b'},{'o','o','o','o','o','o','o','o','o'},{'y','y','y','y','y','y','y','y','y'},{'w','w','w','w','w','w','w','w','w'}};
    
    //holds new values for the unsolved matrix as moves are performed on the cube.
    char workingmatrix[6][9];
    
    //keeps the initial values of the user-input cube. Changes as the functions (R, U, D, F etc.) run.
    char unsolvedmatrix[6][9];
    
    //keeps the values of the user-input cube intact, since the R(), U(), D() etc. functions distort the value of 'unsolvedmatrix' above.
    char originalmatrix[6][9];
    
public:
    
    //MEMBER FUNCTIONS (Definitions are elsewhere)
    //1. INPUT FUNCTIONS. These enable the user to input the state of the cube.
    //function creates instance of Rubik's Cube
    void Create(void);
    //This function prints out directions on how the user should input the state of the cube. It works in conjunction with the loop in main() that asks for user input, face by face.
    void Directions(int const& i);
    //this function checks whether everything the user inputs is in the allowed alphabetical form
    bool AllAllowedCharacters(std::string const& s);
    //this function converts a single digit (color code) and returns its corresponding color. Works hand in hand with 'Directions' function
    std::string Number2Color(int const& number);
    //works in conjunction with IsCorrectFace function
    int Letter2Digit (char const& c);
    //function turns the right face of the cube clockwise
    //This function checks to ensure the user is entering the right face of the cube
    bool IsCorrectFace (std::string const& userinput, int const& face);
    //function checks whether the input state of the cube is valid
    bool IsValidCube (void);
    
    //2. CUBE MOVEMENT FUNCTIONS. These mimic the effects of turning a certain face of the cube in the manner shown.
    //turns right face of cube clockwise
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
    //these functions turn the cube faces anticlockwise
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
    void numbertomove(char const& x);
    //function prints the current state of the cube
    void printcube(void);
    //function takes in number representing move and prints out the corresponding move
    void printmove(char const& x);
    //function checks whether cube is solved
    bool IsSolved(void);
    //function sets value of 'unsolvedmatrix' and originalmatrix. Works with 'for' loop in 'main' which provides value for int const& face
    void SetMatrices(int const& face, std::string const& s);
    
    //this function runs an unintelligent Iterative Deepening Depth First Search (IDDFS) starting from the the depth specified in its parameter
    void IDDFS(int startdepth);
    
};


int main() {//We would just like to mention that we're so proud of how short the main() function is.
    
    Cube Rubix;//Creating an instance of the Cube class. We know it is spelled RUBIK'S. Thanks, Bjarne!
    
    //THIS PART OF THE CODE RECEIVES INPUT FROM THE USER AND VALIDATES IT
    
    Rubix.Create();//Allows user to input state of the cube, and validates the input before the rest of the code runs.
    
    //2. CHECK WHETHER CUBE IS SOLVED. IF NOT, RUN IDDFS AND RETURN SOLUTION TO USER.
    
    if (Rubix.IsSolved())
    {
        
        std::cout<<"The cube is already solved. There's no work for me here."<<std::endl;
        
    }
    
    else
        
    {
        Rubix.IDDFS(1);//initiates the iterative deepening depth-first search starting from Depth 1. A heuristic function to estimate the lower bound of the depth at which the IDDFS should start running would very much be in order right about here.
    }
    
    //the IDDFS function returns 0 and automatically ends the program if a solution is found. How, you ask?
}

void Cube::Create() {
    
    goto entercube;
    
entercube:
    for (int i=0; i<6; i++) {
        
        std::string userinput;//this string holds the value of user-friendly input (a string)
        std::cout<<"Please enter the colors on the face of the cube with a "<<Number2Color(i)<<" center: "<<std::endl;
        std::cout<<std::endl;
        Directions(i);//gives user directions on how to enter the state of the cube depending on which face is being asked for. Cool, right?
        //Example (For face with red center-> corresponding to i=1). The function will print:
        //Now hold the cube such that the green center is facing down and the red center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last.
        std::cout<<std::endl;;
        std::cout<<"Please note that:"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"g - Green"<<std::endl;
        std::cout<<"r - Red"<<std::endl;
        std::cout<<"b - Blue"<<std::endl;
        std::cout<<"o - Orange"<<std::endl;
        std::cout<<"y - Yellow"<<std::endl;
        std::cout<<"w - White"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"Enter everything in lowercase. No spaces."<<std::endl;
        std::cin>>userinput;
        
        
        //step 1 of input validation: ensure that user has entered nine colors for each side of the cube
        while (!(userinput.size()==9)) {
            std::cout<<"What kind of Rubik's cube has a face with "<<userinput.size()<<" cubies? You must enter a series of nine characters, no more no less!"<<std::endl;
            std::cin>>userinput;//overwrites invalid values entered
        }
        
        //step 2 of input validation: ensure that user only enters alphabetic characters
        while (!AllAllowedCharacters(userinput)) {
            std::cout<<"You entered some characters which are not allowed. You can only enter g, r, b, o, y or w. Please enter the colors again. Don't be creative!"<<std::endl;
            std::cin>>userinput;//overwrites invalid values entered
        }
        
        
        //step 3 of input validation: to ensure the user has entered the correct face
        while (!IsCorrectFace(userinput,i)) {
            std::cout<<"You entered values for the wrong face. Please enter colors for the face with a "<<Number2Color(i)<<" center as per the instructions."<<std::endl;
            std::cin>>userinput;//overwrites the invalid values entered
        }
        
        //step 4 of input validation: ask user to confirm their input for each face
        char valid;
        std::cout<<"You entered: "<<userinput<<std::endl;
        std::cout<<"Is this correct?"<<std::endl;
        std::cout<<"Enter 'n' if not, enter 'y' (or anything else) if yes. 'n' must be lowercase"<<std::endl;
        std::cin>>valid;
        while (valid=='n') {
            std::cout<<"Please enter the color values for the face with a "<<Number2Color(i)<<" center cubie again:"<<std::endl;
            std::cin>>userinput;//overwrites wrong input
        }
        
        //feeds values of 'userinput' into Rubix instance of Cube
        
        SetMatrices(i,userinput);
        
        //just to make the console output neat
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
    }
    
    while (!IsValidCube()) {
        
        std::cout<<"The state of the cube you entered is not valid. Please enter the state of the cube again:"<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        goto entercube;//make user input the state of the cube all over again
    }
    
}

//USER INPUT AND DATA VALIDATION FUNCTIONS



//This function prints out directions on how the user should input the state of the cube. It works in conjunction with the loop in main() that asks for user input, face by face.
void Cube::Directions(int const& i) {
    switch (i) {
        case 0: {//corresponds to face with green center
            std::cout<<"Hold the cube such that the orange center is facing down and the green center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<std::endl;
            break;
        } case 1: {//corresponds to face with red center
            std::cout<<"Now hold the cube such that the green center is facing down and the red center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<std::endl;
            break;
        } case 2: {//corresponds to face with blue center
            std::cout<<"Now hold the cube such that the red center is facing down and the blue center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<std::endl;
            break;
        } case 3: {//corresponds to face with orange center
            std::cout<<"Now hold the cube such that the blue center is facing down and the orange center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<std::endl;
            break;
        } case 4: {//corresponds to face with yellow center
            std::cout<<"Now hold the cube such that the green center is facing down and the yellow center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<std::endl;
            break;
        } case 5: {//corresponds to face with white center
            std::cout<<"Now hold the cube such that the green center is facing down and the white center is facing you. Enter the colors on each cubie starting from the one on the top left and ending with the one on the bottom right. Read each row of colors from left to right, and enter the first row first, the second row second and the third row last."<<std::endl;
            break;
        } default: {//theoretically this should never print because the counter for the loop in main() only runs from 0 to 5. This function accepts i (the loop counter) as its parameter
            std::cout<<"WHAAAAAAAAAAAAAAAAAAAAAT?"<<std::endl;
            break;
        }
    }
};

//this function checks whether everything the user inputs is in the allowed alphabetical form
bool Cube::AllAllowedCharacters(std::string const& s) {
    for (int i=0;i<s.size();i++) {
        char c=s.at(i);
        if ((!isalpha(c))||!(c=='g'||c=='r'||c=='b'||c=='o'||c=='y'||c=='w')) {//checks whether character is alphabetic AND either g,r,b,o,y or w
            return false;
        }
    }
    return true;
};
//this function converts a single digit (color code) and returns its corresponding color
std::string Cube::Number2Color(int const& number) {
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
bool Cube::IsCorrectFace (std::string const& userinput, int const& face) {
    if (Letter2Digit(userinput.at(4))==face) {//checks to see whether fourth element corresponds to the face the program is asking for
        return true;
    } else {
        return false;
    }
};

//This function will ensure that the state of the entire cube the user inputs is valid.
 
bool Cube::IsValidCube(void) {
    
    bool edgescorrect = false;
    bool cornerscorrect = false;
    
    //stores colors on edgepieces of input rubik's cube
    std::vector<std::vector<char>> const edges ={{unsolvedmatrix[0][1],unsolvedmatrix[1][7]},{unsolvedmatrix[1][1],unsolvedmatrix[2][7]},{unsolvedmatrix[2][1],unsolvedmatrix[3][7]},{unsolvedmatrix[3][1],unsolvedmatrix[0][7]},{unsolvedmatrix[0][3],unsolvedmatrix[5][7]},{unsolvedmatrix[1][3],unsolvedmatrix[5][5]},{unsolvedmatrix[2][3],unsolvedmatrix[5][1]},{unsolvedmatrix[5][3],unsolvedmatrix[3][3]},{unsolvedmatrix[0][5],unsolvedmatrix[4][7]},{unsolvedmatrix[1][5],unsolvedmatrix[4][3]},{unsolvedmatrix[2][5],unsolvedmatrix[4][1]},{unsolvedmatrix[3][5],unsolvedmatrix[4][5]}};
    std::vector<std::vector<int>> const rightedges = {{'g','r'},{'g','y'},{'g','o'},{'g','w'},{'b','r'},{'b','w'},{'b','o'},{'b','y'},{'r','w'},{'r','y'},{'o','w'},{'o','y'}};
    //keeps values of valid vector indices
    std::vector<int> foundedges;
    
    for (int i=0;i<edges.size();i++) {
        for (int j=0;j<rightedges.size();j++) {
            if (std::is_permutation(edges[i].begin(),edges[i].end(),rightedges[j].begin())) {
                foundedges.push_back(i);//pushes to 'foundedges' position of valid vector combination. If all values of edges are correct, this vector will contain all values from 0 to (edges.size()-1)
            }
        }
    }
    //stores values for all vector positions that should be found to be valid before user input can be accepted. Ensures that no position features more than once
    std::vector<int> const shouldbefoundedges = {0,1,2,3,4,5,6,7,8,9,10,11};
    
    if (std::is_permutation(foundedges.begin(),foundedges.end(),shouldbefoundedges.begin())) {
        edgescorrect = true;//changes value of edgescorrect to 'true'. Must be 'true' for cube state to be accepted
    }
    
    
    
    //holds values of corner pieces entered by the user
    std::vector<std::vector<char>> const corners={{unsolvedmatrix[0][0],unsolvedmatrix[1][6],unsolvedmatrix[5][8]},{unsolvedmatrix[0][8],unsolvedmatrix[1][8],unsolvedmatrix[4][6]},{unsolvedmatrix[0][6],unsolvedmatrix[5][6],unsolvedmatrix[3][8]},{unsolvedmatrix[0][8],unsolvedmatrix[4][8],unsolvedmatrix[3][6]},{unsolvedmatrix[2][0],unsolvedmatrix[5][0],unsolvedmatrix[3][6]},{unsolvedmatrix[2][2],unsolvedmatrix[4][2],unsolvedmatrix[3][8]},{unsolvedmatrix[2][6],unsolvedmatrix[1][0],unsolvedmatrix[5][2]},{unsolvedmatrix[2][8],unsolvedmatrix[1][2],unsolvedmatrix[4][0]}};
    //holds correct possible values of corner pieces
    std::vector<std::vector<char>> rightcorners = {{'g','r','w'},{'g','r','y'},{'g','y','o'},{'g','o','w'},{'b','r','w'},{'b','w','o'},{'b','o','y'},{'b','r','y'}};
    //keeps values of valid vector indices enters by the user. Will keep values from 0 through 7
    std::vector<int> foundcorners;
    
    for (int i=0;i<corners.size();i++) {
        for (int j=0;j<rightcorners.size();j++) {
            if (std::is_permutation(corners[i].begin(),corners[i].end(),rightcorners[j].begin())) {
                foundcorners.push_back(i);//pushes to 'foundcorners' position of valid vector combination. If all values of corners are correct, this vector will contain all values from 0 to (corners.size()-1)
            }
        }
    }
    //contains arbitrary permutation of figures that should be in 'foundcorners' if all corners are positioned correctly
    std::vector<int> shouldbefoundcorners = {0,1,2,3,4,5,6,7};
    
    if (std::is_permutation(foundcorners.begin(),foundcorners.end(),shouldbefoundcorners.begin())) {
        cornerscorrect = true;//changes value of cornerscorrect to 'true'. Must be 'true' for cube state to be accepted.
    }
    
    if (edgescorrect&&cornerscorrect) {
        return true;
    } else {
        return false;
    }

};
 


//works in conjunction with IsCorrectFace function (see above)

int Cube::Letter2Digit (char const& c) {
    
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


//CUBE OPERATION AND NON-INPUT FUNCTIONS.


//assigns vales to 'originalmatrix' and 'unsolvedmatrix' in 'Cube' class. The value of 'face' is determined by a for loop within which the functions runs
void Cube::SetMatrices(int const& face, std::string const& s) {
    for (int j=0; j<9; j++) {
        unsolvedmatrix[face][j] = s.at(j);//this value will change as the functions R,U,D etc. run
        originalmatrix[face][j] = s.at(j);//this value will not change after a series of functions run, and will be reassined to 'originalmatrix' if a series of functions runs and a solution is not found
    }
}

//function corresponding to rotating left layer of cube (white facing left) clockwise
void Cube::R() {
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
void Cube::L() {
    workingmatrix[0][0]=unsolvedmatrix[1][0];
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
void Cube::U() {
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
void Cube::D() {
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
void Cube::F() {
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
void Cube::B() {
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

//these functions turn the cube faces anticlockwise
void Cube::Ri(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[1][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[1][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[1][8];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[2][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[2][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[2][8];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[3][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[3][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[3][8];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[0][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[0][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[0][8];
    workingmatrix[4][0]=unsolvedmatrix[4][2];
    workingmatrix[4][1]=unsolvedmatrix[4][5];
    workingmatrix[4][2]=unsolvedmatrix[4][8];
    workingmatrix[4][3]=unsolvedmatrix[4][1];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][7];
    workingmatrix[4][6]=unsolvedmatrix[4][0];
    workingmatrix[4][7]=unsolvedmatrix[4][3];
    workingmatrix[4][8]=unsolvedmatrix[4][6];
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

void Cube::Li(void) {
    workingmatrix[0][0]=unsolvedmatrix[3][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[3][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[3][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[0][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[0][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[0][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[1][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[1][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[1][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[2][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[2][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[2][6];
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
    workingmatrix[5][0]=unsolvedmatrix[5][2];
    workingmatrix[5][1]=unsolvedmatrix[5][5];
    workingmatrix[5][2]=unsolvedmatrix[5][8];
    workingmatrix[5][3]=unsolvedmatrix[5][1];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][7];
    workingmatrix[5][6]=unsolvedmatrix[5][0];
    workingmatrix[5][7]=unsolvedmatrix[5][3];
    workingmatrix[5][8]=unsolvedmatrix[5][6];

    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
    
};

void Cube::Ui(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[5][0];
    workingmatrix[1][1]=unsolvedmatrix[5][1];
    workingmatrix[1][2]=unsolvedmatrix[5][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[2][2];
    workingmatrix[2][1]=unsolvedmatrix[2][5];
    workingmatrix[2][2]=unsolvedmatrix[2][8];
    workingmatrix[2][3]=unsolvedmatrix[2][1];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][7];
    workingmatrix[2][6]=unsolvedmatrix[2][0];
    workingmatrix[2][7]=unsolvedmatrix[2][3];
    workingmatrix[2][8]=unsolvedmatrix[2][6];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[4][2];
    workingmatrix[3][7]=unsolvedmatrix[4][1];
    workingmatrix[3][8]=unsolvedmatrix[4][0];
    workingmatrix[4][0]=unsolvedmatrix[1][0];
    workingmatrix[4][1]=unsolvedmatrix[1][1];
    workingmatrix[4][2]=unsolvedmatrix[1][2];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[3][8];
    workingmatrix[5][1]=unsolvedmatrix[3][7];
    workingmatrix[5][2]=unsolvedmatrix[3][6];
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

void Cube::Di(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][2];
    workingmatrix[0][1]=unsolvedmatrix[0][5];
    workingmatrix[0][2]=unsolvedmatrix[0][8];
    workingmatrix[0][3]=unsolvedmatrix[0][1];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][7];
    workingmatrix[0][6]=unsolvedmatrix[0][0];
    workingmatrix[0][7]=unsolvedmatrix[0][3];
    workingmatrix[0][8]=unsolvedmatrix[0][6];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[4][6];
    workingmatrix[1][7]=unsolvedmatrix[4][7];
    workingmatrix[1][8]=unsolvedmatrix[4][8];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[5][8];
    workingmatrix[3][1]=unsolvedmatrix[5][7];
    workingmatrix[3][2]=unsolvedmatrix[5][6];
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
    workingmatrix[4][6]=unsolvedmatrix[3][2];
    workingmatrix[4][7]=unsolvedmatrix[3][1];
    workingmatrix[4][8]=unsolvedmatrix[3][0];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[1][6];
    workingmatrix[5][7]=unsolvedmatrix[1][7];
    workingmatrix[5][8]=unsolvedmatrix[1][8];

    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }

};
void Cube::Fi(void) {
    workingmatrix[0][0]=unsolvedmatrix[5][2];
    workingmatrix[0][1]=unsolvedmatrix[5][5];
    workingmatrix[0][2]=unsolvedmatrix[5][8];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[1][2];
    workingmatrix[1][1]=unsolvedmatrix[1][5];
    workingmatrix[1][2]=unsolvedmatrix[1][8];
    workingmatrix[1][3]=unsolvedmatrix[1][1];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][7];
    workingmatrix[1][6]=unsolvedmatrix[1][0];
    workingmatrix[1][7]=unsolvedmatrix[1][3];
    workingmatrix[1][8]=unsolvedmatrix[1][6];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[4][0];
    workingmatrix[2][7]=unsolvedmatrix[4][3];
    workingmatrix[2][8]=unsolvedmatrix[4][6];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[3][8];
    workingmatrix[4][0]=unsolvedmatrix[0][2];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[4][2];
    workingmatrix[4][3]=unsolvedmatrix[0][1];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[0][0];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[2][8];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[2][7];
    workingmatrix[5][6]=unsolvedmatrix[5][6];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[2][6];

    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }

};

void Cube::Bi(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[4][8];
    workingmatrix[0][7]=unsolvedmatrix[4][5];
    workingmatrix[0][8]=unsolvedmatrix[4][2];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[5][6];
    workingmatrix[2][1]=unsolvedmatrix[5][3];
    workingmatrix[2][2]=unsolvedmatrix[5][0];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[3][2];
    workingmatrix[3][1]=unsolvedmatrix[3][5];
    workingmatrix[3][2]=unsolvedmatrix[3][8];
    workingmatrix[3][3]=unsolvedmatrix[3][1];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][7];
    workingmatrix[3][6]=unsolvedmatrix[3][0];
    workingmatrix[3][7]=unsolvedmatrix[3][3];
    workingmatrix[3][8]=unsolvedmatrix[3][6];
    workingmatrix[4][0]=unsolvedmatrix[4][0];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[2][0];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[2][1];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[2][2];
    workingmatrix[5][0]=unsolvedmatrix[0][6];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[0][7];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[0][8];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[5][8];

    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }

};

//these functions turn the associated face of the cube twice
void Cube::R2(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[2][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[2][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[2][8];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[3][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[3][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[3][8];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[0][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[0][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[0][8];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[1][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[1][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[1][8];
    workingmatrix[4][0]=unsolvedmatrix[4][8];
    workingmatrix[4][1]=unsolvedmatrix[4][7];
    workingmatrix[4][2]=unsolvedmatrix[4][6];
    workingmatrix[4][3]=unsolvedmatrix[4][5];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][3];
    workingmatrix[4][6]=unsolvedmatrix[4][2];
    workingmatrix[4][7]=unsolvedmatrix[4][1];
    workingmatrix[4][8]=unsolvedmatrix[4][0];
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

void Cube::L2(void) {
    workingmatrix[0][0]=unsolvedmatrix[2][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[2][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[2][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[3][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[3][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[3][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[0][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[0][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[0][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[1][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[1][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[1][6];
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
    workingmatrix[5][0]=unsolvedmatrix[5][8];
    workingmatrix[5][1]=unsolvedmatrix[5][7];
    workingmatrix[5][2]=unsolvedmatrix[5][6];
    workingmatrix[5][3]=unsolvedmatrix[5][5];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][3];
    workingmatrix[5][6]=unsolvedmatrix[5][2];
    workingmatrix[5][7]=unsolvedmatrix[5][1];
    workingmatrix[5][8]=unsolvedmatrix[5][0];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

void Cube::U2(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[3][8];
    workingmatrix[1][1]=unsolvedmatrix[3][7];
    workingmatrix[1][2]=unsolvedmatrix[3][6];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[2][8];
    workingmatrix[2][1]=unsolvedmatrix[2][7];
    workingmatrix[2][2]=unsolvedmatrix[2][6];
    workingmatrix[2][3]=unsolvedmatrix[2][5];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][3];
    workingmatrix[2][6]=unsolvedmatrix[2][2];
    workingmatrix[2][7]=unsolvedmatrix[2][1];
    workingmatrix[2][8]=unsolvedmatrix[2][0];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[1][2];
    workingmatrix[3][7]=unsolvedmatrix[1][1];
    workingmatrix[3][8]=unsolvedmatrix[1][0];
    workingmatrix[4][0]=unsolvedmatrix[5][0];
    workingmatrix[4][1]=unsolvedmatrix[5][1];
    workingmatrix[4][2]=unsolvedmatrix[5][2];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[4][0];
    workingmatrix[5][1]=unsolvedmatrix[4][1];
    workingmatrix[5][2]=unsolvedmatrix[4][2];
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

//continue from here

void Cube::D2(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][8];
    workingmatrix[0][1]=unsolvedmatrix[0][7];
    workingmatrix[0][2]=unsolvedmatrix[0][6];
    workingmatrix[0][3]=unsolvedmatrix[0][5];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][3];
    workingmatrix[0][6]=unsolvedmatrix[0][2];
    workingmatrix[0][7]=unsolvedmatrix[0][1];
    workingmatrix[0][8]=unsolvedmatrix[0][0];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[3][2];
    workingmatrix[1][7]=unsolvedmatrix[3][1];
    workingmatrix[1][8]=unsolvedmatrix[3][0];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[1][8];
    workingmatrix[3][1]=unsolvedmatrix[1][7];
    workingmatrix[3][2]=unsolvedmatrix[1][6];
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
    workingmatrix[4][6]=unsolvedmatrix[5][6];
    workingmatrix[4][7]=unsolvedmatrix[5][7];
    workingmatrix[4][8]=unsolvedmatrix[5][8];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[4][6];
    workingmatrix[5][7]=unsolvedmatrix[4][7];
    workingmatrix[5][8]=unsolvedmatrix[4][8];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

//THERE IS A PROBLEM HERE

void Cube::F2(void) {
    workingmatrix[0][0]=unsolvedmatrix[2][8];
    workingmatrix[0][1]=unsolvedmatrix[2][7];
    workingmatrix[0][2]=unsolvedmatrix[2][6];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[0][6];
    workingmatrix[0][7]=unsolvedmatrix[0][7];
    workingmatrix[0][8]=unsolvedmatrix[0][8];
    workingmatrix[1][0]=unsolvedmatrix[1][8];
    workingmatrix[1][1]=unsolvedmatrix[1][7];
    workingmatrix[1][2]=unsolvedmatrix[1][6];
    workingmatrix[1][3]=unsolvedmatrix[1][5];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][3];
    workingmatrix[1][6]=unsolvedmatrix[1][2];
    workingmatrix[1][7]=unsolvedmatrix[1][1];
    workingmatrix[1][8]=unsolvedmatrix[1][0];
    workingmatrix[2][0]=unsolvedmatrix[2][0];
    workingmatrix[2][1]=unsolvedmatrix[2][1];
    workingmatrix[2][2]=unsolvedmatrix[2][2];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[0][2];
    workingmatrix[2][7]=unsolvedmatrix[0][1];
    workingmatrix[2][8]=unsolvedmatrix[0][0];
    workingmatrix[3][0]=unsolvedmatrix[3][0];
    workingmatrix[3][1]=unsolvedmatrix[3][1];
    workingmatrix[3][2]=unsolvedmatrix[3][2];
    workingmatrix[3][3]=unsolvedmatrix[3][3];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][5];
    workingmatrix[3][6]=unsolvedmatrix[3][6];
    workingmatrix[3][7]=unsolvedmatrix[3][7];
    workingmatrix[3][8]=unsolvedmatrix[3][8];
    workingmatrix[4][0]=unsolvedmatrix[5][8];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[4][2];
    workingmatrix[4][3]=unsolvedmatrix[5][5];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[4][5];
    workingmatrix[4][6]=unsolvedmatrix[5][2];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[4][8];
    workingmatrix[5][0]=unsolvedmatrix[5][0];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[4][6];
    workingmatrix[5][3]=unsolvedmatrix[5][3];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[4][3];
    workingmatrix[5][6]=unsolvedmatrix[5][6];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[4][0];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};
void Cube::B2(void) {
    workingmatrix[0][0]=unsolvedmatrix[0][0];
    workingmatrix[0][1]=unsolvedmatrix[0][1];
    workingmatrix[0][2]=unsolvedmatrix[0][2];
    workingmatrix[0][3]=unsolvedmatrix[0][3];
    workingmatrix[0][4]=unsolvedmatrix[0][4];
    workingmatrix[0][5]=unsolvedmatrix[0][5];
    workingmatrix[0][6]=unsolvedmatrix[2][2];
    workingmatrix[0][7]=unsolvedmatrix[2][1];
    workingmatrix[0][8]=unsolvedmatrix[2][0];
    workingmatrix[1][0]=unsolvedmatrix[1][0];
    workingmatrix[1][1]=unsolvedmatrix[1][1];
    workingmatrix[1][2]=unsolvedmatrix[1][2];
    workingmatrix[1][3]=unsolvedmatrix[1][3];
    workingmatrix[1][4]=unsolvedmatrix[1][4];
    workingmatrix[1][5]=unsolvedmatrix[1][5];
    workingmatrix[1][6]=unsolvedmatrix[1][6];
    workingmatrix[1][7]=unsolvedmatrix[1][7];
    workingmatrix[1][8]=unsolvedmatrix[1][8];
    workingmatrix[2][0]=unsolvedmatrix[0][8];
    workingmatrix[2][1]=unsolvedmatrix[0][7];
    workingmatrix[2][2]=unsolvedmatrix[0][6];
    workingmatrix[2][3]=unsolvedmatrix[2][3];
    workingmatrix[2][4]=unsolvedmatrix[2][4];
    workingmatrix[2][5]=unsolvedmatrix[2][5];
    workingmatrix[2][6]=unsolvedmatrix[2][6];
    workingmatrix[2][7]=unsolvedmatrix[2][7];
    workingmatrix[2][8]=unsolvedmatrix[2][8];
    workingmatrix[3][0]=unsolvedmatrix[3][8];
    workingmatrix[3][1]=unsolvedmatrix[3][7];
    workingmatrix[3][2]=unsolvedmatrix[3][6];
    workingmatrix[3][3]=unsolvedmatrix[3][5];
    workingmatrix[3][4]=unsolvedmatrix[3][4];
    workingmatrix[3][5]=unsolvedmatrix[3][3];
    workingmatrix[3][6]=unsolvedmatrix[3][2];
    workingmatrix[3][7]=unsolvedmatrix[3][1];
    workingmatrix[3][8]=unsolvedmatrix[3][0];
    workingmatrix[4][0]=unsolvedmatrix[4][0];
    workingmatrix[4][1]=unsolvedmatrix[4][1];
    workingmatrix[4][2]=unsolvedmatrix[5][6];
    workingmatrix[4][3]=unsolvedmatrix[4][3];
    workingmatrix[4][4]=unsolvedmatrix[4][4];
    workingmatrix[4][5]=unsolvedmatrix[5][3];
    workingmatrix[4][6]=unsolvedmatrix[4][6];
    workingmatrix[4][7]=unsolvedmatrix[4][7];
    workingmatrix[4][8]=unsolvedmatrix[5][0];
    workingmatrix[5][0]=unsolvedmatrix[4][8];
    workingmatrix[5][1]=unsolvedmatrix[5][1];
    workingmatrix[5][2]=unsolvedmatrix[5][2];
    workingmatrix[5][3]=unsolvedmatrix[4][5];
    workingmatrix[5][4]=unsolvedmatrix[5][4];
    workingmatrix[5][5]=unsolvedmatrix[5][5];
    workingmatrix[5][6]=unsolvedmatrix[4][2];
    workingmatrix[5][7]=unsolvedmatrix[5][7];
    workingmatrix[5][8]=unsolvedmatrix[5][8];
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            unsolvedmatrix[i][j]=workingmatrix[i][j];
        }
    }
};

bool Cube::IsSolved() {
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            if (unsolvedmatrix[i][j]!=solvedmatrix[i][j]) {
                return false;
            }
        }
    }
    return true;
};

//function to convert series of moves (in number form) to functions
void Cube::numbertomove(char const& x) {
    
    switch (x) {
        case 'a': {
            L();
            break;
        } case 'b': {
            R();
            break;
        } case 'c': {
            U();
            break;
        } case 'd': {
            D();
            break;
        } case 'e': {
            F();
            break;
        } case 'f': {
            B();
            break;
        } case 'g': {
            L2();
            break;
        } case 'h': {
            R2();
            break;
        } case 'i': {
            U2();
            break;
        } case 'j': {
            D2();
            break;
        } case 'k': {
            F2();
            break;
        } case 'l': {
            B2();
            break;
        } case 'm': {
            Li();
            break;
        } case 'n': {
            Ri();
            break;
        } case 'o': {
            Ui();
            break;
        } case 'p': {
            Di();
            break;
        } case 'q': {
            Fi();
            break;
        } case 'r': {
            Bi();
            break;
        } default: {
            break;
            //std::cout<<"If this prints, there is a problem where the 'numbertomove' function is called"<<std::endl;
        }
    }
    
};

void Cube::printmove(char const& x) {
    
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

//this function prints out the current state of the cube
void Cube::printcube() {
    
    for (int i=0;i<6;i++) {
        for (int j=0;j<9;j++) {
            std::cout<<unsolvedmatrix[i][j];
        }
        std::cout<<std::endl;
    }
};


//this is the 'big bad function' - a function which runs the iterative deepening depth-first search. There were way more elegant ways of implementing this, but a bunch of for loops actually ended up being the easiest to write and fastest to implement.

void Cube::IDDFS(int startdepth) {
    
    std::time_t start = std::time(NULL);//to keep track of how long the program runs for
    
    int depth = startdepth;
    
    do {
        if (depth==1) {
            for (char a='a';a<'s';a++) {
                numbertomove(a);//tries all possible combinations of one move.
                if (IsSolved()) {
                    std::cout<<"Solved! Steps for solving: "<<std::endl;
                    printmove(a);
                    goto label;//label prints out instructions on how the cube should be solved using the generated moves and exits the program.
                } else {
                    for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                        for (int j=0;j<9;j++) {
                            unsolvedmatrix[i][j]=originalmatrix[i][j];
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;//You know, just to make sure the user knows what's going on.
            IDDFS(depth+1);//RECURSION. See for every other series of nested loops below. Will only run if solution is not found.
        } else if (depth==2) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    numbertomove(a);
                    numbertomove(b);//tries all possible combinations of two moves. The next series of nested for loops test all possible combinations of three moves etc.
                    if (IsSolved()) {
                        std::cout<<"Solved! Steps for solving: "<<std::endl;
                        printmove(a);
                        printmove(b);
                        goto label;
                    }else {
                        for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                            for (int j=0;j<9;j++) {
                                unsolvedmatrix[i][j]=originalmatrix[i][j];
                            }
                        }
                    }
                    
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==3) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        numbertomove(a);
                        numbertomove(b);
                        numbertomove(c);
                        
                        if (IsSolved()) {
                            std::cout<<"Solved! Steps for solving: "<<std::endl;
                            printmove(a);
                            printmove(b);
                            printmove(c);
                            goto label;
                        } else {
                            for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                for (int j=0;j<9;j++) {
                                    unsolvedmatrix[i][j]=originalmatrix[i][j];
                                }
                            }
                        }
                        
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==4) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            numbertomove(a);
                            numbertomove(b);
                            numbertomove(c);
                            numbertomove(d);
                            if (IsSolved()) {
                                std::cout<<"Solved! Steps for solving: "<<std::endl;
                                printmove(a);
                                printmove(b);
                                printmove(c);
                                printmove(d);
                                goto label;
                            } else {
                                for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                    for (int j=0;j<9;j++) {
                                        unsolvedmatrix[i][j]=originalmatrix[i][j];
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==5) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                numbertomove(a);
                                numbertomove(b);
                                numbertomove(c);
                                numbertomove(d);
                                numbertomove(e);
                                if (IsSolved()) {
                                    std::cout<<"Solved! Steps for solving: "<<std::endl;
                                    printmove(a);
                                    printmove(b);
                                    printmove(c);
                                    printmove(d);
                                    printmove(e);
                                    goto label;
                                } else {
                                    for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                        for (int j=0;j<9;j++) {
                                            unsolvedmatrix[i][j]=originalmatrix[i][j];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==6) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    numbertomove(a);
                                    numbertomove(b);
                                    numbertomove(c);
                                    numbertomove(d);
                                    numbertomove(e);
                                    numbertomove(f);
                                    if (IsSolved()) {
                                        std::cout<<"Solved! Steps for solving: "<<std::endl;
                                        printmove(a);
                                        printmove(b);
                                        printmove(c);
                                        printmove(d);
                                        printmove(e);
                                        printmove(f);
                                        goto label;
                                    } else {
                                        for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                            for (int j=0;j<9;j++) {
                                                unsolvedmatrix[i][j]=originalmatrix[i][j];
                                            }
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==7) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        numbertomove(a);
                                        numbertomove(b);
                                        numbertomove(c);
                                        numbertomove(d);
                                        numbertomove(e);
                                        numbertomove(f);
                                        numbertomove(g);
                                        if (IsSolved()) {
                                            std::cout<<"Solved! Steps for solving: "<<std::endl;
                                            printmove(a);
                                            printmove(b);
                                            printmove(c);
                                            printmove(d);
                                            printmove(e);
                                            printmove(f);
                                            printmove(g);
                                            goto label;
                                        } else {
                                            for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                for (int j=0;j<9;j++) {
                                                    unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==8) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            numbertomove(a);
                                            numbertomove(b);
                                            numbertomove(c);
                                            numbertomove(d);
                                            numbertomove(e);
                                            numbertomove(f);
                                            numbertomove(g);
                                            numbertomove(h);
                                            if (IsSolved()) {
                                                std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                printmove(a);
                                                printmove(b);
                                                printmove(c);
                                                printmove(d);
                                                printmove(e);
                                                printmove(f);
                                                printmove(g);
                                                printmove(h);
                                                goto label;
                                            } else {
                                                for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                    for (int j=0;j<9;j++) {
                                                        unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==9) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<='s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                numbertomove(a);
                                                numbertomove(b);
                                                numbertomove(c);
                                                numbertomove(d);
                                                numbertomove(e);
                                                numbertomove(f);
                                                numbertomove(g);
                                                numbertomove(h);
                                                numbertomove(i);
                                                if (IsSolved()) {
                                                    std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                    printmove(a);
                                                    printmove(b);
                                                    printmove(c);
                                                    printmove(d);
                                                    printmove(e);
                                                    printmove(f);
                                                    printmove(g);
                                                    printmove(h);
                                                    printmove(i);
                                                    goto label;
                                                } else {
                                                    for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                        for (int j=0;j<9;j++) {
                                                            unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==10) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    numbertomove(a);
                                                    numbertomove(b);
                                                    numbertomove(c);
                                                    numbertomove(d);
                                                    numbertomove(e);
                                                    numbertomove(f);
                                                    numbertomove(g);
                                                    numbertomove(h);
                                                    numbertomove(i);
                                                    numbertomove(j);
                                                    if (IsSolved()) {
                                                        std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                        printmove(a);
                                                        printmove(b);
                                                        printmove(c);
                                                        printmove(d);
                                                        printmove(e);
                                                        printmove(f);
                                                        printmove(g);
                                                        printmove(h);
                                                        printmove(i);
                                                        printmove(j);
                                                        goto label;
                                                    }  else {
                                                        for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                            for (int j=0;j<9;j++) {
                                                                unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==11) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        numbertomove(a);
                                                        numbertomove(b);
                                                        numbertomove(c);
                                                        numbertomove(d);
                                                        numbertomove(e);
                                                        numbertomove(f);
                                                        numbertomove(g);
                                                        numbertomove(h);
                                                        numbertomove(i);
                                                        numbertomove(j);
                                                        numbertomove(k);
                                                        if (IsSolved()) {
                                                            std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                            printmove(a);
                                                            printmove(b);
                                                            printmove(c);
                                                            printmove(d);
                                                            printmove(e);
                                                            printmove(f);
                                                            printmove(g);
                                                            printmove(h);
                                                            printmove(i);
                                                            printmove(j);
                                                            printmove(k);
                                                            goto label;
                                                        } else {
                                                            for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                for (int j=0;j<9;j++) {
                                                                    unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                }
                                                            }
                                                        }
                                                        
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==12) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            numbertomove(a);
                                                            numbertomove(b);
                                                            numbertomove(c);
                                                            numbertomove(d);
                                                            numbertomove(e);
                                                            numbertomove(f);
                                                            numbertomove(g);
                                                            numbertomove(h);
                                                            numbertomove(i);
                                                            numbertomove(j);
                                                            numbertomove(k);
                                                            numbertomove(l);
                                                            if (IsSolved()) {
                                                                std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                printmove(a);
                                                                printmove(b);
                                                                printmove(c);
                                                                printmove(d);
                                                                printmove(e);
                                                                printmove(f);
                                                                printmove(g);
                                                                printmove(h);
                                                                printmove(i);
                                                                printmove(j);
                                                                printmove(k);
                                                                printmove(l);
                                                                goto label;
                                                            } else {
                                                                for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                    for (int j=0;j<9;j++) {
                                                                        unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                    }
                                                                }
                                                            }
                                                            
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==13) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                numbertomove(a);
                                                                numbertomove(b);
                                                                numbertomove(c);
                                                                numbertomove(d);
                                                                numbertomove(e);
                                                                numbertomove(f);
                                                                numbertomove(g);
                                                                numbertomove(h);
                                                                numbertomove(i);
                                                                numbertomove(j);
                                                                numbertomove(k);
                                                                numbertomove(l);
                                                                numbertomove(m);
                                                                if (IsSolved()) {
                                                                    std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                    printmove(a);
                                                                    printmove(b);
                                                                    printmove(c);
                                                                    printmove(d);
                                                                    printmove(e);
                                                                    printmove(f);
                                                                    printmove(g);
                                                                    printmove(h);
                                                                    printmove(i);
                                                                    printmove(j);
                                                                    printmove(k);
                                                                    printmove(l);
                                                                    printmove(m);
                                                                    goto label;
                                                                } else {
                                                                    for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                        for (int j=0;j<9;j++) {
                                                                            unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==14) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    numbertomove(a);
                                                                    numbertomove(b);
                                                                    numbertomove(c);
                                                                    numbertomove(d);
                                                                    numbertomove(e);
                                                                    numbertomove(f);
                                                                    numbertomove(g);
                                                                    numbertomove(h);
                                                                    numbertomove(i);
                                                                    numbertomove(j);
                                                                    numbertomove(k);
                                                                    numbertomove(l);
                                                                    numbertomove(m);
                                                                    numbertomove(n);
                                                                    if (IsSolved()) {
                                                                        std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                        printmove(a);
                                                                        printmove(b);
                                                                        printmove(c);
                                                                        printmove(d);
                                                                        printmove(e);
                                                                        printmove(f);
                                                                        printmove(g);
                                                                        printmove(h);
                                                                        printmove(i);
                                                                        printmove(j);
                                                                        printmove(k);
                                                                        printmove(l);
                                                                        printmove(m);
                                                                        printmove(n);
                                                                        goto label;
                                                                    } else {
                                                                        for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                            for (int j=0;j<9;j++) {
                                                                                unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==15) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    for (char o='a';o<'s';o++) {
                                                                        numbertomove(a);
                                                                        numbertomove(b);
                                                                        numbertomove(c);
                                                                        numbertomove(d);
                                                                        numbertomove(e);
                                                                        numbertomove(f);
                                                                        numbertomove(g);
                                                                        numbertomove(h);
                                                                        numbertomove(i);
                                                                        numbertomove(j);
                                                                        numbertomove(k);
                                                                        numbertomove(l);
                                                                        numbertomove(m);
                                                                        numbertomove(n);
                                                                        numbertomove(o);
                                                                        if (IsSolved()) {
                                                                            std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                            printmove(a);
                                                                            printmove(b);
                                                                            printmove(c);
                                                                            printmove(d);
                                                                            printmove(e);
                                                                            printmove(f);
                                                                            printmove(g);
                                                                            printmove(h);
                                                                            printmove(i);
                                                                            printmove(j);
                                                                            printmove(k);
                                                                            printmove(l);
                                                                            printmove(m);
                                                                            printmove(n);
                                                                            printmove(o);
                                                                            goto label;
                                                                        } else {
                                                                            for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                                for (int j=0;j<9;j++) {
                                                                                    unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==16) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    for (char o='a';o<'s';o++) {
                                                                        for (char p='a';p<'s';p++) {
                                                                            numbertomove(a);
                                                                            numbertomove(b);
                                                                            numbertomove(c);
                                                                            numbertomove(d);
                                                                            numbertomove(e);
                                                                            numbertomove(f);
                                                                            numbertomove(g);
                                                                            numbertomove(h);
                                                                            numbertomove(i);
                                                                            numbertomove(j);
                                                                            numbertomove(k);
                                                                            numbertomove(l);
                                                                            numbertomove(m);
                                                                            numbertomove(n);
                                                                            numbertomove(o);
                                                                            numbertomove(p);
                                                                            if (IsSolved()) {
                                                                                std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                                printmove(a);
                                                                                printmove(b);
                                                                                printmove(c);
                                                                                printmove(d);
                                                                                printmove(e);
                                                                                printmove(f);
                                                                                printmove(g);
                                                                                printmove(h);
                                                                                printmove(i);
                                                                                printmove(j);
                                                                                printmove(k);
                                                                                printmove(l);
                                                                                printmove(m);
                                                                                printmove(n);
                                                                                printmove(o);
                                                                                printmove(p);
                                                                                goto label;
                                                                            } else {
                                                                                for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                                    for (int j=0;j<9;j++) {
                                                                                        unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==17) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    for (char o='a';o<'s';o++) {
                                                                        for (char p='a';p<'s';p++) {
                                                                            for (char q='a';q<'s';q++) {
                                                                                numbertomove(a);
                                                                                numbertomove(b);
                                                                                numbertomove(c);
                                                                                numbertomove(d);
                                                                                numbertomove(e);
                                                                                numbertomove(f);
                                                                                numbertomove(g);
                                                                                numbertomove(h);
                                                                                numbertomove(i);
                                                                                numbertomove(j);
                                                                                numbertomove(k);
                                                                                numbertomove(l);
                                                                                numbertomove(m);
                                                                                numbertomove(n);
                                                                                numbertomove(o);
                                                                                numbertomove(p);
                                                                                numbertomove(q);
                                                                                if (IsSolved()) {
                                                                                    std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                                    printmove(a);
                                                                                    printmove(b);
                                                                                    printmove(c);
                                                                                    printmove(d);
                                                                                    printmove(e);
                                                                                    printmove(f);
                                                                                    printmove(g);
                                                                                    printmove(h);
                                                                                    printmove(i);
                                                                                    printmove(j);
                                                                                    printmove(k);
                                                                                    printmove(l);
                                                                                    printmove(m);
                                                                                    printmove(n);
                                                                                    printmove(o);
                                                                                    printmove(p);
                                                                                    printmove(q);
                                                                                    goto label;
                                                                                } else {
                                                                                    for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                                        for (int j=0;j<9;j++) {
                                                                                            unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==18) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    for (char o='a';o<'s';o++) {
                                                                        for (char p='a';p<'s';p++) {
                                                                            for (char q='a';q<'s';q++) {
                                                                                for (char r='a';r<'s';r++) {
                                                                                    numbertomove(a);
                                                                                    numbertomove(b);
                                                                                    numbertomove(c);
                                                                                    numbertomove(d);
                                                                                    numbertomove(e);
                                                                                    numbertomove(f);
                                                                                    numbertomove(g);
                                                                                    numbertomove(h);
                                                                                    numbertomove(i);
                                                                                    numbertomove(j);
                                                                                    numbertomove(k);
                                                                                    numbertomove(l);
                                                                                    numbertomove(m);
                                                                                    numbertomove(n);
                                                                                    numbertomove(o);
                                                                                    numbertomove(p);
                                                                                    numbertomove(q);
                                                                                    numbertomove(r);
                                                                                    if (IsSolved()) {
                                                                                        std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                                        printmove(a);
                                                                                        printmove(b);
                                                                                        printmove(c);
                                                                                        printmove(d);
                                                                                        printmove(e);
                                                                                        printmove(f);
                                                                                        printmove(g);
                                                                                        printmove(h);
                                                                                        printmove(i);
                                                                                        printmove(j);
                                                                                        printmove(k);
                                                                                        printmove(l);
                                                                                        printmove(m);
                                                                                        printmove(n);
                                                                                        printmove(o);
                                                                                        printmove(p);
                                                                                        printmove(q);
                                                                                        printmove(r);
                                                                                        goto label;
                                                                                    } else {
                                                                                        for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                                            for (int j=0;j<9;j++) {
                                                                                                unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        } else if (depth==19) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    for (char o='a';o<'s';o++) {
                                                                        for (char p='a';p<'s';p++) {
                                                                            for (char q='a';q<'s';q++) {
                                                                                for (char r='a';r<'s';r++) {
                                                                                    for (char s='a';s<'s';s++) {
                                                                                        numbertomove(a);
                                                                                        numbertomove(b);
                                                                                        numbertomove(c);
                                                                                        numbertomove(d);
                                                                                        numbertomove(e);
                                                                                        numbertomove(f);
                                                                                        numbertomove(g);
                                                                                        numbertomove(h);
                                                                                        numbertomove(i);
                                                                                        numbertomove(j);
                                                                                        numbertomove(k);
                                                                                        numbertomove(l);
                                                                                        numbertomove(m);
                                                                                        numbertomove(n);
                                                                                        numbertomove(o);
                                                                                        numbertomove(p);
                                                                                        numbertomove(q);
                                                                                        numbertomove(r);
                                                                                        numbertomove(s);
                                                                                        if (IsSolved()) {
                                                                                            std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                                            printmove(a);
                                                                                            printmove(b);
                                                                                            printmove(c);
                                                                                            printmove(d);
                                                                                            printmove(e);
                                                                                            printmove(f);
                                                                                            printmove(g);
                                                                                            printmove(h);
                                                                                            printmove(i);
                                                                                            printmove(j);
                                                                                            printmove(k);
                                                                                            printmove(l);
                                                                                            printmove(m);
                                                                                            printmove(n);
                                                                                            printmove(o);
                                                                                            printmove(p);
                                                                                            printmove(q);
                                                                                            printmove(r);
                                                                                            printmove(s);
                                                                                            goto label;
                                                                                        } else {
                                                                                            for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                                                for (int j=0;j<9;j++) {
                                                                                                    unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<"Now searching depth "<<depth+1<<"..."<<std::endl;
            IDDFS(depth+1);
        }else if (depth==20) {
            for (char a='a';a<'s';a++) {
                for (char b='a';b<'s';b++) {
                    for (char c='a';c<'s';c++) {
                        for (char d='a';d<'s';d++) {
                            for (char e='a';e<'s';e++) {
                                for (char f='a';f<'s';f++) {
                                    for (char g='a';g<'s';g++) {
                                        for (char h='a';h<'s';h++) {
                                            for (char i='a';i<'s';i++) {
                                                for (char j='a';j<'s';j++) {
                                                    for (char k='a';k<'s';k++) {
                                                        for (char l='a';l<'s';l++) {
                                                            for (char m='a';m<'s';m++) {
                                                                for (char n='a';n<'s';n++) {
                                                                    for (char o='a';o<'s';o++) {
                                                                        for (char p='a';p<'s';p++) {
                                                                            for (char q='a';q<'s';q++) {
                                                                                for (char r='a';r<'s';r++) {
                                                                                    for (char s='a';s<'s';s++) {
                                                                                        for (char t='a';t<'s';a++) {
                                                                                            numbertomove(a);
                                                                                            numbertomove(b);
                                                                                            numbertomove(c);
                                                                                            numbertomove(d);
                                                                                            numbertomove(e);
                                                                                            numbertomove(f);
                                                                                            numbertomove(g);
                                                                                            numbertomove(h);
                                                                                            numbertomove(i);
                                                                                            numbertomove(j);
                                                                                            numbertomove(k);
                                                                                            numbertomove(l);
                                                                                            numbertomove(m);
                                                                                            numbertomove(n);
                                                                                            numbertomove(o);
                                                                                            numbertomove(p);
                                                                                            numbertomove(q);
                                                                                            numbertomove(r);
                                                                                            numbertomove(s);
                                                                                            numbertomove(t);
                                                                                            if (IsSolved()) {
                                                                                                std::cout<<"Solved! Steps for solving: "<<std::endl;
                                                                                                printmove(a);
                                                                                                printmove(b);
                                                                                                printmove(c);
                                                                                                printmove(d);
                                                                                                printmove(e);
                                                                                                printmove(f);
                                                                                                printmove(g);
                                                                                                printmove(h);
                                                                                                printmove(i);
                                                                                                printmove(j);
                                                                                                printmove(k);
                                                                                                printmove(l);
                                                                                                printmove(m);
                                                                                                printmove(n);
                                                                                                printmove(o);
                                                                                                printmove(p);
                                                                                                printmove(q);
                                                                                                printmove(r);
                                                                                                printmove(s);
                                                                                                printmove(t);
                                                                                                goto label;//guarantees loop termination
                                                                                            } else {
                                                                                                for (int i=0;i<6;i++) {//reassign values of originalmatrix to unsolvedmatrix before trying again
                                                                                                    for (int j=0;j<9;j++) {
                                                                                                        unsolvedmatrix[i][j]=originalmatrix[i][j];
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
    } while (depth<21);
    
    std::cout<<"Solution was not found. That is a weird, weird cube state. By the way, how old are you now? :D"<<std::endl;//Theory indicates that this should never print. The maximum number of moves to solve a cube in any state is supposed to be 20.
    
label:
    std::cout<<std::endl;
    std::cout<<"The search took "<<std::difftime(std::time(NULL),start)<<" seconds."<<std::endl;//print out how long it took the program to find solution to given cube state
    std::cout<<std::endl;
    std::cout<<"To solve the cube using the generated moves, hold the cube such that the green center is facing down and the red center is facing you."<<std::endl;
    std::exit(0);//from cstdlib. Pretty cool, right?
    
}

