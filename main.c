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

//global variables

int load, winner_score;
FILE *end1;
FILE *end2;
char top_ten_names[10][100], winner_name1[100], winner_name2[100];
int top_ten_scores[10];

//player info

int score1, score2, ai_score, turn1, turn2, ai_turn, moves1, moves2, ai_moves, symbol1, symbol2, ai_symbol, rows, cols, mode, remaining_moves;
int grid2[12][12], complete_box2[25];

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
                printf("\t\t\t\t  ");
            }
            if(i == rows || j == cols){
                printf("%d ", grid[i][j]);
                continue;
            }
            if(grid[i][j] == 179 || grid[i][j] == 196 || grid[i][j] == 'A'){
                printf(ANSI_COLOR_GREEN "% c " ANSI_COLOR_RESET, grid[i][j]);
                continue;
            }else if(grid[i][j] == 186 || grid[i][j] == 205 || grid[i][j] == 'B' || grid[i][j] == 'X'){
                printf(ANSI_COLOR_RED "% c " ANSI_COLOR_RESET, grid[i][j]);
                continue;
            }
            printf(ANSI_COLOR_YELLOW "% c " ANSI_COLOR_RESET, grid[i][j]);
        }
        printf("\n");
    }
}

//grid initiation function (game loop included)

void grid(int rows, int cols, int mode, int load){
    //initiating the grid
    int grid[rows + 1][cols + 1];
    int complete_box[(rows / 2) * (rows / 2)];
    if(!load){
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
        //initialize complete boxes array
        for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
            complete_box[i] = 0;
        }
    }
    if(load){
        for(int i = 0; i < rows + 1; ++i){
            for(int j = 0; j < cols + 1; ++j){
                grid[i][j] = grid2[i][j];
            }
        }
        for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
            complete_box[i] = complete_box2[i];
        }
    }
    //calling the grid printing function
    print_grid(rows, cols, grid);
    //score calculations
    int boxes_center_coordinates[(rows / 2) * (rows / 2)][2];
    int boxes_sides_coordinates[4 * (rows / 2) * (rows / 2)][2];
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
    //game loop
    int x, y;
    int game_on = 1;
    if(!load){
        symbol1 = 'A';
        symbol2 = 'B';
        ai_symbol = 'X';
        turn1 = 1;
        turn2 = 0;
        ai_turn = 0;
        score1 = 0;
        score2 = 0;
        ai_score = 0;
        moves1 = 0;
        moves2 = 0;
        ai_moves = 0;
        remaining_moves = 2*(rows/2)*(rows/2+1);
    }
    //defining the struct that will carry the game state
    //it will be used to execute the undo and the redo functions
    typedef struct{
        int grid_state[rows + 1][rows + 1];
        int score1, score2, score3, turn1, turn2, turn3;
        int complete_box_state[(rows / 2) * (rows / 2)];
    } game_state;
    typedef struct{
        int moves1, moves2, moves3, remaining_moves;
    } moves_state;
    moves_state current_moves_state[2*(rows/2)*(rows/2+1)];
    game_state current_game_state[2*(rows/2)*(rows/2+1)];
    current_game_state[0].score1 = score1;
    current_game_state[0].score2 = score2;
    current_game_state[0].score3 = ai_score;
    current_game_state[0].turn1 = turn1;
    current_game_state[0].turn2 = turn2;
    current_game_state[0].turn3 = ai_turn;
    current_moves_state[0].moves1 = moves1;
    current_moves_state[0].moves2 = moves2;
    current_moves_state[0].moves3 = ai_moves;
    current_moves_state[0].remaining_moves = 2*(rows/2)*(rows/2+1);
    for(int i = 0; i < rows + 1; ++i){
        for(int j = 0; j < rows + 1; ++j){
            current_game_state[0].grid_state[i][j] = grid[i][j];
        }
    }
    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
        current_game_state[0].complete_box_state[i] = complete_box[i];
    }
    int scored = 0, game_state_counter = 0, moves_state_counter = 0, redoable = 0, saved = 0;
    while(game_on){
        if(turn1){
            printf(ANSI_COLOR_GREEN "\n\nPlayer One's turn" ANSI_COLOR_RESET);
        }else if(turn2){
            printf(ANSI_COLOR_RED "\n\nPlayer Two's turn" ANSI_COLOR_RESET);
        }else if(ai_turn){
            printf(ANSI_COLOR_RED "\n\nComputer's turn" ANSI_COLOR_RESET);
        }
        if(mode == 1){
            printf(ANSI_COLOR_GREEN "\n\nPlayer One's score: %d\t\t\t\t\tPlayer one's moves: %d" ANSI_COLOR_RESET, score1, moves1);
            printf(ANSI_COLOR_RED "\nPlayer Two's score: %d\t\t\t\t\tPlayer Two's moves: %d" ANSI_COLOR_RESET, score2, moves2);
        }else if(mode == 2){
            printf(ANSI_COLOR_GREEN "\n\nPlayer One's score: %d\t\t\t\t\tPlayer one's moves: %d" ANSI_COLOR_RESET, score1, moves1);
            printf(ANSI_COLOR_RED "\nComputer's score: %d\t\t\t\t\tComputer's moves: %d" ANSI_COLOR_RESET, ai_score, ai_moves);
        }
        printf(ANSI_COLOR_CYAN "\n\n\t\t\t\tMoves left: %d\t\t\tundo: (-1, -1)\tredo: (-2, -2)\n\t\t\t\t\t\t\t\tsave and go back to main menu: (-3, -3)\n\t\t\t\t\t\t\t\texit: (0, 0)", remaining_moves);
        if(turn1 || turn2){
            printf(ANSI_COLOR_BLUE "\n\nDraw a line at row: " ANSI_COLOR_RESET);
            scanf("%d", &x);
            printf(ANSI_COLOR_BLUE "and column: " ANSI_COLOR_RESET);
            scanf("%d", &y);
        }else if(ai_turn){
            Beep(0, 500);
            // AI algorithm
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                if(complete_box[i] == 3){
                    for(int j = i * 4; j < i * 4 + 4; ++j){
                        if(grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] == ' '){
                            if(boxes_sides_coordinates[j][0] % 2 == 0 && boxes_sides_coordinates[j][1] % 2 != 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 205;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                moves_state_counter++;
                                redoable = 0;
                                goto s1;
                            }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 186;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                moves_state_counter++;
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
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 205;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                moves_state_counter++;
                                redoable = 0;
                                goto s1;
                            }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 186;
                                x = boxes_sides_coordinates[j][0];
                                y = boxes_sides_coordinates[j][1];
                                game_state_counter++;
                                moves_state_counter++;
                                redoable = 0;
                                goto s1;
                            }
                        }
                    }
                }
            }
            for(int k = 1; k <= 2; ++k){
                for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                    if(complete_box[i] == k){
                        for(int j = i * 4; j < i * 4 + 4; ++j){
                            if(grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] == ' '){
                                if(boxes_sides_coordinates[j][0] % 2 == 0 && boxes_sides_coordinates[j][1] % 2 != 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                    grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 205;
                                    x = boxes_sides_coordinates[j][0];
                                    y = boxes_sides_coordinates[j][1];
                                    game_state_counter++;
                                    moves_state_counter++;
                                    redoable = 0;
                                    goto s1;
                                }else if(boxes_sides_coordinates[j][0] % 2 != 0 && boxes_sides_coordinates[j][1] % 2 == 0 && boxes_sides_coordinates[j][0] < rows && boxes_sides_coordinates[j][1] < cols){
                                    grid[boxes_sides_coordinates[j][0]][boxes_sides_coordinates[j][1]] = 186;
                                    x = boxes_sides_coordinates[j][0];
                                    y = boxes_sides_coordinates[j][1];
                                    game_state_counter++;
                                    moves_state_counter++;
                                    redoable = 0;
                                    goto s1;
                                }
                            }
                        }
                    }
                }
            }
        } //end of AI function
        //check if a line already exists
        if(grid[x][y] == 179 || grid[x][y] == 196 || grid[x][y] == 186 || grid[x][y] == 205){
            print_grid(rows, cols, grid);
            printf("\nAlready occupied, please enter a valid input");
            continue;
        }
        //exit
        if(x == 0 && y == 0){
            return;
        }
        //save input handling
        if(x == -3 && y == -3){
            FILE *game;
            system("cls");
            h:
            printf("\nSave at 1, 2, or 3\n");
            scanf("%d", &load);
            switch(load){
                case 1:
                    game = fopen("save1.bin", "wb");
                    break;
                case 2:
                    game = fopen("save2.bin", "wb");
                    break;
                case 3:
                    game = fopen("save3.bin", "wb");
                    break;
                default:
                    system("cls");
                    printf("Enter a valid input");
                    goto h;
            }
            fprintf(game, "%d %d %d %d %d %d %c %c %d %d %d %d", score1, score2, turn1, turn2, moves1, moves2, symbol1, symbol2, rows, cols, mode, remaining_moves);
            for(int i = 0; i < rows + 1; ++i){
                for(int j = 0; j < rows + 1; ++j){
                    fprintf(game, "%c", grid[i][j]);
                }
            }
            for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                fprintf(game, " %d ", complete_box[i]);
            }
            fclose(game);
            system("cls");
            saved = 1;
            break;
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
                    moves_state_counter--;
                    score1 = current_game_state[game_state_counter].score1;
                    score2 = current_game_state[game_state_counter].score2;
                    turn1 = current_game_state[game_state_counter].turn1;
                    turn2 = current_game_state[game_state_counter].turn2;
                    moves1 = current_moves_state[moves_state_counter].moves1;
                    moves2 = current_moves_state[moves_state_counter].moves2;
                    remaining_moves = current_moves_state[moves_state_counter].remaining_moves;
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
                    moves_state_counter--;
                    score1 = current_game_state[game_state_counter].score1;
                    ai_score = current_game_state[game_state_counter].score3;
                    turn1 = current_game_state[game_state_counter].turn1;
                    ai_turn = current_game_state[game_state_counter].turn3;
                    moves1 = current_moves_state[game_state_counter].moves1;
                    ai_moves = current_moves_state[moves_state_counter].moves3;
                    remaining_moves = current_moves_state[moves_state_counter].remaining_moves;
                    for(int i = 0; i < rows + 1; ++i){
                        for(int j = 0; j < rows + 1; ++j){
                            grid[i][j] = current_game_state[game_state_counter].grid_state[i][j];
                        }
                    }
                    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
                        complete_box[i] = current_game_state[game_state_counter].complete_box_state[i];
                    }
                    while(!turn1){
                        game_state_counter--;
                        moves_state_counter--;
                        score1 = current_game_state[game_state_counter].score1;
                        ai_score = current_game_state[game_state_counter].score3;
                        turn1 = current_game_state[game_state_counter].turn1;
                        ai_turn = current_game_state[game_state_counter].turn3;
                        moves1 = current_moves_state[moves_state_counter].moves1;
                        ai_moves = current_moves_state[moves_state_counter].moves3;
                        remaining_moves = current_moves_state[moves_state_counter].remaining_moves;
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
                    moves_state_counter++;
                    score1 = current_game_state[game_state_counter].score1;
                    score2 = current_game_state[game_state_counter].score2;
                    turn1 = current_game_state[game_state_counter].turn1;
                    turn2 = current_game_state[game_state_counter].turn2;
                    moves1 = current_moves_state[moves_state_counter].moves1;
                    moves2 = current_moves_state[moves_state_counter].moves2;
                    remaining_moves = current_moves_state[moves_state_counter].remaining_moves;
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
                    moves_state_counter++;
                    score1 = current_game_state[game_state_counter].score1;
                    ai_score = current_game_state[game_state_counter].score3;
                    turn1 = current_game_state[game_state_counter].turn1;
                    ai_turn = current_game_state[game_state_counter].turn3;
                    moves1 = current_moves_state[moves_state_counter].moves1;
                    ai_moves = current_moves_state[moves_state_counter].moves3;
                    remaining_moves = current_moves_state[moves_state_counter].remaining_moves;
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
            if(turn1){
                grid[x][y] = 196;
                moves1++;
                remaining_moves--;
            }else if(turn2){
                grid[x][y] = 205;
                moves2++;
                remaining_moves--;
            }
            //incrementing state counter
            game_state_counter++;
            moves_state_counter++;
            redoable = 0;
        }else if(y % 2 == 0 && x % 2 != 0 && x < rows && y < cols && x >= 0 && y >= 0){
            if(turn1){
                grid[x][y] = 179;
                moves1++;
                remaining_moves--;
            }else if(turn2){
                grid[x][y] = 186;
                moves2++;
                remaining_moves--;
            }
            //incrementing state counter
            game_state_counter++;
            moves_state_counter++;
            redoable = 0;
        }else{ //if the input is not suitable, the following executes
            print_grid(rows, cols, grid);
            printf("\nPlease, enter a valid input");
            continue;
        }
        s1: //modifiy moves for AI
        if(ai_turn){
            ai_moves++;
            remaining_moves--;
        }
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
                    if(turn1){
                        score1++;
                    }else if(turn2){
                        score2++;
                    }else if(ai_turn){
                        ai_score++;
                    }
                    scored = 1;
                    if(turn1){
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = symbol1;
                    }else if(turn2){
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = symbol2;
                    }else if(ai_turn){
                        grid[boxes_center_coordinates[i / 4][0]][boxes_center_coordinates[i / 4][1]] = ai_symbol;
                    }
                }
            }
        }
        if(scored && turn1){
            scored = 0;
            Beep(750, 300);
            if(mode == 1){
                turn1 = 1;
                turn2 = 0;
            }else if(mode == 2){
                turn1 = 1;
                ai_turn = 0;
            }
            goto s;
        }else if(scored && turn2){
            scored = 0;
            Beep(750, 300);
            turn1 = 0;
            turn2 = 1;
            goto s;
        }else if(scored && ai_turn){
            scored = 0;
            Beep(750, 300);
            turn1 = 0;
            ai_turn = 1;
            goto s;
        }
        if(mode == 1){
            if(turn1){
                turn1 = 0;
                turn2 = 1;
            }else if(turn2){
                turn1 = 1;
                turn2 = 0;
            }
        }else if(mode == 2){
            if(turn1){
                turn1 = 0;
                ai_turn = 1;
            }else if(ai_turn){
                turn1 = 1;
                ai_turn = 0;
            }
        }
        s:
        current_game_state[game_state_counter].score1 = score1;
        current_game_state[game_state_counter].turn1 = turn1;
        current_moves_state[moves_state_counter].moves1 = moves1;
        current_moves_state[moves_state_counter].remaining_moves = remaining_moves;
        if(mode == 1){
            current_game_state[game_state_counter].score2 = score2;
            current_game_state[game_state_counter].turn2 = turn2;
            current_moves_state[moves_state_counter].moves2 = moves2;
        }else if(mode == 2){
            current_game_state[game_state_counter].score3 = ai_score;
            current_game_state[game_state_counter].turn3 = ai_turn;
            current_moves_state[moves_state_counter].moves3 = ai_moves;
        }
        for(int i = 0; i < rows + 1; ++i){
            for(int j = 0; j < rows + 1; ++j){
                current_game_state[game_state_counter].grid_state[i][j] = grid[i][j];
            }
        }
        for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
            current_game_state[game_state_counter].complete_box_state[i] = complete_box[i];
        }
        print_grid(rows, cols, grid);
        if(remaining_moves == 0){
            game_on = 0;
        }
    }
    if(saved){
        menu();
        return ;
    }
    if(mode == 1){
        printf(ANSI_COLOR_GREEN "\nPlayer One's score: %d\t\t\t\t\tPlayer one's moves: %d" ANSI_COLOR_RESET, score1, moves1);
        printf(ANSI_COLOR_RED "\nPlayer Two's score: %d\t\t\t\t\tPlayer Two's moves: %d" ANSI_COLOR_RESET, score2, moves2);
        if(score1 > score2){
            printf(ANSI_COLOR_GREEN "\nPlayer one wins!" ANSI_COLOR_RESET);
            winner_score = score1;
        }else if(score1 < score2){
            printf(ANSI_COLOR_RED "\nPlayer two wins!" ANSI_COLOR_RESET);
            winner_score = score2;
        }else{
            printf(ANSI_COLOR_MAGENTA "\nDraw!" ANSI_COLOR_RESET);
        }
    }else if(mode == 2){
        printf(ANSI_COLOR_GREEN "\nPlayer One's score: %d\t\t\t\t\tPlayer one's moves: %d" ANSI_COLOR_RESET, score1, moves1);
        printf(ANSI_COLOR_RED "\nComputer's score: %d\t\t\t\t\tComputer's moves: %d" ANSI_COLOR_RESET, ai_score, ai_moves);
        if(score1 > ai_score){
            printf(ANSI_COLOR_GREEN "\nPlayer one wins!" ANSI_COLOR_RESET);
            winner_score = score1;
        }else if(score1 < ai_score){
            printf(ANSI_COLOR_RED "\nComputer wins!" ANSI_COLOR_RESET);
        }else{
            printf(ANSI_COLOR_MAGENTA "\nDraw!" ANSI_COLOR_RESET);
        }
    }
    for (int i=0; i<500; i+=100){
    Beep (i,200);
    }
    for (int i=500; i>0; i-=100){
    Beep (i,200);
    }
    /*end1 = fopen("topTenNames.bin", "wb");
    int name_length = 0;
    if(ai_score <= score1){
        if(winner_score > top_ten_scores[9]){
            printf("\nEnter your name: ");
            while(getchar() != '\0'){
                scanf("%c", &winner_name1[name_length++]);
            }
            for(int i = 8; i >= 0; --i){
                if(winner_score < top_ten_scores[i]){
                    for(int j = 0; i < name_length; ++i){
                        top_ten_names[i + 1][j] = winner_name1[j];
                    }
                    break;
                }
            }
        }
        for(int i = 0; i < 10; ++i){
            fprintf(end1, "%s", top_ten_names[i]);
        }
        fclose(end1);
    }*/
    system("cls");
    menu();
}

