#ifndef GAME
#define GAME

#include <vector>
#include <iostream>
#include <string>

class Othello
{
    public :
    Othello(int player = 1);
    // Player 1 is O; Player 2 is X
    // Number 1              0
    Othello(int player, uint64_t state1, uint64_t state2);

    uint64_t m_state1, m_state2;
    int m_current_player;
    
    uint64_t shift(uint64_t board, int direction) const;
    // Updates boards and return the winner if available
    int make_move(int move, uint64_t &state1, uint64_t &state2, int &player);
    int make_move_self(int move);
    // Computes the possible moves
    uint64_t possible_moves(uint64_t state1, uint64_t state2, int player) const;
    int compute_winner(uint64_t state1, uint64_t state2);

    friend std::ostream& operator<<(std::ostream& os, const Othello& othello);
};

#endif
