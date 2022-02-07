#include "MCTS.h"
#include <unistd.h>

void printMove(int action) {
    if (action != -1) {
        std::cout << "Chosen move: " << '(' << action / 8 << ',' << mod(action,8) << ')' << std::endl;
    }
    else {
        std::cout << "Chosen move: (0,-1)" << std::endl;
    }
}

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

                printMove(action);
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

                printMove(action);
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

                printMove(action1);
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

                printMove(action2);
                winner = o.make_move_self(action2);

                if (winner != -1) {
                    std::cout << "Winner: " << winner << std::endl;
                    std::cout << o << std::endl;
                    break;
                }
            }
        }
        break;

    case 5 : 
    {
        int wins0(0), wins1(0), draws(0);
        for (int i=0;i<100;++i) {
            Othello o;
            MCTS m(&o,1);
            MCTS m2(&o,0);
            int action1(-1), action2(-1), winner;
            
            while (true) {

                m.get_move(action2,action1);
                winner = o.make_move_self(action1);

                if (winner != -1) {
                    break;
                }       

                m2.get_move(action1,action2);
                winner = o.make_move_self(action2);

                if (winner != -1) {
                    break;
                }
            }
            switch (winner) {
                case 0:
                    ++wins0;
                    break;
                case 1:
                    ++wins1;
                    break;
                default:
                    ++draws;
                    break;
            }
        }
        std::cout << "P1: " << wins1 << "\nP2: " << wins0 << "\nDraws: " << draws << std::endl;

    }
        break;

    default:
        std::cout << "Unavailable\n";
        break;
    }
}