//function that handles invalid inputs

void wrong_selection(){
    system("cls");
    printf("\nPlease, enter a valid input\n");
}

//load

void loadf(){
    FILE *game;
    o:
    printf("\nSave 1 (press 1)\n\nSave 2 (press 2)\n\nSave 3 (press 3)\n");
    scanf("%d", &load);
    switch(load){
        case 1:
            game = fopen("save1.bin", "rb");
            break;
        case 2:
            game = fopen("save2.bin", "rb");
            break;
        case 3:
            game = fopen("save3.bin", "rb");
            break;
        default:
            wrong_selection();
            goto o;
            break;
    }
    fscanf(game, "%d %d %d %d %d %d %c %c %d %d %d %d", &score1, &score2, &turn1, &turn2, &moves1, &moves2, &symbol1, &symbol2, &rows, &cols, &mode, &remaining_moves);
    for(int i = 0; i < rows + 1; ++i){
        for(int j = 0; j < rows + 1; ++j){
            fscanf(game, "%c", &grid2[i][j]);
        }
    }
    for(int i = 0; i < ((rows / 2) * (rows / 2)); ++i){
        fscanf(game, " %d ", &complete_box2[i]);
    }
    fclose(game);
    system("cls");
    grid(rows, cols, mode, load);
}

//game mode selection

