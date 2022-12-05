//
// Created by Wu Chenghao on 2022/11/24.
//

#ifndef ASSIGNMENT3_DOMTREE_H
#define ASSIGNMENT3_DOMTREE_H


#include <vector>

#include "Node.h"

class DomTree {
public:
    DomTree();

    void build_tree(const std::string &content);

    ~DomTree();

    std::vector<Node *> traversal() const;

    std::vector<Node *>
    select_1(const std::string &tag_name, const std::string &class_name, const std::string &id_name) const;

    std::vector<Node *> select_2(const std::string &class_1, const std::string &class_2) const;

    std::vector<Node *> select_3(const std::string &class_1, const std::string &class_2) const;

    std::vector<Node *> select_4(const std::string &tag_1, const std::string &tag_2) const;

    std::vector<Node *> select_5(const std::string &tag_1, const std::string &tag_2) const;

    std::vector<Node *> select_6(const std::string &tag_1, const std::string &tag_2) const;

    std::vector<Node *> select_7(const std::string &tag_1, const std::string &tag_2) const;

    std::vector<Node *> select_8(const std::string &tag_1, const std::string &tag_2) const;

private:
    Node *_root;
};


#endif //ASSIGNMENT3_DOMTREE_H
