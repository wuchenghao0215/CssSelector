//
// Created by Wu Chenghao on 2022/11/24.
//

#ifndef ASSIGNMENT3_NODE_H
#define ASSIGNMENT3_NODE_H


#include <string>

class Node {
public:
    friend class DomTree;

    Node();

    Node(std::string t, std::string o);

    Node(std::string t, std::string o, std::string c, std::string e);

    ~Node();

private:
    std::string type;
    std::string opening_tag;
    std::string closing_tag;
    std::string element;
    Node *parent;
    Node *left_child;
    Node *right_sibling;
};


#endif //ASSIGNMENT3_NODE_H
