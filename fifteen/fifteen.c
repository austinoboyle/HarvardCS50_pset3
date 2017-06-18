/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
int * find(int array[d][d], int size, int value);
void clear(void);
void greet(void);
void init(int array[d][d], int size);
void draw(int array[d][d], int size);
bool move(int array[d][d], int size, int tile);
bool won(int array[d][d], int size);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }
    printf("Dimensions are: %i x %i\n",d,d);
    // greet user with instructions
    greet();
    
    
    // initialize the board
    int board[d][d];
    init(board, d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(board, d);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(board, d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(board, d , tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int array[d][d], int size)
{
    for (int row = 0; row < size ; row++){
        for (int col = 1; col <= size; col++){
            array[row][col-1] = size * size - (col + row * size);
        }
        
    }
    if (size % 2 == 0){
        array[size-1][size-2] = 2;
        array[size-1][size-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(int array[d][d], int size)
{
    for (int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            if (array[row][col] == 0){
                printf("  _ ");
            }
            else{
                printf(" %2i ", array[row][col]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int array[d][d], int size, int tile)
{
    int *tilePos;
    int *emptyPos;
    
    tilePos = find(array, size, tile);
    int tRow = tilePos[0];
    int tCol = tilePos[1];
    
    emptyPos = find(array, size, 0);
    int eRow = emptyPos[0];
    int eCol = emptyPos[1];
    
    if (tRow == -1){
        return false;
    }
    
    if (abs(tRow - eRow) + abs(tCol - eCol) < 2){
        int temp = array[tRow][tCol];
        array[tRow][tCol] = array[eRow][eCol];
        array[eRow][eCol] = temp;
        return true;
    }
    
    return false;
    
}

/**
 * Checks if a value is 
 */
int * find(int array[d][d], int size, int value)
{
    static int returnArray[2];
    for (int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            if (array[row][col] == value){
                returnArray[0] = row;
                returnArray[1] = col;
                return returnArray;
            }
        }
    }
    returnArray[0] = -1;
    returnArray[1] = -1;
    return returnArray;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int array[d][d], int size)
{
    if (array[size-1][size-1] != 0){
        return false;
    }
    for (int row = 0; row < size; row++){
        for (int col = 1; col <= size; col++){
            if (row == size-1 && col == size){
                return true;
            }
            else if (array[row][col-1] != (col + row * size)){
                return false;
            }
        }
    }
    return true;
}