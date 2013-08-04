
#include "Othello.h"
#include <stdio.h>
#include <stdlib.h>

void init_othello(Othello *othello)
{
    othello->ply_count = 0;
    othello->game_over = false;
    othello->black = INITIAL_BLACK_BB;
    othello->white = INITIAL_WHITE_BB;
    othello->legal = INITIAL_LEGAL_BB;
}

bool is_game_over(Othello *othello)
{
    return othello->game_over;
}

cell_type *get_board(Othello *othello)
{
    cell_type *b = (cell_type *) malloc(BOARD_CELLS_COUNT * sizeof(cell_type));
    
    for (int i = 0; i < BOARD_CELLS_COUNT; i++)
    {
        if ((othello->black & (UINT64_C(1) << i)) != UINT64_C(0))
            b[i] = kBlack;
        else if ((othello->white & (UINT64_C(1) << i)) != UINT64_C(0))
            b[i] = kWhite;
        else if ((othello->legal & (UINT64_C(1) << i)) != UINT64_C(0))
            b[i] = kLegal;
        else
            b[i] = kEmpty;
    }
    
    return b;
}

void set_current_board(Othello *othello, uint64_t bitboard)
{
    if (othello->ply_count % 2 == 0)
        othello->black = bitboard;
    else
        othello->white = bitboard;
}

void set_opponent_board(Othello *othello, uint64_t bitboard)
{
    if (othello->ply_count % 2 == 0)
        othello->white = bitboard;
    else
        othello->black = bitboard;
}

void print_bitboard(uint64_t board)
{    
    for (int i = 0; i < BOARD_CELLS_COUNT; i++)
    {
        if ((board & (UINT64_C(1) << i)) != UINT64_C(0))
            printf(" X ");
        else
            printf(" - ");
        
        if (i % BOARD_SIZE == BOARD_SIZE - 1)
            printf("\n");
    }
    
    printf("\n");
}

int *moves_array(Othello *othello)
{
    int *moves = (int *) malloc(number_of_moves(othello) * sizeof(int));
    int move = 0;
    
    for (int i = 0; i < BOARD_CELLS_COUNT; i++)
    {
        uint64_t cell_bb = UINT64_C(1) << i;
        
        if ((othello->legal & cell_bb) != UINT64_C(0))
            moves[move++] = i;
    }
    
    return moves;
}

void make_move(Othello *othello, int move)
{
    if (move < 0 || move >= number_of_moves(othello))
        return;

    uint64_t the_move = move_bitboard(othello, move);
    uint64_t all_cells[BOARD_CELLS_COUNT];
    uint64_t tmp_cells[BOARD_SIZE];
    uint64_t next;
    uint64_t last_cell;
    uint64_t opp_board = get_opponent_board(othello);
    uint64_t cur_board = get_current_board(othello);
    
    int tmp_count = 0; 
    int all_count = 0;

    set_current_board(othello, get_current_board(othello) | the_move); 
    
    // UP
    
    last_cell = UINT64_C(0);
    next = (the_move >> BOARD_SIZE) & DOWN_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next >> BOARD_SIZE) & DOWN_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];
    
    // DOWN
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move << BOARD_SIZE) & UP_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next << BOARD_SIZE) & UP_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];
    
    // LEFT
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move >> 1) & RIGHT_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next >> 1) & RIGHT_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];
    
    // RIGHT
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move << 1) & LEFT_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next << 1) & LEFT_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];
    
    
    // TOP LEFT
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move >> (BOARD_SIZE + 1)) & RIGHT_MASK & DOWN_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next >> (BOARD_SIZE + 1)) & RIGHT_MASK & DOWN_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];
    
    // TOP RIGHT
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move >> (BOARD_SIZE - 1)) & LEFT_MASK & DOWN_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next >> (BOARD_SIZE - 1)) & LEFT_MASK & DOWN_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];
    
    // DOWN LEFT
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move << (BOARD_SIZE - 1)) & RIGHT_MASK & UP_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next << (BOARD_SIZE - 1)) & RIGHT_MASK & UP_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++];    
    
    // DOWN RIGHT
    
    tmp_count = 0;
    last_cell = UINT64_C(0);
    next = (the_move << (BOARD_SIZE + 1)) & LEFT_MASK & UP_MASK & opp_board;
    
    while (next != UINT64_C(0)) {
        tmp_cells[tmp_count++] = next;
        uint64_t tmp = (next << (BOARD_SIZE + 1)) & LEFT_MASK & UP_MASK;
        last_cell = tmp & cur_board;
        next = tmp & opp_board;
    }
    
    if (last_cell != UINT64_C(0))
        for (int i = 0; i < tmp_count; i++)
            all_cells[all_count++] = tmp_cells[i++]; 
    
    // CHANGE BITBOARDS
    printf("all count: %d\n\n", all_count);
    
    for (int i = 0; i < all_count; i++) {
        set_current_board(othello, get_current_board(othello) | all_cells[i]);
        set_opponent_board(othello, get_opponent_board(othello) & ~all_cells[i]);
    }
    
    othello->ply_count++;
    calculate_moves(othello);
    
    if (count_bits(othello->legal) == UINT64_C(0))
    {
        othello->ply_count++;
        calculate_moves(othello);
        
        if (count_bits(othello->legal) == UINT64_C(0))
            othello->game_over = true;
        else
            othello->legal = PASS;
        
        othello->ply_count--;
    }
}

