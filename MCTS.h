#ifndef DEF_MCTS
#define DEF_MCTS

#include "game.h"
#include "tree.h"
#include <random>
#include <chrono>
#include <ctime>
#include <immintrin.h>
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::seconds Units;

// Modulo operator; C++ doesn't always returns positive values
int mod(int k, int n);

class MCTS
{
    // Random generators
    std::random_device rd;
    std::mt19937 gen{rd()};
    //std::mt19937 gen{0};

public:
    int m_MCTS_player; // 0 or 1
    //Units m_calculation_time = std::chrono::duration_cast<Units>(std::chrono::milliseconds(100));
    float m_calculation_time = 100;
    int m_max_iter = 61;
    float m_C = 1.4;
    int m_max_depth = 0;

    // Eventually map to a set depending on the constrainsts
    //robin_hood::unordered_map<long long int,Node*> m_visited_states;
    //std::unordered_map<long long int,Node*> m_visited_states;

    // Nodes of the tree are preallocated as creation and deletion can be expensive
    //Node pool[5000000];
    Node *pool = new Node[5000000];
    Node *m_pool_ptr = &pool[0]; // Pointer to the pool
    Node *m_node_ptr = nullptr; // Current node pointer

    Othello *m_game_ptr;

    MCTS(Othello *game, int MCTS_player);
    ~MCTS();

    void run_simulation();
    // Returns the number of legal moves for the current state of the game
    int get_num_move_sim();
    // Returns a move based on num_move (RNG for random move)
    void get_move_sim(uint64_t state1, uint64_t state2, int player, int num_move, int &action, int &tot_moves);

    void get_move(int opp_action, int &action);
};

#endif