#include <stdio.h>
#include <stdlib.h>

//el headers bwzo el dnya 5ales fa sheltohom w ktbt kolo fel main

//function to print the grid initiated from grid();

void print_grid(int rows, int cols, int grid[rows + 1][cols + 1]){
    system("cls");
    for(int i = 0; i < rows + 1; ++i){
        for(int j = 0; j < cols + 1; ++j){
            if(i == rows && j == cols){
                break;
            }
            if(i == rows || j == cols){
                printf("%d ", grid[i][j]);
                continue;
            }
            printf("% c ", grid[i][j]);
        }
        printf("\n");
    }
}

//grid initiation function (game loop included)

void grid(int rows, int cols){
    //initiating the grid
    int grid[rows + 1][cols + 1];
    //declaring the empty spaces
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            grid[i][j] = ' ';
        }
    }
    //declaring the dots
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            if(i % 2 == 0 && j % 2 == 0){
                grid[i][j] = 254; //ascii code for the dots used in the project
            }
        }
    }
    //declaring side numbers
    int numbers = 0;
    for(int j = 0; j < cols; ++j){
        grid[rows][j] = numbers++;
    }
    numbers = 0;
    for(int i = 0; i < rows; ++i){
        grid[i][cols] = numbers++;
    }
    //calling the grid printing function
    print_grid(rows, cols, grid);
    //game loop
    int x, y;
    int game_on = 1;
    while(game_on){
        printf("\nDraw a line at row: ");
        scanf("%d", &x);
        printf("and column: ");
        scanf("%d", &y);
        //check if a line already exists
        if(grid[x][y] == 124 || grid[x][y] == 196){
            print_grid(rows, cols, grid);
            printf("\nAlready occupied, please enter a valid input");
            continue;
        }
        //check if the line to be printed is horizontal or vertical
        if(x % 2 == 0 && y % 2 != 0 && x < rows && y < cols){
            grid[x][y] = 196;
        }else if(y % 2 == 0 && x % 2 != 0 && x < rows && y < cols){
            grid[x][y] = 124;
        }else{ //if the input is not suitable, the following executes
            print_grid(rows, cols, grid);
            printf("\nPlease, enter a valid input");
            continue;
        }
        print_grid(rows, cols, grid);
    }
}

//function that handles invalid inputs

void wrong_selection(){
    system("cls");
    printf("\nPlease, enter a valid input\n");
}

//game mode selection (work in progress)

void game_mode(char mode_selection){
    printf("\n%c PVP (Press 1)\n%c PVE (Press 2)\n\n", 16, 16);
    scanf("%d", &mode_selection);
    switch(mode_selection){
        case 1:
            system("cls");
            break;
        case 2:
            system("cls");
            break;
        default:
            wrong_selection();
            game_mode(mode_selection);
            break;
    }
}

//3x3 or 5x5 (more could be added later)

void difficulity(char difficulity_selection){
    printf("\n%c Beginner (Press 1)\n%c Expert (Press 2)\n\n", 16, 16);
    scanf("%d", &difficulity_selection);
    switch(difficulity_selection){
        case 1:
            system("cls");
            grid(5, 5);
            break;
        case 2:
            system("cls");
            grid(11, 11);
            break;
        default:
            wrong_selection();
            difficulity(difficulity_selection);
            break;
    }
}

//main menu function (work in progress)

void menu(){
    printf("\n%c Start Game (Press 1)\n%c Load Game (Press 2)\n%c Top 10 Players (Press 3)\n%c Exit (Press 4)\n\n", 16, 16, 16, 16);
        char first_selection, difficulity_selection, mode_selection;
        scanf("%d", &first_selection);
        switch(first_selection){
            case 1:
                system("cls");
                game_mode(mode_selection);
                difficulity(difficulity_selection);
                break;
            case 2:
                system("cls");
                printf("\nChoose the saved game to load: \n\n");
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                system("cls");
                printf("\nPlease, type a valid input\n\n");
                menu();
                break;
        }
}

int main(){

    menu();

    return 0;
}
