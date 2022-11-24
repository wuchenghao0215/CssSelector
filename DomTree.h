//
// Created by Wu Chenghao on 2022/11/24.
//

#ifndef ASSIGNMENT3_DOMTREE_H
#define ASSIGNMENT3_DOMTREE_H


#include "Node.h"

class DomTree {
public:
    DomTree();

    explicit DomTree(const std::string &content);

    ~DomTree();

    bool check() const;

private:
    Node *root;
    bool is_legal;
};


#endif //ASSIGNMENT3_DOMTREE_H
