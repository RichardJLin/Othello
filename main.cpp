#include "MCTS.h"
#include <unistd.h>

int main()
{
    int play_mode;
    std::cout << "1.P1 CPU P2 Human\n2.P1 Human P2 CPU \n3.P1 Human P2 Human\n4.P1 CPU P2 CPU\n";
    std::cin >> play_mode;
    switch (play_mode) {
    case 1:
        {
            Othello o;
            MCTS m(&o,1);

            int opp_action = -1; // will create a dummy node
            int oppCol,oppLine, action, winner;
            
            while (true) {
                std::cout << "Current player: " << o.m_current_player << std::endl;
                std::cout << o << std::endl;

                if (m.m_node_ptr) {
                    std::cout << *m.m_node_ptr << std::endl;
                }

                m.get_move(opp_action,action);

                std::cout << "Chosen move: " << '(' << action / 8 << ',' << mod(action,8) << ')' << std::endl;
                winner = o.make_move_self(action);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }

                std::cout << "Current player: " << o.m_current_player << std::endl;
                std::cout << o << std::endl;
                std::cout << *m.m_node_ptr << std::endl;

                std::cin >> oppLine >> oppCol;
                opp_action = 8*oppLine + oppCol;
                winner = o.make_move_self(opp_action);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }
            }
        }
        break;

    case 2:
        {
            Othello o;
            MCTS m(&o,0);

            int action, opp_action, oppLine, oppCol, winner;
            
            while (true) {
                std::cout << "Current player: " << o.m_current_player << std::endl;
                std::cout << o << std::endl;
                if (m.m_node_ptr) {
                    std::cout << *m.m_node_ptr << std::endl;
                }

                std::cin >> oppLine >> oppCol;
                opp_action = 8*oppLine + oppCol;
                winner = o.make_move_self(opp_action);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }

                std::cout << "Current player: " << o.m_current_player << std::endl;
                std::cout << o << std::endl;

                if (m.m_node_ptr) {
                    std::cout << *m.m_node_ptr << std::endl;
                }
                
                m.get_move(opp_action,action);

                std::cout << "Chosen move: " << '(' << action / 8 << ',' << mod(action,8) << ')' << std::endl;
                winner = o.make_move_self(action);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }
            }
        }
        break;

    case 3:
        {
            Othello o;
            int action, Line, Col, winner;

            while (true) {
                std::cout << "Current player: " << o.m_current_player << std::endl;
                std::cout << o << std::endl;

                std::cin >> Line >> Col;
                action = 8 * Line + Col;
                winner = o.make_move_self(action);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }
            }
        }
        break;

    case 4 : 
        {
            Othello o;
            MCTS m(&o,1);
            MCTS m2(&o,0);
            int action1(-1), action2(-1), winner;
            
            while (true) {
                std::cout << std::endl << "Current player: " << o.m_current_player << std::endl;
                std::cout << o ;
                usleep(500000);

                m.get_move(action2,action1);

                std::cout << "Chosen move: " << '(' << action1 / 8 << ',' << mod(action1,8) << ')' << std::endl;
                winner = o.make_move_self(action1);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }       

                std::cout << std::endl << "Current player: " << o.m_current_player << std::endl;
                std::cout << o ;
                usleep(500000);

                m2.get_move(action1,action2);

                std::cout << "Chosen move: " << '(' << action2 / 8 << ',' << mod(action2,8) << ')' << std::endl;
                winner = o.make_move_self(action2);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }
            }
        }
        break;
    
    default:
        std::cout << "Unavailable\n";
        break;
    }
}

/*
int main()
{
    TicTacToes t(1);
    MCTS m(&t,1);
    if (!m.m_visited_states.contains(m.m_game->m_state))
    {
        Node *node_ptr = &m.pool[++m.m_node_pos];
        m.m_game->compute_possible_moves(m.m_game->m_board1|m.m_game->m_board2,m.m_game->m_summary_board_all,m.m_game->m_true_last_board_pos,node_ptr->m_possible_moves);
        node_ptr->set_values(m.m_node);
        m.m_visited_states[m.m_game->m_state] = node_ptr;
    }
    m.m_node = m.m_visited_states[m.m_game->m_state];

    m.run_simulation();
}
*/

/*
// Speed test
int main()
{
    int n;
    std::cout << "Number of repetitions: ";
    std::cin >> n;

    ProgressBar bar(std::cout);
    bar.init();

    TicTacToes t(1);

    int s=0;
    for (int i=0;i<n;++i)
    {
        MCTS m(&t,1);
        s += m.get_move_test();

        bar.update(i, n);
    }
    std::cout << std::endl;
    std::cout << "Average number of games: " << (double)s/n << std::endl;
    return 0;
}
*/