int number_of_moves(Othello *othello)
{
    return count_bits(othello->legal);
//    return othello->legal == PASS ? 1 : count_bits(othello->legal);
}

void calculate_moves(Othello *othello)
{
    othello->legal = UINT64_C(0);
    uint64_t potential_moves = UINT64_C(0);
    uint64_t cur_board = get_current_board(othello);
    uint64_t opp_board = get_opponent_board(othello);
    uint64_t emp_board = empty_board(othello);
    
    // UP
    potential_moves = (cur_board >> BOARD_SIZE) & DOWN_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves >> BOARD_SIZE) & DOWN_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // DOWN
    potential_moves = (cur_board << BOARD_SIZE) & UP_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves << BOARD_SIZE) & UP_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // LEFT
    potential_moves = (cur_board >> 1) & RIGHT_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves >> 1) & RIGHT_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // RIGHT
    potential_moves = (cur_board << 1) & LEFT_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves << 1) & LEFT_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // UP LEFT
    potential_moves = (cur_board >> (BOARD_SIZE + 1)) & RIGHT_MASK & DOWN_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves >> (BOARD_SIZE + 1)) & RIGHT_MASK & DOWN_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // UP RIGHT
    potential_moves = (cur_board >> (BOARD_SIZE - 1)) & LEFT_MASK & DOWN_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves >> (BOARD_SIZE - 1)) & LEFT_MASK & DOWN_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // DOWN LEFT
    potential_moves = (cur_board << (BOARD_SIZE - 1)) & RIGHT_MASK & UP_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves << (BOARD_SIZE - 1)) & RIGHT_MASK & UP_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
    
    // DOWN RIGHT
    potential_moves = (cur_board << (BOARD_SIZE + 1)) & LEFT_MASK & UP_MASK & opp_board;
    while (potential_moves != UINT64_C(0)) {
        long tmp = (potential_moves << (BOARD_SIZE + 1)) & LEFT_MASK & UP_MASK;
        othello->legal |= tmp & emp_board;
        potential_moves = tmp & opp_board;
    }
}

unsigned int count_bits(uint64_t n)
{     
    unsigned int c; // c accumulates the total bits set in v
    for (c = 0; n; c++) 
        n &= n - UINT64_C(1); // clear the least significant bit set
    return c;
}

uint64_t move_bitboard(Othello *othello, int move)
{
//    uint64_t *moves = (uint64_t *) malloc(othello->legal_moves * sizeof(uint64_t));    
    if (othello->legal == PASS)
        return PASS;
    
    for (int i = 0, move_count = 0; i < BOARD_CELLS_COUNT; i++)
    {
        uint64_t cell_bb = UINT64_C(1) << i;
        
        if (((othello->legal & cell_bb) != UINT64_C(0)) && (move_count++ == move))
            return cell_bb;
    }
    
    return UINT64_C(0);
}

uint64_t get_current_board(Othello *othello)
{
    return othello->ply_count % 2 == 0 ? othello->black : othello->white;
}

uint64_t get_opponent_board(Othello *othello)
{
    return othello->ply_count % 2 == 0 ? othello->white : othello->black;
}

uint64_t empty_board(Othello *othello)
{
    return ~(othello->black | othello->white);
}

int current_player(Othello *othello)
{
    return othello->ply_count % 2;
}

void print_board(Othello *othello)
{
    printf("Ply %d\n", othello->ply_count);
    printf("Black count %d\n", count_bits(othello->black));
    printf("White count %d\n", count_bits(othello->white));
    printf("Number of moves %d\n", count_bits(othello->legal));
    printf("Current player: %d\n\n", current_player(othello));
    
    for (int i = 0; i < BOARD_CELLS_COUNT; i++)
    {
        if ((othello->black & (UINT64_C(1) << i)) != UINT64_C(0))
            printf(" X ");
        else if ((othello->white & (UINT64_C(1) << i)) != UINT64_C(0))
            printf(" O ");
        else if ((othello->legal & (UINT64_C(1) << i)) != UINT64_C(0))
            printf(" P ");
        else
            printf(" - ");
        
        if (i % BOARD_SIZE == BOARD_SIZE - 1)
            printf("\n");
    }
    
    printf("\n");
}
