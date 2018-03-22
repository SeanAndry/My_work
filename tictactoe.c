/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

typedef enum {top_left = 1, top_middle, top_right,
            middle_left, middle, middle_right,
            bottom_left, bottom_middle, bottom_right} position_t;
typedef enum {FALSE, TRUE} bool_t;
typedef enum {empty, x, o} state_t;
typedef enum {one = 1, two} players_t;

#define MAX_NUM_PLAYS 9
static state_t board[3][3] = {
    {empty,empty,empty},
    {empty,empty,empty},
    {empty,empty,empty}};
//static bool_t is_won = FALSE;

const char *player_strings[] = {" ", "x", "o"};
  
static bool_t place_move(position_t position, state_t player);
static state_t game_over(void);
static void show_board(void);
static void show_explination(void);
static void one_player(void);
static void two_player(void);
static position_t smart_move(void);

int main()
{
    srand(time(0)); //seeds random number generator
    show_explination();//describes how to play
    for(players_t num_players = 0; &num_players != 1 || &num_players != 2; )
    {
        printf("How many players are there? (eiher 1 or 2)\n");
        scanf("%u", &num_players);
    
        if(num_players == one)
        {
            one_player();
            break;
        }
        if(num_players == two)
        {
            two_player();
            break;
        }
    }
    
    if(game_over() == empty )
    {
        //the game was a tie
        printf("\nThe game is over! It was a draw.\n");
        show_board();//shows final game
    }
    
    return 0;
}

