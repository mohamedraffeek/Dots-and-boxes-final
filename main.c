#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//el headers bwzo el dnya 5ales fa sheltohom w ktbt kolo fel main

//structs

typedef struct{
    int score;
    char name[200];
    int turn;
    char symbol;
} player;


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
    //score calculations
    int boxes_center_coordinates[(rows / 2) * (rows / 2)][2];
    int boxes_sides_coordinates[4 * (rows / 2) * (rows / 2)][2];
    int complete_box[(rows / 2) * (rows / 2)];
    int center_counter = 0, side_counter = 0, boxes_counter = 0, side_index_counter = 0;
    for(int i = 1; i < rows; i += 2){
        for(int j = 1; j < rows; j += 2){
            boxes_center_coordinates[center_counter][0] = i;
            boxes_center_coordinates[center_counter][1] = j;
            center_counter++;
        }
    }
    for(int i = 1; i < rows; i += 2){
        for(int j = 1; j < rows; j += 2){
            for(int k = 0; k < 4; ++k){
                boxes_sides_coordinates[side_counter][0] = boxes_center_coordinates[boxes_counter][0];
                boxes_sides_coordinates[side_counter][1] = boxes_center_coordinates[boxes_counter][1];
                side_counter++;
                if(k == 0){
                    boxes_sides_coordinates[side_index_counter++][0]--;
                }else if(k == 1){
                    boxes_sides_coordinates[side_index_counter++][1]--;
                }else if(k == 2){
                    boxes_sides_coordinates[side_index_counter++][0]++;
                }else if(k == 3){
                    boxes_sides_coordinates[side_index_counter++][1]++;
                }
            }
            boxes_counter++;
        }
    }
    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
        complete_box[i] = 0;
    }
    //game loop
    int x, y;
    int game_on = 1;
    player player1, player2;
    player1.symbol = 'A';
    player2.symbol = 'B';
    player1.turn = 1;
    player2.turn = 0;
    player1.score = 0;
    player2.score = 0;
    //defining undo/redo struct
    typedef struct{
        int grid_state[rows + 1][rows + 1];
        int score1;
        int score2;
        int turn1;
        int turn2;
        int complete_box_state[(rows / 2) * (rows / 2)];
    } game_state;
    game_state current_game_state[2*(rows/2)*(rows/2+1)];
    current_game_state[0].score1 = player1.score;
    current_game_state[0].score2 = player2.score;
    current_game_state[0].turn1 = player1.turn;
    current_game_state[0].turn2 = player2.turn;
    for(int i = 0; i < rows + 1; ++i){
        for(int j = 0; j < rows + 1; ++j){
            current_game_state[0].grid_state[i][j] = grid[i][j];
        }
    }
    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
        current_game_state[0].complete_box_state[i] = complete_box[i];
    }
    int scored = 0, game_state_counter = 0, redoable = 0;
    while(game_on){
        if(player1.turn){
            printf("\nPlayer One's turn");
            printf("\nPlayer One's score: %d", player1.score);
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
            //check if undo input is received
            if(x == -1 && y == -1){
                if(game_state_counter == 0){
                    print_grid(rows, cols, grid);
                    printf("\nNo previous plays to undo");
                    continue;
                }else{
                    game_state_counter--;
                    player1.score = current_game_state[game_state_counter].score1;
                    player2.score = current_game_state[game_state_counter].score2;
                    player1.turn = current_game_state[game_state_counter].turn1;
                    player2.turn = current_game_state[game_state_counter].turn2;
                    for(int i = 0; i < rows + 1; ++i){
                        for(int j = 0; j < rows + 1; ++j){
                            grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                        }
                    }
                    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                        complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                    }
                    redoable++;
                    print_grid(rows, cols, grid);
                    continue;
                }
            }
            //check if redo input is received
            if(x == -2 && y == -2){
                if(!redoable){
                    print_grid(rows, cols, grid);
                    printf("\nNo plays registered to redo");
                    continue;
                }else if(redoable){
                    game_state_counter++;
                    player1.score = current_game_state[game_state_counter].score1;
                    player2.score = current_game_state[game_state_counter].score2;
                    player1.turn = current_game_state[game_state_counter].turn1;
                    player2.turn = current_game_state[game_state_counter].turn2;
                    for(int i = 0; i < rows + 1; ++i){
                        for(int j = 0; j < rows + 1; ++j){
                            grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                        }
                    }
                    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                        complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                    }
                    redoable--;
                    print_grid(rows, cols, grid);
                    continue;
                }
            }
            //check if the line to be printed is horizontal or vertical
            if(x % 2 == 0 && y % 2 != 0 && x < rows && y < cols){
                grid[x][y] = 196;
                game_state_counter++;
                redoable = 0;
            }else if(y % 2 == 0 && x % 2 != 0 && x < rows && y < cols){
                grid[x][y] = 124;
                game_state_counter++;
                redoable = 0;
            }else{ //if the input is not suitable, the following executes
                print_grid(rows, cols, grid);
                printf("\nPlease, enter a valid input");
                continue;
            }
            for(int i = 0; i < (4 * (rows / 2) * (rows / 2)); ++i){
                int side_increment = 0;
                for(int j = 0; j < 2; ++j){
                    if(x == boxes_sides_coordinates[i][j] && side_increment == 0){
                        side_increment++;
                    }else if(y == boxes_sides_coordinates[i][j] && side_increment == 1){
                        side_increment++;
                    }
                }
                if(side_increment == 2){
                    complete_box[i / 4]++;
                    if(complete_box[i / 4] == 4){
                        player1.score++;
                        scored = 1;
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = player1.symbol;
                    }
                }
            }
            if(scored){
                scored = 0;
                Beep(750, 300);
                player1.turn = 1;
                player2.turn = 0;
                goto s1;
            }
            player1.turn = 0;
            player2.turn = 1;
            s1:
            current_game_state[game_state_counter].score1 = player1.score;
            current_game_state[game_state_counter].score2 = player2.score;
            current_game_state[game_state_counter].turn1 = player1.turn;
            current_game_state[game_state_counter].turn2 = player2.turn;
            for(int i = 0; i < rows + 1; ++i){
                for(int j = 0; j < rows + 1; ++j){
                    current_game_state[game_state_counter].grid_state[i][j] = grid[i][j];
                }
            }
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                current_game_state[game_state_counter].complete_box_state[i] = complete_box[i];
            }
            print_grid(rows, cols, grid);
        }else if(player2.turn){
            printf("\nPlayer Two's turn");
            printf("\nPlayer Two's score: %d", player2.score);
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
            //check if undo input is received
            if(x == -1 && y == -1){
                if(game_state_counter == 0){
                    print_grid(rows, cols, grid);
                    printf("\nNo previous plays to undo");
                    continue;
                }else{
                    game_state_counter--;
                    player1.score = current_game_state[game_state_counter].score1;
                    player2.score = current_game_state[game_state_counter].score2;
                    player1.turn = current_game_state[game_state_counter].turn1;
                    player2.turn = current_game_state[game_state_counter].turn2;
                    for(int i = 0; i < rows + 1; ++i){
                        for(int j = 0; j < rows + 1; ++j){
                            grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                        }
                    }
                    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                        complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                    }
                    redoable++;
                    print_grid(rows, cols, grid);
                    continue;
                }
            }
            //check if redo input is received
            if(x == -2 && y == -2){
                if(!redoable){
                    print_grid(rows, cols, grid);
                    printf("\nNo plays registered to redo");
                    continue;
                }else if(redoable){
                    game_state_counter++;
                    player1.score = current_game_state[game_state_counter].score1;
                    player2.score = current_game_state[game_state_counter].score2;
                    player1.turn = current_game_state[game_state_counter].turn1;
                    player2.turn = current_game_state[game_state_counter].turn2;
                    for(int i = 0; i < rows + 1; ++i){
                        for(int j = 0; j < rows + 1; ++j){
                            grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                        }
                    }
                    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                        complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                    }
                    redoable--;
                    print_grid(rows, cols, grid);
                    continue;
                }
            }
            //check if the line to be printed is horizontal or vertical
            if(x % 2 == 0 && y % 2 != 0 && x < rows && y < cols){
                grid[x][y] = 196;
                game_state_counter++;
                redoable = 0;
            }else if(y % 2 == 0 && x % 2 != 0 && x < rows && y < cols){
                grid[x][y] = 124;
                game_state_counter++;
                redoable = 0;
            }else{ //if the input is not suitable, the following executes
                print_grid(rows, cols, grid);
                printf("\nPlease, enter a valid input");
                continue;
            }
            for(int i = 0; i < (4 * (rows / 2) * (rows / 2)); ++i){
                int side_increment = 0;
                for(int j = 0; j < 2; ++j){
                    if(x == boxes_sides_coordinates[i][j] && side_increment == 0){
                        side_increment++;
                    }else if(y == boxes_sides_coordinates[i][j] && side_increment == 1){
                        side_increment++;
                    }
                }
                if(side_increment == 2){
                    complete_box[i / 4]++;
                    if(complete_box[i / 4] == 4){
                        player2.score++;
                        scored = 1;
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = player2.symbol;
                    }
                }
            }
            if(scored){
                scored = 0;
                Beep(750, 300);
                player1.turn = 0;
                player2.turn = 1;
                goto s2;
            }
            player1.turn = 1;
            player2.turn = 0;
            s2:
            current_game_state[game_state_counter].score1 = player1.score;
            current_game_state[game_state_counter].score2 = player2.score;
            current_game_state[game_state_counter].turn1 = player1.turn;
            current_game_state[game_state_counter].turn2 = player2.turn;
            for(int i = 0; i < rows + 1; ++i){
                for(int j = 0; j < rows + 1; ++j){
                    current_game_state[game_state_counter].grid_state[i][j] = grid[i][j];
                }
            }
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                current_game_state[game_state_counter].complete_box_state[i] = complete_box[i];
            }
            print_grid(rows, cols, grid);
        }
        if(player1.score + player2.score == (rows / 2) * (rows / 2)){
            game_on = 0;
        }
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
            printf("\nGame over!");
            break;
        case 2:
            system("cls");
            grid(11, 11);
            printf("\nGame over!");
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
