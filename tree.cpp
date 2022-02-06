#include "tree.h"

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