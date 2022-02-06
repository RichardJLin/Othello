#ifndef DEF_TREE
#define DEF_TREE

#include <vector>
#include <iostream>

class Node {
public:
    Node* childrenBegin;
    Node* childrenEnd;
    int m_action;
    float m_visits;
    float m_wins;
    bool m_fully_connected;

    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};

#endif