//
// Created by Wu Chenghao on 2022/11/24.
//

#include "Node.h"

Node::Node() {
    type = "root";
    opening_tag = "";
    closing_tag = "";
    element = "";
    parent = nullptr;
    left_child = nullptr;
    right_sibling = nullptr;
}

Node::Node(std::string t, std::string o) {
    type = std::move(t);
    opening_tag = std::move(o);
    closing_tag = "";
    element = "";
    parent = nullptr;
    left_child = nullptr;
    right_sibling = nullptr;
}

Node::Node(std::string t, std::string o, std::string c, std::string e) {
    type = std::move(t);
    opening_tag = std::move(o);
    closing_tag = std::move(c);
    element = std::move(e);
    parent = nullptr;
    left_child = nullptr;
    right_sibling = nullptr;
}

Node::~Node() = default;
