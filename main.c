#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


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
    printf("\n\n");
    for(int i = 0; i < rows + 1; ++i){
        for(int j = 0; j < cols + 1; ++j){
            if(i == rows && j == cols){
                break;
            }
            if(j == 0){
                printf("\t\t\t\t\t\t  ");
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

void grid(int rows, int cols, int mode){
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
    char ai_symbol = 'X';
    player1.turn = 1;
    player2.turn = 0;
    int ai_turn = 0;
    player1.score = 0;
    player2.score = 0;
    int ai_score = 0;
    //defining the struct that will carry the game state
    //it will be used to execute the undo and the redo functions
    typedef struct{
        int grid_state[rows + 1][rows + 1];
        int score1;
        int score2;
        int score3;
        int turn1;
        int turn2;
        int turn3;
        int complete_box_state[(rows / 2) * (rows / 2)];
    } game_state;
    game_state current_game_state[2*(rows/2)*(rows/2+1)];
    current_game_state[0].score1 = player1.score;
    current_game_state[0].score2 = player2.score;
    current_game_state[0].score3 = ai_score;
    current_game_state[0].turn1 = player1.turn;
    current_game_state[0].turn2 = player2.turn;
    current_game_state[0].turn3 = ai_turn;
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
        }else if(player2.turn){
            printf("\nPlayer Two's turn");
        }
        if(mode == 1){
            printf("\nPlayer One's score: %d", player1.score);
            printf("\nPlayer Two's score: %d", player2.score);
        }else if(mode == 2){
            printf("\nPlayer One's score: %d", player1.score);
            printf("\nComputer's score: %d", ai_score);
        }
        if(player1.turn || player2.turn){
            printf("\nDraw a line at row: ");
            scanf("%d", &x);
            printf("and column: ");
            scanf("%d", &y);
        }else if(ai_turn){
            // AI algorithm
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                if(complete_box[i] == 3){
                    for(int j = i * 4; j < i * 4 + 4; ++j){
                        if(grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] == ' '){
                            if(boxes_sides_coordinates[j][0] % 2 == 0 && boxes_sides_coordinates[j][1] % 2 != 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 196;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 124;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }
                        }
                    }
                }
            }
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                if(complete_box[i] == 0){
                    for(int j = i * 4 + 3; j >= i * 4; --j){
                        if(grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] == ' '){
                            if(boxes_sides_coordinates[j][0] % 2 == 0 && boxes_sides_coordinates[j][1] % 2 != 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 196;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 124;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }
                        }
                    }
                }
            }
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                if(complete_box[i] == 1){
                    for(int j = i * 4; j < i * 4 + 4; ++j){
                        if(grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] == ' '){
                            if(boxes_sides_coordinates[j][0] % 2 == 0 && boxes_sides_coordinates[j][1] % 2 != 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 196;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 124;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }
                        }
                    }
                }
            }
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                if(complete_box[i] == 2){
                    for(int j = i * 4; j < i * 4 + 4; ++j){
                        if(grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] == ' '){
                            if(boxes_sides_coordinates[j][0] % 2 == 0 && boxes_sides_coordinates[j][1] % 2 != 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 196;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 124;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                redoable = 0;
                                goto s1;
                            }
                        }
                    }
                }
            }
        }
        //check if a line already exists
        if(grid[x][y] == 124 || grid[x][y] == 196){
            print_grid(rows, cols, grid);
            printf("\nAlready occupied, please enter a valid input");
            continue;
        }
        //undo input handling
        if(x == -1 && y == -1){
            if(game_state_counter == 0){
                print_grid(rows, cols, grid);
                printf("\nNothing to undo");
                continue;
            }else{
                if(mode == 1){
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
                }else if(mode == 2){
                    game_state_counter--;
                        player1.score = current_game_state[game_state_counter].score1;
                        ai_score = current_game_state[game_state_counter].score3;
                        player1.turn = current_game_state[game_state_counter].turn1;
                        ai_turn = current_game_state[game_state_counter].turn3;
                        for(int i = 0; i < rows + 1; ++i){
                            for(int j = 0; j < rows + 1; ++j){
                                grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                            }
                        }
                        for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                            complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                        }
                    while(!player1.turn){
                        game_state_counter--;
                        player1.score = current_game_state[game_state_counter].score1;
                        ai_score = current_game_state[game_state_counter].score3;
                        player1.turn = current_game_state[game_state_counter].turn1;
                        ai_turn = current_game_state[game_state_counter].turn3;
                        for(int i = 0; i < rows + 1; ++i){
                            for(int j = 0; j < rows + 1; ++j){
                                grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                            }
                        }
                        for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                            complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                        }
                    }
                    redoable++;
                    print_grid(rows, cols, grid);
                    continue;
                }
            }
        }
        //redo input handling
        if(x == -2 && y == -2){
            if(!redoable){
                print_grid(rows, cols, grid);
                printf("\nNo plays registered to redo");
                continue;
            }else if(redoable){
                if(mode == 1){
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
                }else if(mode == 2){
                    game_state_counter++;
                    player1.score = current_game_state[game_state_counter].score1;
                    ai_score = current_game_state[game_state_counter].score3;
                    player1.turn = current_game_state[game_state_counter].turn1;
                    ai_turn = current_game_state[game_state_counter].turn3;
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
        }
        //check if the line to be printed is horizontal or vertical
        if(x % 2 == 0 && y % 2 != 0 && x < rows && y < cols && x >= 0 && y >= 0){
            grid[x][y] = 196;
            //incrementing state counter
            game_state_counter++;
            redoable = 0;
        }else if(y % 2 == 0 && x % 2 != 0 && x < rows && y < cols && x >= 0 && y >= 0){
            grid[x][y] = 124;
            //incrementing state counter
            game_state_counter++;
            redoable = 0;
        }else{ //if the input is not suitable, the following executes
            print_grid(rows, cols, grid);
            printf("\nPlease, enter a valid input");
            continue;
        }
        s1:
        //score loop
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
                    if(player1.turn){
                        player1.score++;
                    }else if(player2.turn){
                        player2.score++;
                    }else if(ai_turn){
                        ai_score++;
                    }
                    scored = 1;
                    if(player1.turn){
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = player1.symbol;
                    }else if(player2.turn){
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = player2.symbol;
                    }else if(ai_turn){
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = ai_symbol;
                    }
                }
            }
        }
        if(scored && player1.turn){
            scored = 0;
            Beep(750, 300);
            if(mode == 1){
                player1.turn = 1;
                player2.turn = 0;
            }else if(mode == 2){
                player1.turn = 1;
                ai_turn = 0;
            }
            goto s;
        }else if(scored && player2.turn){
            scored = 0;
            Beep(750, 300);
            player1.turn = 0;
            player2.turn = 1;
            goto s;
        }else if(scored && ai_turn){
            scored = 0;
            Beep(750, 300);
            player1.turn = 0;
            ai_turn = 1;
            goto s;
        }
        if(mode == 1){
            if(player1.turn){
                player1.turn = 0;
                player2.turn = 1;
            }else if(player2.turn){
                player1.turn = 1;
                player2.turn = 0;
            }
        }else if(mode == 2){
            if(player1.turn){
                player1.turn = 0;
                ai_turn = 1;
            }else if(ai_turn){
                player1.turn = 1;
                ai_turn = 0;
            }
        }
        s:
        current_game_state[game_state_counter].score1 = player1.score;
        current_game_state[game_state_counter].score2 = player2.score;
        current_game_state[game_state_counter].score3 = ai_score;
        current_game_state[game_state_counter].turn1 = player1.turn;
        current_game_state[game_state_counter].turn2 = player2.turn;
        current_game_state[game_state_counter].turn3 = ai_turn;
        for(int i = 0; i < rows + 1; ++i){
            for(int j = 0; j < rows + 1; ++j){
                current_game_state[game_state_counter].grid_state[i][j] = grid[i][j];
            }
        }
        for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
            current_game_state[game_state_counter].complete_box_state[i] = complete_box[i];
        }
        print_grid(rows, cols, grid);
        if(mode == 1){
            if(player1.score + player2.score == (rows / 2) * (rows / 2)){
                game_on = 0;
            }
        }else if(mode == 2){
            if(player1.score + ai_score == (rows / 2) * (rows / 2)){
                game_on = 0;
            }
        }
    }
}

