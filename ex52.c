
 /******************************************
* Name: Coral Malachi
* ID: 314882853
* Course Exercise Group: 07
* Exercise name: ex52
******************************************/

#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#include <stdlib.h>

#define QUIT_GAME 'q'
#define VERTICAL_STATUS 1
#define TURN_LEFT 'a'
#define TURN_RIGHT 'd'
#define GO_DOWN 's'
#define TRANSPOSE 'w'
#define SIZE_OF_BOARD_GAME 20

#define LIE_STATUS 0
#define STAR '*'
#define EMPTY ' '
#define MINUS '-'

#define YES 1
#define NO 0


typedef struct Play_Board
{
    //the position of the shape in the board
    int m_x,m_y;
    //the status of the shape -
    int m_status;
    //the matrix 20x20 represent the play board
    char m_play_board[SIZE_OF_BOARD_GAME][SIZE_OF_BOARD_GAME];

}Play_Board_t;

//init the board
 Play_Board_t play_board;



 //forward delecrations
 void delete_shape_from_board(Play_Board_t* board);
 void turn_left(Play_Board_t* board);
 void make_step(int m_signal);
 void turn_right(Play_Board_t* board);
 void go_down(Play_Board_t* board);
 void transpose(Play_Board_t* board);
 void Perform_user_request(int m_signal);
 void make_step(int m_signal);
 void new_line();
 int check_Stuck_in_the_wall(int k);
 void show_play_board(Play_Board_t* board);
 void init_the_play_board(Play_Board_t* board);
 int check_if_reallocation_is_valid(int new_y,Play_Board_t* board);
 void reallocation_of_shape(int new_x, int new_y,int is_shape_need_delete,Play_Board_t* board);
 void update_shape_now(int x, int y,Play_Board_t* board);
 int trans_valid(Play_Board_t* board);

 int main()
 {
     init_the_play_board(&play_board);
     show_play_board(&play_board);
     signal(SIGALRM,make_step);
     alarm(1);
     signal(SIGUSR2,Perform_user_request);
     while (1)
         pause();
 }

 /***********************************************************************
*Function Name:turn_left
*Function input:pointer to the board
*Function output:none
*Function Action:the function move the shape left
***********************************************************************/
void turn_left(Play_Board_t* board)
{
    int y_left = board->m_y -1;
    reallocation_of_shape(board->m_x,y_left,YES,board);


}

 /***********************************************************************
*Function Name:turn_right
*Function input:pointer to the board
*Function output:none
*Function Action:the function move the shape right
***********************************************************************/

 void turn_right(Play_Board_t* board)
 {
     int y_right = board->m_y +1;
     reallocation_of_shape(board->m_x,y_right,YES,board);

 }

 /***********************************************************************
*Function Name:go_down
*Function input:pointer to the board
*Function output:none
*Function Action:the function move the shape down
***********************************************************************/

 void go_down(Play_Board_t* board)
 {
     int x_down = board->m_x +1;
     reallocation_of_shape(x_down,board->m_y,YES,board);
 }
 /***********************************************************************
*Function Name:transpose
*Function input:pointer to the board
*Function output:none
*Function Action:the function transpose the shape
***********************************************************************/


 void transpose(Play_Board_t* board)
 {
     int x=board->m_x;
     int y = board->m_y;
     int new_x, new_y;
     if (board->m_status == VERTICAL_STATUS)
     {
         new_x = x + 1;
         new_y = y -1;
         //check we did not get to borders
         if (new_y < 1)
             new_y=1;
         else if(new_y >= SIZE_OF_BOARD_GAME -3)
             new_y = SIZE_OF_BOARD_GAME -4;
         board->m_status = LIE_STATUS;
         reallocation_of_shape(new_x,new_y,NO,board);
     }
     //LIE_STATUS
     else
     {
        new_x = x -1;
        new_y = y +1;
        board->m_status = VERTICAL_STATUS;
        reallocation_of_shape(new_x,new_y,NO,board);
     }

 }
 /***********************************************************************
*Function Name:trans_valid
*Function input:pointer to the board
*Function output:0 or 1
*Function Action:the function check if  the new positions
  * are valid. if its valid return 1.else return0
***********************************************************************/
 int trans_valid(Play_Board_t* board)
 {
     int x = board->m_x;
     if(board->m_status == LIE_STATUS && (x >=1 && x<=18) )
         return 1;
     else if (board->m_status == VERTICAL_STATUS)
         return 1;
     else
         return 0;


 }
 /***********************************************************************
*Function Name:Perform_user_request
*Function input:
*Function output:none
*Function Action:according to the user selection we go to the required direction
***********************************************************************/
 void Perform_user_request(int m_signal)
 {
     signal(SIGUSR2,Perform_user_request);
     char user_input;
     user_input = (char)getchar();
     if (user_input == QUIT_GAME)
     {
         //exit program
         exit(1);
     } else if (user_input == TURN_LEFT)
         turn_left(&play_board);
     else if(user_input == TURN_RIGHT)
         turn_right(&play_board);
     else if (user_input == GO_DOWN)
         go_down(&play_board);
     else if (user_input == TRANSPOSE && trans_valid(&play_board))
     {
             delete_shape_from_board(&play_board);
             transpose(&play_board);
     }
     show_play_board(&play_board);
 }

 /***********************************************************************
*Function Name:make_step
*Function input:
*Function output:none
*Function Action:the function force the shape go down each second
***********************************************************************/
 void make_step(int m_signal)
 {
     signal(SIGALRM,make_step);
     //make step each second
     alarm(1);

     int x_down = 1+ play_board.m_x;//update x position
     reallocation_of_shape(x_down,play_board.m_y,YES,&play_board);
     //show the board after update
     show_play_board(&play_board);
 }
 /***********************************************************************
*Function Name:new_line
*Function input:none
*Function output:none
*Function Action:print new line
***********************************************************************/
 void new_line()
 {
     printf("\n");
 }

 /***********************************************************************
*Function Name:show_play_board
*Function input:pointer to the board
*Function output:none
*Function Action:print on the screen the board
***********************************************************************/

 void show_play_board(Play_Board_t* board)
 {
     int k;
     int i;
     //clear the terminal screen before starting the game
     system("clear");
     for (i=0;i<SIZE_OF_BOARD_GAME;++i)
     {
         for (k=0;k<SIZE_OF_BOARD_GAME;++k)
         {
             printf("%c",board->m_play_board[i][k]);
         }
         new_line();
     }
 }

 /***********************************************************************
*Function Name:check_Stuck_in_the_wall
*Function input:k
*Function output::0 or 1
*Function Action:the function check we didnt reach the borders
  * if we did return 1.else return0
***********************************************************************/