int game_mode(){
    int mode = 0;
    k:
    printf("\n\n\n\n\n\n\n%c PVP (Press 1)\n%c PVE (Press 2)\n\n", 16, 16);
    scanf("%d", &mode);
    switch(mode){
        case 1:
            system("cls");
            break;
        case 2:
            system("cls");
            break;
        default:
            wrong_selection();
            goto k;
            break;
    }
    return mode;
}

//3x3 or 5x5 (more could be added later)

void difficulity(int mode){
    int diff = 0;
    load = 0;
    t:
    printf("\n\n\n\n\n\n\n%c Beginner (Press 1)\n%c Expert (Press 2)\n\n", 16, 16);
    scanf("%d", &diff);
    switch(diff){
        case 1:
            system("cls");
            grid(5, 5, mode, load);
            break;
        case 2:
            system("cls");
            grid(11, 11, mode, load);
            break;
        default:
            wrong_selection();
            goto t;
            break;
    }
}

//main menu function (work in progress)

void menu(){
    system("cls");
    printf("\n\n\n\n\n\n\n%c Start Game (Press 1)\n%c Load Game (Press 2)\n%c Top 10 Players (Press 3)\n%c Exit (Press 4)\n\n", 16, 16, 16, 16);
    char first_selection, difficulity_selection, mode_selection;
    int mode = 0;
    scanf("%d", &first_selection);
    switch(first_selection){
        case 1:
            system("cls");
            mode = game_mode();
            difficulity(mode);
            break;
        case 2:
            system("cls");
            loadf();
            break;
        case 3:
            /*system("cls");
            printf("\nName\tScore\n");
            end1 = fopen("topTenNames.bin", "rb");
            for(int i = 0; i < 10; ++i){
                fscanf(end1, "%s", &top_ten_names[i]);
            }
            fclose(end1);
            for(int i = 0; i < 10; ++i){
                printf("%s\n", top_ten_names[i]);
            }
            printf("Enter anything to go back to main menu\n");
            int foo;
            scanf("%d", &foo);
            menu();*/
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