//function that handles invalid inputs

void wrong_selection(){
    system("cls");
    printf("\nPlease, enter a valid input\n");
}

//game mode selection (work in progress)

int game_mode(char mode_selection){
    int mode = 0;
    printf("\n\n\n\n\n\n\n%c PVP (Press 1)\n%c PVE (Press 2)\n\n", 16, 16);
    scanf("%d", &mode_selection);
    switch(mode_selection){
        case 1:
            mode = 1;
            system("cls");
            break;
        case 2:
            mode = 2;
            system("cls");
            break;
        default:
            wrong_selection();
            game_mode(mode_selection);
            break;
    }
    return mode;
}

//3x3 or 5x5 (more could be added later)

void difficulity(char difficulity_selection, int mode){
    printf("\n\n\n\n\n\n\n%c Beginner (Press 1)\n%c Expert (Press 2)\n\n", 16, 16);
    scanf("%d", &difficulity_selection);
    switch(difficulity_selection){
        case 1:
            system("cls");
            grid(5, 5, mode);
            printf("\nGame over!");
            break;
        case 2:
            system("cls");
            grid(11, 11, mode);
            printf("\nGame over!");
            break;
        default:
            wrong_selection();
            difficulity(difficulity_selection, mode);
            break;
    }
}

//main menu function (work in progress)

void menu(){
    system("Color 90");
    printf("\n\n\n\n\n\n\n%c Start Game (Press 1)\n%c Load Game (Press 2)\n%c Top 10 Players (Press 3)\n%c Exit (Press 4)\n\n", 16, 16, 16, 16);
        char first_selection, difficulity_selection, mode_selection;
        int mode = 0;
        scanf("%d", &first_selection);
        switch(first_selection){
            case 1:
                system("cls");
                mode = game_mode(mode_selection);
                difficulity(difficulity_selection, mode);
                break;
            case 2:
                system("cls");
                printf("\nChoose the saved game to load: \n\n");
                break;
            case 3:
                break;
            case 4:
                system("exit");
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
