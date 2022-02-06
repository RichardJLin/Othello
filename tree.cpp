#include "tree.h"

/*
std::ostream& operator<<(std::ostream& os, const Node& node)
{
    os << "Node id: " << &node << " (" << node.m_grid << "," << node.m_action << ") " << node.m_visits << " " << node.m_wins << std::endl;
    os << "Children id: ";
    for (auto i=node.childrenBegin; i<node.childrenEnd; ++i)
    {
        os << i << " (" << i->m_grid << "," << i->m_action << ") ";
    }
    os << std::endl;
    return os;
}
*/

std::ostream& operator<<(std::ostream& os, const Node& node)
{
    os << "Node: " << "(" << node.m_action/8 << ',' << node.m_action%8 << ") " << node.m_wins/node.m_visits << std::endl;
    os << "Children: ";
    for (auto i=node.childrenBegin; i<node.childrenEnd; ++i)
    {
        os << "(" << i->m_action/8 << "," << i->m_action%8 << ") " << i->m_wins/i->m_visits << "  ";
    }
    os << std::endl;
    return os;
}

/*
int main()
{
    Node node[3];
    //std::cout << node[0] << std::endl << node[1] << std::endl << node[2];
    Node *ptr = &node[0];
    node[0].set_values(nullptr);
    std::cout << "test start \n" << *ptr << "test end \n";
    std::cout << node[0] << std::endl << node[1] << std::endl << node[2];
}
*/