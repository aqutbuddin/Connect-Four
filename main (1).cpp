// Project 6: Connect 4 Game with Backtracking 
// Aima Qutbuddin 
// Date: 12/01/2023
// Professor M.T., Fall 2023  
// Environment: Replit and Zybooks 
// In this project, we added to the functional Connect 4 game we created in Project 5. 
// Specifically, I added a function that outputs all the moves created by the players upon the user input of a 'P' or 'p'.  
// I also added a function that undoes the most recent move made by the user upon the user input of a 'U' or 'u'.
//To create and correctly execute these functions, I utilized a class and implemented a linked list to store the moves of each player.  
// Pass by reference allowed me to manipulate the boards and pointers associated with each node.  

// Header Files 
#include <iostream>
#include <string>
#include <cctype>
using namespace std; // utilized to avoid the std repetition for all statements

class gameBoard{ // creating a class, 
    public: // public accessor
    char board[6][7]; // 2D array is the data type stored to node 
    gameBoard* next; // pointer called next, used to point to the next node in the list 

    gameBoard(){ //default constructor that builds the initial board
        for (int row = 0; row < 6; row++){ // nested loop to build the board with rows and columns 
            for (int col = 0; col < 7; col++) {
                board[row][col] = '-';
            }
        }
    }
 
    // class method
    void displayBoard(){ // display function for the output after each update to the game board 
        cout <<"\t0 1 2 3 4 5 6"<< endl; 
        for(int row = 0; row < 6; row++){ // nested loop for the columns and rows of the gameboard
            cout<<"\t";
            for (int column = 0; column < 7; column++){
                if (board[row][column] == 'X' || board[row][column] == 'O'){
                    cout << board[row][column]; // populates the board with the users' move and the actual display 
                    if(column!=6){ 
                        cout<<" ";
                    }
                }
                else{
                    board[row][column] = '-'; //allows for board to be populated with dashes
                    cout << board[row][column]; //outputs the board, actual displaying 
                    if(column!=6){
                        cout<<" ";
                    }
                }
            
            }
        cout << endl;
        }  
    }
};

