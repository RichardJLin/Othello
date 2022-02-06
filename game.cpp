#include "game.h"


Othello::Othello(int player)
{
    m_current_player = player;
    m_state2=0x1008000000, m_state1=0x810000000;

}

Othello::Othello(int player, uint64_t state1, uint64_t state2) {
    m_current_player = player;
    m_state1 = state1;
    m_state2 = state2;
}

// Shifts the board according to a direction
// 4 5 6
// 7 X 0
// 1 2 3
uint64_t Othello::shift(uint64_t board, int direction) const {
    const uint64_t masks[8] = {
        0xFEFEFEFEFEFEFEFEULL,
        0x7F7F7F7F7F7F7F00ULL,
        0xFFFFFFFFFFFFFF00ULL,
        0xFEFEFEFEFEFEFE00ULL,
        0x007F7F7F7F7F7F7FULL,
        0x00FFFFFFFFFFFFFFULL,
        0x00FEFEFEFEFEFEFEULL,
        0x007F7F7F7F7F7F7FULL,
    };

    const uint dirs[4] = {1, 7, 8, 9};

    if (direction >= 4) {
        return (board >> dirs[7-direction]) & masks[direction];
    }
    else {
        return (board << dirs[direction]) & masks[direction];
    }

}

uint64_t Othello::possible_moves(uint64_t state1, uint64_t state2, int player) const
{
    uint64_t currentState = player ? state1 : state2;
    uint64_t oppState = player ? state2 : state1;
    uint64_t empty = ~(currentState | oppState);
    uint64_t moves = 0;
    uint64_t x;

    // Dumb 7 fill for each direction
    for (int dir=0; dir < 8; ++dir) {
        /* Get opponent disks adjacent to my disks in direction dir. */
        x = shift(currentState, dir) & oppState;
        x |= shift(x, dir) & oppState;
        x |= shift(x, dir) & oppState;
        x |= shift(x, dir) & oppState;
        x |= shift(x, dir) & oppState;
        x |= shift(x, dir) & oppState;

        moves |= shift(x, dir) & empty;
    }

    return moves;
}

int Othello::make_move(int move, uint64_t &state1, uint64_t &state2, int &player) {

    if (move != -1) {
        uint64_t &currentState = player ? state1 : state2;
        uint64_t &oppState = player ? state2 : state1;
        uint64_t captured_stones = 0;
        uint64_t x, bounded, new_board = 1ULL << move;

        currentState |= new_board;

        // Check for each direction from the stone and we take opponent stones
        for (int dir=0;dir<8;++dir) {
            x = shift(new_board, dir) & oppState;
            x |= shift(x,dir) & oppState;
            x |= shift(x,dir) & oppState;
            x |= shift(x,dir) & oppState;
            x |= shift(x,dir) & oppState;
            x |= shift(x,dir) & oppState;

            // Check if a stone from the current player at the end
            bounded = shift(x,dir) & currentState;
            captured_stones |= (bounded ? x : 0);
        }

        currentState ^= captured_stones ;
        oppState ^= captured_stones;
    }
    player ^= 1;

    // Unfinished game
    if (possible_moves(state1, state2, 0) | possible_moves(state1, state2, 1)) {
        return -1;
    }
    else {
        int p1 = __builtin_popcountll(state1);
        int p2 = __builtin_popcountll(state2);
        if (p1 < p2) {
            return 0;
        }
        else if (p1 > p2) {
            return 1;
        }
        else {
            // Draw
            return -2;
        }
    }
}

int Othello::make_move_self(int move) {
    return make_move(move, m_state1, m_state2, m_current_player);
}

int Othello::compute_winner(uint64_t state1, uint64_t state2) {
    // Unfinished game
    if (possible_moves(state1, state2, 0) | possible_moves(state1, state2, 1)) {
        return -1;
    }
    else {
        int p1 = __builtin_popcountll(state1);
        int p2 = __builtin_popcountll(state2);
        if (p1 < p2) {
            return 0;
        }
        else if (p1 > p2) {
            return 1;
        }
        else {
            // Draw
            return -2;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Othello& othello) {
    /*
    0 1 2 3 4 5 6 7
    8 9 10...
    */
    uint64_t moves = othello.possible_moves(othello.m_state1, othello.m_state2, othello.m_current_player);
    std::string movesStr;
    os << " 01234567";
    for (int i=0;i<64;++i) {
        if (i%8 == 0) {
            os << '\n' << i / 8;
        }
        if (othello.m_state1 >> i & 1) {
            os << 'X';
        }
        else if (othello.m_state2 >> i & 1) {
            os << 'O';
        }
        else if (moves >> i & 1) {
            os << '*';
            movesStr.append(std::to_string(i));
            movesStr.push_back(' ');
        }
        else {
            os << '.';
        }
    }
    os << '\n' << "Possible moves: " << movesStr << '\n';
    return os;
}
