#include "MCTS.h"
#include <math.h>
#include <x86intrin.h>
#include <immintrin.h>

#include <bitset>

int mod(int k, int n) {
    return ((k %= n) < 0) ? k+n : k;
}


MCTS::MCTS(Othello *game, int MCTS_player) {
    m_game_ptr = game;
    m_MCTS_player = MCTS_player;
}

MCTS::~MCTS() {
    delete []pool;
}

void MCTS::get_move_sim(uint64_t state1, uint64_t state2, int player, int num_move, int &action, int &tot_moves) {
    uint64_t avail_moves = m_game_ptr->possible_moves(state1, state2, player);
    tot_moves = __builtin_popcountll(avail_moves);
    // End game or skip turn
    if (tot_moves) {
        action = __builtin_ctzll(_pdep_u64(1ULL << mod(num_move, tot_moves), avail_moves));
    }
    // else already action = -1
}

int MCTS::get_num_move_sim()
{
    uint64_t avail_moves = m_game_ptr->possible_moves(m_game_ptr->m_state1, m_game_ptr->m_state2, m_game_ptr->m_current_player);
    return __builtin_popcountll(avail_moves);
}


void MCTS::run_simulation()
{
    // Store path
    std::vector<Node*> sim_path;
    sim_path.reserve(61);

    // Copy the elements
    uint64_t state1(m_game_ptr->m_state1),state2(m_game_ptr->m_state2);
    int player=m_game_ptr->m_current_player;

    // Keep track of the current node
    Node *node_ptr = m_node_ptr;
    Node *next_node_ptr = nullptr;

    // Expand one node at most
    bool expand = 1, expand_further = 0;
    // Node being added
    int winner, action = -1;

    for (int i=0;i<m_max_iter;++i) {
        // Check if node exists
        // The parent node always exists; created in get_move if non existent
        if (node_ptr) {
            sim_path.push_back(node_ptr);
            // Check if fully connected
            if (node_ptr->m_fully_connected) {
                // Choose a move using UCT
                float UCT_value = -1;
                float mult = (player == m_MCTS_player) ? 1 : -1;
                for (auto i=node_ptr->childrenBegin; i<node_ptr->childrenEnd;++i) {
                    float m = mult * (*i).m_wins / (*i).m_visits + m_C * std::sqrt(std::log(node_ptr->m_visits) /  (*i).m_visits);
                    if (m > UCT_value) {
                        UCT_value = m;
                        action = (*i).m_action;
                        next_node_ptr = i;
                    }
                }
                node_ptr = next_node_ptr;
            }
            // If not fully connected and expand
            else if (expand) {
                expand = 0;
                int tot_moves, target_pos = node_ptr->childrenEnd - node_ptr->childrenBegin;
                // We take the next child node in order
                get_move_sim(state1, state2, player, target_pos, action, tot_moves);

                // Case when no legal moves available
                if (!tot_moves) {
                    tot_moves = 1;
                }

                if (target_pos == tot_moves - 1) {
                    node_ptr->m_fully_connected = 1;
                }

                node_ptr = node_ptr->childrenEnd++;
                node_ptr->m_action = action;
                node_ptr->m_fully_connected = 0;
                node_ptr->m_visits = 0;
                node_ptr->m_wins = 0;

                expand_further = 1;
            }
            // This means expand_further = True; we need to keep it for endgame cases
            else {
                expand_further = 0;
                int tot_moves;
                get_move_sim(state1, state2, player, 0, action, tot_moves);

                node_ptr->childrenBegin = m_pool_ptr;
                node_ptr->childrenEnd = m_pool_ptr;
                m_pool_ptr += tot_moves;

                node_ptr = nullptr;
            }
        }
        else {
            // Random move
            int tot_moves;
            get_move_sim(state1, state2, player, gen(), action, tot_moves);
        }

        // Compute next states
        winner = m_game_ptr->make_move(action, state1, state2, player);

        // Check if victory condition
        if (winner != -1) {
            break;
        }
    }

    // This means it's a terminal node
    if (expand_further) {
        node_ptr->childrenBegin = m_pool_ptr;
        node_ptr->childrenEnd = m_pool_ptr;
        node_ptr->m_fully_connected = true;
    }

    // Add to path if no expansion / expand further
    if (node_ptr) {
        sim_path.push_back(node_ptr);
    }

    //std::cout << "Path size:" << sim_path.size() << std::endl;

    // Update values from path
    if (winner == m_MCTS_player) {   
        for (auto i=0;i<sim_path.size();++i) {
            ++sim_path[i]->m_visits;
            ++sim_path[i]->m_wins;
        }
    }
    else if (winner == -2) {
        for (auto i=0;i<sim_path.size();++i) {
            ++sim_path[i]->m_visits;
            sim_path[i]->m_wins += 0.5;
        }
    }
    else {
        for (auto i=0;i<sim_path.size();++i) {
            ++sim_path[i]->m_visits;
        }
    }
}

void MCTS::get_move(int opp_action, int &action) {
    //grid, action are new moves
    m_max_depth = 0;
    
    // Create a dummy node
    bool new_state = true;

    if (m_node_ptr) {
        // We need to check if the state is a recorded one
        for (auto i=m_node_ptr->childrenBegin;i<m_node_ptr->childrenEnd;++i) {
            if (i->m_action == opp_action) {
                new_state = false;
                m_node_ptr = i;
                break;
            }
        }
    }

    // Create a new node if necessary
    if (new_state) {
        m_node_ptr = m_pool_ptr++;
        m_node_ptr->m_visits = 0;
        m_node_ptr->m_wins = 0;
        m_node_ptr->m_action = opp_action;
        m_node_ptr->m_fully_connected = false;
        m_node_ptr->childrenBegin = m_pool_ptr;
        m_node_ptr->childrenEnd = m_pool_ptr;
        m_pool_ptr += std::max(get_num_move_sim(), 1);
    }

    
    // Run the MCTS
    int num_iter = 0;
    Clock::time_point start_time = Clock::now();

    while (std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_time).count() < m_calculation_time)
    {
        run_simulation();
        ++num_iter;
    }
    
    
    /*
    int num_iter = 0;
    Clock::time_point start_time = Clock::now();

    while (num_iter < 50)
    {
        run_simulation();
        ++num_iter;
    }
    */
    


    Clock::time_point end_time = Clock::now();
    std::cout << "Number of iterations: " << num_iter << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count() << std::endl;
    std::cout << "Nodes: " << m_pool_ptr - &pool[0] << std::endl;

    float win_ratio = -1;
    Node *new_ptr = nullptr;
    for (auto i=m_node_ptr->childrenBegin;i<m_node_ptr->childrenEnd;++i)
    {
        float m = i->m_wins / i->m_visits;
        if (m > win_ratio)
        {
            win_ratio = m;
            action = i->m_action;
            new_ptr = i;
        }
    }
    //std::cout << (*m_node_ptr) << std::endl;
    m_node_ptr = new_ptr;
    //std::cout << "Expected victory chances:" << win_ratio << std::endl;
    return ;
}