// function that makes the move on the board and  stores a copy to a node of the linked list 
gameBoard* makePlay(char column, gameBoard*& head, char &turn){ // parameters for the function, notice pas by reference to directly manipulate the board
    // changing the char to an integer to utilize the column value as an integer
    int numColumnPiece = int(column - '0');
    int row;

    // Create a new board for the current play
    gameBoard* updatedBoard = new gameBoard();
    for (row = 0; row < 6; row++) { // looping through and rows and columns of the board 
        for (int col = 0; col < 7; col++) {
            updatedBoard->board[row][col] = head->board[row][col]; // making a copy of the last node's board 
        }
    }

    for (row = 5; row >= 0; row--) { // looping through the rows 
        if (updatedBoard->board[row][numColumnPiece] == '-') {
            updatedBoard->board[row][numColumnPiece] = turn; // making the move on the new copy of board
            break; //exit loop once the move is made
        }
    }
    //Creating a link between the new board to the previous one
    // pushing in nodes through the head of the list, the newest node added becomes the head of the list 
    updatedBoard->next = head;
    head = updatedBoard; // head is reassigned to the most up-to-date board

    return head;// returning the most updated board as the headboard
}
// function that outputs all the moves made by the users until the point of a user input of 'P' or 'p'
void PrintMovesMade(gameBoard* head){ //pointer, to manipulate the board associated with the pointer 
    if(head == nullptr){ // conditional checking if the head pointer is null
        return;
    }
    PrintMovesMade(head->next); // recursive all to output the boards in order of no move made, to most updated board
    head->displayBoard(); // actually displaying the headboard associated with the head node. 
    cout<<endl;
}
//function that undoes the most current move made by reassigning the head 
gameBoard* undoMove(gameBoard*& head){ 
    if(head->next == nullptr){ // condition checking if the next pointer is null, 
        gameBoard emptyBoard; 
        emptyBoard.displayBoard(); // displaying an empty board, when the next pointer goes to null
        return nullptr; // no more undo moves to be made 
    }

    gameBoard* tempBoard = head; // create temporary board and store the head 
    head = head->next; // assigning the head to the board previous to it, with one less move than the most updated board 
    delete tempBoard; // removing the temporary board holding the previously most updated board 

    if (head != nullptr){ // when the head is not null condition being checked 
        head->displayBoard(); // display the board with a move undone
    }

    return head;  // returning the undone move, reassigned as the head 
}
//function checking if all the dashes of the column have been populated with a turn, so an X or an O
bool columnFull(int column, gameBoard* head){
    for (int row = 5 ; row >= 0 ; row--){ //looping through all the rows of that column 
        if (head->board[row][column] == '-'){ // when dash detected the column  is not full
            return false; // false, there is still an available spot 
        }
    }
    return true; // true, no spots available, column full
}
//function checking the horizontal win, 
// making sure that 4 spaces within a row are all the same piece either X or O
bool checkHorizontalWin(gameBoard* head, char turn){
    for(int row = 5; row >= 0; row--){ //decrementing to start checking on the upper side of the board
        for(int column = 0; column < 4; column++){ // checking through the spots next to a piece 
            if(head->board[row][column] == turn && 
                head->board[row][column + 1] == turn && 
                head->board[row][column + 2] == turn && 
                head->board[row][column + 3] == turn){
                return true; // true, there is a horizontal win detected  
            }
        }
    }
  return false;// false, no horizontal win detected 
}
//function checking for the  vertical win, 
// making sure that 4 spaces within a column are all the same piece either X or O
bool checkVerticalWin(gameBoard* head, char turn){
    for(int row = 5; row >= 3; row--){ //decrementing to start checking on the upper side of the board
        for(int column = 0; column < 7; column++){ 
            if(head->board[row][column] == turn && // checking through the spots above a piece
            head->board[row - 1][column] == turn && 
            head->board[row - 2][column] == turn && 
            head->board[row - 3][column] == turn){
            return true;// true, there is a vertical win detected
            }
        }
    }
  return false;// false, no vertical win detected 
}
//function checking for the diagonal win, (bottom left corner to top right corner)
// making sure that 4 spaces diagonal to a piece are all the same piece either X or O
bool checkDiagonalWin1(gameBoard* head, char turn){
    for (int row = 5; row >= 3; row--) { //decrementing to start checking on the upper side of the board
        for (int col = 0; col < 4; col++) {
            if (head->board[row][col] == turn && //checking the piece above and to the left, 4 in a diagonal row 
                head->board[row - 1][col + 1] == turn &&
                head->board[row - 2][col + 2] == turn &&
                head->board[row - 3][col + 3] == turn) {
                return true; // true, diagonal win detected
            }
        }
    }
  return false; // false, diagonal win not detected
}
//function checking for the diagonal win, (bottom right corner to top left corner)
// making sure that 4 spaces diagonal to a piece are all the same piece either X or O
bool checkDiagonalWin2(gameBoard* head, char turn){
    for (int row = 5; row >= 3; row--) { //decrementing to start checking on the upper side of the board
        for (int col = 0; col < 4; col++) {
            if (head->board[row][col] == turn && //checking the piece below and to the right, 4 in a diagonal row 
                head->board[row - 1][col - 1] == turn &&
                head->board[row - 2][col - 2] == turn &&
                head->board[row - 3][col - 3] == turn){ 
                return true;// true, diagonal win detected
            }
        }
    }
  return false; // false, diagonal win not detected
}
//function searches for a draw by looking to see if each spot is populated with a piece, either X or O
bool boardFull(gameBoard* head){
    for(int row = 0; row < 6; row++){ //looping through the 2D array
        for(int column = 0; column< 7; column++){ 
            if(head->board[row][column] == '-'){ // when dash detected the board is not full
                return false; //false, board is not full 
            }
        }
    }
  return true; //true board is full
}
int main(){
    
    // initializing variables, specifically the column piece, the gameBoard, and 
    // the turn of the current player --> always set to 'X' first
    char columnPiece; 
    char playerTurn = 'X'; 

    // creating pointers of class gameBoard 
    gameBoard* initialBoard = new gameBoard(); 
    gameBoard* head = nullptr;
    
    while(true){ //gameplay loop 
        // prompting for input with count statement and receiving input with cin statement
        cout << "It is " << playerTurn << "'s turn." << endl;
        cout << "Enter a column to place your piece.";
        cin >> columnPiece;
        //cout << endl; 

        if (columnPiece == 'Q' || columnPiece == 'q'){ // in the case where the user would like to quit the game
            break; // break out of the loop, no more gameplay 
        }
        else if(columnPiece == 'P' || columnPiece == 'p'){ // in the case where the user wants to print all the moves made
            PrintMovesMade(head);  // calling function
            break;// break out of the loop, no more gameplay 
        }
        else if(columnPiece == 'U' || columnPiece == 'u'){ // in the case where the user wants to undo a move 
            head = undoMove(head); // calling function, assigning to head
            if (head!= nullptr){  // when the head not null, switch the user because undo counts as a play 
                playerTurn = (playerTurn == 'X') ? 'O' : 'X';
            }
            continue;// continue gameplay 
        }

        if(isdigit(columnPiece)){ // checking, if the user entered char, is a digit or not 
            int columnInput = int(columnPiece - '0'); // changing the char to an integer 
            // checking integer falls between the bounds of an acceptable column window 
            if (columnInput >= 0 && columnInput < 7){ 
                // making sure there is room within their chosen column for a move to be made 
                if(head!=nullptr){
                    if(columnFull(columnInput, head)){ 
                        cout << "column chosen is already full" << endl;
                        continue; // break out of the current iteration and start the next 
                    }
                }
                //calling function to make moves and update the board with the board that has the move made 
                if(head == nullptr){ // when condition met, update the initial board pointer
                    head = makePlay(columnPiece, initialBoard, playerTurn);
                }
                else{ // when condition not met, update the head board pointer
                    head = makePlay(columnPiece, head, playerTurn); 
                } 
                head->displayBoard(); // displaying the updated board with the move 
            }
            else{ // prompting whne iinvalid column number entered 
                cout << "Please enter a valid column." << endl; 
                continue; // break out of the current iteration and start the next
            }
        }
        // when conditions for any type of win( horizontal,vertical, or 1 of the 2 diagonals) are met
        if (checkHorizontalWin(initialBoard, playerTurn)||  
            checkVerticalWin(initialBoard,playerTurn)||
            checkDiagonalWin1(initialBoard,playerTurn)||
            checkDiagonalWin2(initialBoard,playerTurn) == true){
            //congratulatory statement outputted 
            cout << "Game is Over, Player " << playerTurn << " got 4 in a row!!!! " << endl;
            break; // break out of the loop, no more gameplay 
        }
        else if (boardFull(initialBoard) == true){ // when condition for a draw between players is detected
            // tie, no winner, statement outputted 
            cout << "Board is Full, It's a Draw!!!" << endl;
            break;// break out of the loop, no more gameplay 
        }
        else { // switching turns so gameplay can continue 
            //When the current player is X is true, it switches to O. If it is false, then it stays X 
            playerTurn = (playerTurn == 'X') ? 'O' : 'X'; 
        }
    }
    // indicated game has ended 
    cout << endl << "Ending Game";
    return 0; 
}