void one_player(void)
{
   int position;
    for(int i = 0; i < MAX_NUM_PLAYS; )
    {
        //check if game has been won yet
        if(game_over() == x )
        {
            printf("\nThe game is over! You won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
        else if(game_over() == o )
        {
            printf("\nThe game is over! The computer won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
        else if (i % 2 == 0) //human player move
        {
            show_board();
            printf("Your turn, pick a position: ");
            scanf("%u", &position);
            if(place_move((position_t) position, i % 2 == 0 ? x : o))
                i++;
        }
        else //computer player move
        {
            show_board();
            printf("Computer's turn... ");
            sleep( 1 );
            position = smart_move();
            if(place_move((position_t) position, i % 2 == 0 ? x : o))
                i++;
        }
        
        //checks for win on last move
        if(game_over() == x )
        {
            printf("\nThe game is over! You won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
        else if(game_over() == o )
        {
            printf("\nThe game is over! The computer won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
    }
}

position_t smart_move(void)
{
    //checks to defend from horizontal wins
    //defends against the top row
    if(board[0][0] == x && board[0][1] == x && board[0][2] == empty)
    {
        return top_right;
    }
    if(board[0][0] == x && board[0][2] == x && board[0][1] == empty)
    {
        return top_middle;
    }
    if(board[0][1] == x && board[0][2] == x && board[0][2] == empty)
    {
        return top_left;
    }
    //defends against the middle row
    if(board[1][0] == x && board[1][1] == x && board[1][2] == empty)
    {
        return middle_right;
    }
    if(board[1][0] == x && board[1][2] == x && board[1][1] == empty)
    {
        return middle;
    }
    if(board[1][1] == x && board[1][2] == x && board[1][0] == empty)
    {
        return middle_left;
    }
    //defends against bottom row
    if(board[2][0] == x && board[2][1] == x && board[2][2] == empty)
    {
        return bottom_right;
    }
    if(board[2][0] == x && board[2][2] == x && board[2][1] == empty)
    {
        return bottom_middle;
    }
    if(board[2][1] == x && board[2][2] == x && board[2][0] == empty)
    {
        return bottom_left;
    }
    
    //checks to defend against vertical wins
    //defends against left side wins
    if(board[0][0] == x && board[1][0] == x  && board[2][0] == empty)
    {
        return bottom_left;
    }
    if(board[0][0] == x && board[2][0] == x && board[1][0] == empty)
    {
        return middle_left;
    }
    if(board[1][0] == x && board[2][0] == x && board[0][0] == empty)
    {
        return top_left;
    }
    //defends against middle row wins
    if(board[0][1] == x && board[1][1] == x && board[2][1] == empty)
    {
        return bottom_middle;
    }
    if(board[0][1] == x && board[2][1] == x && board[1][1] == empty)
    {
        return middle;
    }
    if(board[1][1] == x && board[2][1] == x && board[0][1] == empty)
    {
        return top_middle;
    }
    //defends against right side wins
    if(board[0][2] == x && board[1][2] == x && board[2][2] == empty)
    {
        return bottom_right;
    }
    if(board[0][2] == x && board[2][2] == x && board[1][2] == empty)
    {
        return middle_right;
    }
    if(board[1][2] == x && board[2][2] == x && board[0][2] == empty)
    {
        return top_right;
    }
    
    //checks for diagonal wins
    if(board[0][0] == x && board[1][1] == x && board[2][2] == empty)
    {
        return bottom_right;
    }
    if(board[0][0] == x && board[2][2] == x && board[1][1] == empty)
    {
        return middle;
    }
    if(board[1][1] == x && board[2][2] == x && board[0][0] == empty)
    {
        return top_left;
    }
    
    if(board[2][0] == x && board[1][1] == x && board[0][2] == empty)
    {
        return top_right;
    }
    if(board[2][0] == x && board[0][2] == x && board[1][1] == empty)
    {
        return middle;
    }
    if(board[0][2] == x && board[1][1] == x && board[2][0] == empty)
    {
        return bottom_left;
    }
    if(board[1][1] == empty )
    {
        return middle;
    }
    
    //other cases
    if (board[0][2] == x && board[2][0] == empty && board[2][0] == empty)
    {
        return bottom_left;
    }
    if (board[0][2] == x && board[2][0] == x && board[0][1] == empty)
    {
        return top_middle;
    }
    if (board[0][0] == x && board[2][2] == empty && board[2][0] == empty)
    {
        return bottom_left;
    }
    if (board[0][0] == x && board[2][2] == x && board[0][1] == empty)
    {
        return top_middle;
    }
    else
    {
        return rand() % 8;
    }
}

void two_player(void)
{
    int position;
    for(int i = 0; i < MAX_NUM_PLAYS; )
    {
        //check if game has been won yet
        if(game_over() == x )
        {
            printf("\nThe game is over! Player 1 has won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
        else if(game_over() == o )
        {
            printf("\nThe game is over! Player 2 has won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
        else
        {
            show_board();
            printf("Player %u, pick a position: ", i % 2 == 0 ? 1 : 2);
            scanf("%u", &position);
            if(place_move((position_t) position, i % 2 == 0 ? x : o))
                i++;
        }
        //checks for win on the last move
        if(game_over() == x )
        {
            printf("\nThe game is over! Player 1 has won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
        else if(game_over() == o )
        {
            printf("\nThe game is over! Player 2 has won!\n");
            show_board();//shows final game
            i = MAX_NUM_PLAYS;
        }
    }
}

bool_t place_move(position_t position, state_t player)
{
    bool_t result = TRUE;
    switch(position)
        {
        case 1 :
            if(board[0][0] == empty)
            {
                board[0][0] = player;
            }
            else
            {
                result = FALSE;
            }
        break;
           
        case 2 :
            if(board[0][1] == empty)
            {
                board[0][1] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 3 :
            if(board[0][2] == empty)
            {
                board[0][2] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 4 :
            if(board[1][0] == empty)
            {
                board[1][0] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 5 :
            if(board[1][1] == empty)
            {
                board[1][1] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 6 :
            if(board[1][2] == empty)
            {
                board[1][2] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 7 :
            if(board[2][0] == empty)
            {
                board[2][0] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 8 :
            if(board[2][1] == empty)
            {
                board[2][1] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        case 9 :    
            if(board[2][2] == empty)
            {
                board[2][2] = player;
            }
            else
            {
                result = FALSE;
            }
            break;
        default :
        result = FALSE;
       }
    return result;
}

state_t game_over(void)
{
    //checks for horizontal wins
    if(board[0][0] == x && board[0][1] == x && board[0][2] == x )
    {
        return x;
    }
    if(board[1][0] == x && board[1][1] == x && board[1][2] == x )
    {
        return x;
    }
    if(board[2][0] == x && board[2][1] == x && board[2][2] == x )
    {
        return x;
    }
    if(board[0][0] == o && board[0][1] == o && board[0][2] == o )
    {
        return o;
    }
    if(board[1][0] == o && board[1][1] == o && board[1][2] == o )
    {
        return o;
    }
    if(board[2][0] == o && board[2][1] == o && board[2][2] == o )
    {
        return o;
    }
    //checks for vertical wins
    if(board[0][0] == x && board[1][0] == x && board[2][0] == x )
    {
        return x;
    }
    if(board[0][1] == x && board[1][1] == x && board[2][1] == x )
    {
        return x;
    }
    if(board[0][2] == x && board[1][2] == x && board[2][2] == x )
    {
        return x;
    }
    if(board[0][0] == o && board[1][0] == o && board[2][0] == o )
    {
        return o;
    }
    if(board[0][1] == o && board[1][1] == o && board[2][1] == o )
    {
        return o;
    }
    if(board[0][2] == o && board[1][2] == o && board[2][2] == o )
    {
        return o;
    }
    //checks for diagonal wins
    if(board[0][0] == x && board[1][1] == x && board[2][2] == x )
    {
        return x;
    }
    if(board[2][0] == x && board[1][1] == x && board[0][2] == x )
    {
        return x;
    }
    if(board[0][0] == o && board[1][1] == o && board[2][2] == o )
    {
        return o;
    }
    if(board[2][0] == o && board[1][1] == o && board[0][2] == o )
    {
        return o;
    }
    else
    {
        return empty;
    }
}

void show_board(void)
{
    printf("The board looks like: \n");
    
    printf(" %c | %c | %c \n", board[0][0] == empty ? ' ' : board[0][0] == o ? 'o' : 'x',
            board[0][1] == empty ? ' ' : board[0][1] == o ? 'o' : 'x',
            board[0][2] == empty ? ' ' : board[0][2] == o ? 'o' : 'x');
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[1][0] == empty ? ' ' : board[1][0] == o ? 'o' : 'x',
            board[1][1] == empty ? ' ' : board[1][1] == o ? 'o' : 'x',
            board[1][2] == empty ? ' ' : board[1][2] == o ? 'o' : 'x');
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[2][0] == empty ? ' ' : board[2][0] == o ? 'o' : 'x',
            board[2][1] == empty ? ' ' : board[2][1] == o ? 'o' : 'x',
            board[2][2] == empty ? ' ' : board[2][2] == o ? 'o' : 'x');
    printf("\n");
}

void show_explination(void)
{
    printf("The rules of the game are simple, get three in a row \n");
    printf("to place a marker in a position, follow the guide below \n");
    
    printf(" %u | %u | %u \n", 1, 2, 3);
    printf("-----------\n");
    printf(" %u | %u | %u \n", 4, 5, 6);
    printf("-----------\n");
    printf(" %u | %u | %u \n", 7, 8, 9);
    printf("\n");
}




