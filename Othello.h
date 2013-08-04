
#ifndef _OTHELLO_
#define _OTHELLO_

#include <stdint.h>
#include <stdbool.h>

#define BOARD_SIZE                  8
#define BOARD_CELLS_COUNT          64
#define BOARD_SIZE_64     UINT64_C(8)
#define EMPTY_CELL                 -1
#define PASS               UINT64_MAX

#define INITIAL_BLACK_BB UINT64_C(0x810000000)
#define INITIAL_WHITE_BB UINT64_C(0x1008000000)
#define INITIAL_LEGAL_BB UINT64_C(0x102004080000)

#define RIGHT_MASK  9187201950435737471LL
#define LEFT_MASK -72340172838076674LL
#define UP_MASK -256LL
#define DOWN_MASK 72057594037927935LL

typedef enum cell_type {
    kBlack, kWhite, kEmpty, kLegal
} cell_type;

typedef struct Othello {    
    int ply_count;
    bool game_over;
    uint64_t black;
    uint64_t white;
    uint64_t legal;
} Othello;

// Game

int current_player(Othello *othello);
bool is_game_over(Othello *othello);
void print_board(Othello *othello);
void make_move(Othello *othello, int move);
int number_of_moves(Othello *othello);

// Othello

cell_type *get_board(Othello *othello);
void init_othello(Othello *othello);
void print_bitboard(uint64_t board);
int *moves_array(Othello *othello);
void calculate_moves(Othello *othello);
uint64_t get_current_board(Othello *othello);
uint64_t get_opponent_board(Othello *othello);
uint64_t empty_board(Othello *othello);
uint64_t move_bitboard(Othello *othello, int move);
void set_current_board(Othello *othello, uint64_t board);
void set_opponent_board(Othello *othello, uint64_t bitboard);
unsigned int count_bits(uint64_t n);


#endif