int check_Stuck_in_the_wall(int k)
{
    if (k == SIZE_OF_BOARD_GAME -1
            || k==0)
        return 1;
    return 0;
}

 /***********************************************************************
*Function Name:init_the_play_board
*Function input:pointer to the board
*Function output:none
*Function Action:this function create an initial board and print it on screen
***********************************************************************/

void init_the_play_board(Play_Board_t* board)
{
    int i,k;
    board->m_x = 0;
    board->m_status = LIE_STATUS;
    i = -1 + (SIZE_OF_BOARD_GAME)/2;
    board->m_y = i;
    i=0;
    //create the bottom border of the play board with stars
    for (k=0;k<SIZE_OF_BOARD_GAME ; ++k)
    {
        board->m_play_board[SIZE_OF_BOARD_GAME -1][k] = STAR;
    }
    //fill inner board
    for (i;i<SIZE_OF_BOARD_GAME-1;++i)
    {
        for (k=0;k<SIZE_OF_BOARD_GAME;++k)
        {
            if(check_Stuck_in_the_wall(k))
                board->m_play_board[i][k] = STAR;
            else
                board->m_play_board[i][k] = EMPTY;
        }
    }
    reallocation_of_shape(board->m_x,board->m_y,YES,board);

}

 /***********************************************************************
*Function Name:check_if_reallocation_is_valid
*Function input:pointer to the board
*Function output:0 or 1
*Function Action:the function check if we need to check if the new positions
  * are valid. if its valid return 1.else return0
***********************************************************************/

int check_if_reallocation_is_valid(int new_y,Play_Board_t* board)
{
    if (board->m_status == VERTICAL_STATUS)
    {
        if (new_y >= SIZE_OF_BOARD_GAME -1 ||
                new_y<=0)
            return 0;
    }
    if (board->m_status == LIE_STATUS)
    {
        //each sahpe has 3 *** : y+2 >=20-1
        if (new_y >=SIZE_OF_BOARD_GAME-3 ||
                new_y<=0)
            return 0;
    }
    return 1;
}

 /***********************************************************************
*Function Name:reallocation_of_shape
*Function input:pointer to the board,new x and y
*Function output:none
*Function Action:the function first check if we need to check if the new positions
  * are valid, than we delete the current shape and create a new one
***********************************************************************/

void reallocation_of_shape(int new_x, int new_y,int is_shape_need_delete,Play_Board_t* board)
{
    if (check_if_reallocation_is_valid(new_y,board) == 0)
        return;
    //if the sahpe was not plifed - delete
    if(is_shape_need_delete == YES)
        delete_shape_from_board(board);
    board->m_y = new_y;
    board->m_x = new_x;
    //call help function to complete the action
    update_shape_now(new_x,new_y,board);
}

 /***********************************************************************
*Function Name:update_shape_now
*Function input:pointer to the board , new x and y
*Function output:none
*Function Action: preform the moving action. Consider the different cases
  * :VERTICAL_STATUS v.s. LIE_STATUS
***********************************************************************/

void update_shape_now(int x, int y,Play_Board_t* board)
{
    if (board->m_status == VERTICAL_STATUS)
    {
        board->m_play_board[x][y] = MINUS;
        board->m_play_board[x+1][y] = MINUS;
        board->m_play_board[x+2][y] = MINUS;

        //check limit
        if (SIZE_OF_BOARD_GAME -1 <= x+2)
            //game over- start again
            init_the_play_board(board);
    } else
    {
        board->m_play_board[x][y] = MINUS;
        board->m_play_board[x][y+1] = MINUS;
        board->m_play_board[x][y+2] = MINUS;
        if (SIZE_OF_BOARD_GAME - 1 <= x)
            init_the_play_board(board);
    }
}

 /***********************************************************************
*Function Name:delete_shape_from_board
*Function input:pointer to the board
*Function output:none
*Function Action:the function delete the shape form the screen
***********************************************************************/

void delete_shape_from_board(Play_Board_t* board)
{
    int x,y;
    //save x and y as varibles and save many calls
    x=board->m_x;
    y=board->m_y;
    if (board->m_status == VERTICAL_STATUS)
    {
        board->m_play_board[x][y]=EMPTY;
        board->m_play_board[x+1][y]=EMPTY;
        board->m_play_board[x+2][y] = EMPTY;
    }
    //the case of lie
    else
        {
            board->m_play_board[x][y]=EMPTY;
            board->m_play_board[x][y+1]=EMPTY;
            board->m_play_board[x][y+2] = EMPTY;
    }
}