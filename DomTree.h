//
// Created by Wu Chenghao on 2022/11/24.
//

#ifndef ASSIGNMENT3_DOMTREE_H
#define ASSIGNMENT3_DOMTREE_H


#include "Node.h"

class DomTree {
public:
    DomTree();

    void build_tree(const std::string &content);

    ~DomTree();

private:
    Node *_root;
};


#endif //ASSIGNMENT3_DOMTREE_H
