//
// Created by Wu Chenghao on 2022/11/24.
//

#include "DomTree.h"

#include <set>
#include <stack>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

DomTree::DomTree() {
    root = new Node();
    is_legal = false;
}

DomTree::DomTree(const std::string &content) {
    root = new Node();
    is_legal = false;
    set<string> unclosed_tag = {"br", "hr", "base", "meta", "img", "input", "link"};
    stack<Node *> nodes;
    string tag, element, type;
    bool is_tag, is_comment;
    for (char i: content) {
        if (is_comment) {
            tag.push_back(i);
            if (tag.substr(tag.length() - 3, tag.length()) == "-->") {
                is_comment = false;
                tag.clear();
                is_tag = false;
            }
        } else if (i == '<') {
            is_tag = true;
            if (!element.empty()) {
                size_t n = element.find_last_not_of(" \n\r\t");
                if (n != string::npos) {
                    element.erase(n + 1, element.size() - n);
                }
                n = element.find_first_not_of(" \n\r\t");
                if (n != 0) {
                    element.erase(0, n);
                }
            }
            if (!element.empty()) {
                auto *node = new Node("IText", "", "", element);
                if (!nodes.empty()) {
                    node->parent = nodes.top();
                } else {
                    node->parent = root;
                }
                if (node->parent->left_child == nullptr) {
                    node->parent->left_child = node;
                } else {
                    Node *p = node->parent->left_child;
                    while (p->right_sibling != nullptr) {
                        p = p->right_sibling;
                    }
                    p->right_sibling = node;
                }
                element.clear();
            }
        } else if (i == '>') {
            is_tag = false;
            istringstream ss(tag);
            ss >> type;
            if (type[0] == '!') {   // ignore commentary
                tag.clear();
                continue;
            }
            if (unclosed_tag.count(type) != 0) {    // if the tag doesn't need closing
                auto *node = new Node(type, tag);
                if (!nodes.empty()) {
                    node->parent = nodes.top();
                } else {
                    node->parent = root;
                }
                if (node->parent->left_child == nullptr) {
                    node->parent->left_child = node;
                } else {
                    Node *p = node->parent->left_child;
                    while (p->right_sibling != nullptr) {
                        p = p->right_sibling;
                    }
                    p->right_sibling = node;
                }
            } else if (type[0] != '/') {    // if the tag need closing and is an opening tag
                auto *node = new Node(type, tag);
                if (!nodes.empty()) {
                    node->parent = nodes.top();
                } else {
                    node->parent = root;
                }
                if (node->parent->left_child == nullptr) {
                    node->parent->left_child = node;
                } else {
                    Node *p = node->parent->left_child;
                    while (p->right_sibling != nullptr) {
                        p = p->right_sibling;
                    }
                    p->right_sibling = node;
                }
                nodes.push(node);
                element.clear();
            } else {    // if the tag is a closing tag
                string t_type = type.substr(1);
                if (t_type != nodes.top()->type) {  // illegal
                    is_legal = false;
                    break;
                } else {
                    nodes.top()->closing_tag = tag;
                    nodes.pop();
                }
            }
            tag.clear();
        } else if (is_tag) {
            tag.push_back(i);
            if (tag == "!--") {
                is_comment = true;
            }
        } else {
            element.push_back(i);
        }
    }
    if (nodes.empty()) {
        is_legal = true;
    }
}

DomTree::~DomTree() {
    queue<Node *> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        if (nodes.front()->left_child != nullptr) {
            nodes.push(nodes.front()->left_child);
        }
        if (nodes.front()->right_sibling != nullptr) {
            nodes.push(nodes.front()->right_sibling);
        }
        delete nodes.front();
        nodes.pop();
    }
}

bool DomTree::check() const {
    return is_legal;
}
