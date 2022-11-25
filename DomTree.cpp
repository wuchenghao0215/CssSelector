//
// Created by Wu Chenghao on 2022/11/24.
//

#include "DomTree.h"

#include <set>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

DomTree::DomTree() {
    _root = new Node();
}

void DomTree::build_tree(const std::string &content) {
    set<string> unclosed_tag = {"br", "hr", "base", "meta", "img", "input", "link"};
    stack<Node *> nodes;
    string tag, text, script, tag_name, class_name, id_name;
    bool is_tag = false, is_comment = false, is_script = false;
    for (char i: content) {
        if (is_comment) {
            tag.push_back(i);
            if (tag.size() >= 3 and tag.substr(tag.size() - 3, tag.size()) == "-->") {
                is_comment = false;
                tag.clear();
                is_tag = false;
            }
        } else if (is_script) {
            script.push_back(i);
            if (script.size() >= 9 and script.substr(script.size() - 9, script.size()) == "</script>") {
                is_script = false;
                nodes.top()->_close_tag = "/script";
                script.erase(script.size() - 9, 9);
                if (!script.empty()) {
                    size_t n = script.find_last_not_of(" \n\r\t");
                    if (n != string::npos) {
                        script.erase(n + 1, script.size() - n);
                    }
                    n = script.find_first_not_of(" \n\r\t");
                    if (n != 0) {
                        script.erase(0, n);
                    }
                }
                if (!script.empty()) {
                    auto node = new Node("iScript", "", "", "", "", script);
                    node->_parent = nodes.top();
                    if (nodes.top()->_left_child == nullptr) {
                        nodes.top()->_left_child = node;
                    } else {
                        Node *p = nodes.top()->_left_child;
                        while (p->_right_sibling != nullptr) {
                            p = p->_right_sibling;
                        }
                        p->_right_sibling= node;
                    }
                    script.clear();
                }
                nodes.pop();
            }
        } else if (i == '<') {
            is_tag = true;
            if (!text.empty()) {
                size_t n = text.find_last_not_of(" \n\r\t");
                if (n != string::npos) {
                    text.erase(n + 1, text.size() - n);
                }
                n = text.find_first_not_of(" \n\r\t");
                if (n != 0) {
                    text.erase(0, n);
                }
            }
            if (!text.empty()) {
                auto *node = new Node("iText", "", "", "", "", text);
                if (!nodes.empty()) {
                    node->_parent = nodes.top();
                } else {
                    node->_parent = _root;
                }
                if (node->_parent->_left_child == nullptr) {
                    node->_parent->_left_child = node;
                } else {
                    Node *p = node->_parent->_left_child;
                    while (p->_right_sibling != nullptr) {
                        p = p->_right_sibling;
                    }
                    p->_right_sibling = node;
                }
                text.clear();
            }
        } else if (is_tag and i == '>') {
            is_tag = false;
            if (tag[0] == '!') {
                // ignore !DOCTYPE
                tag.clear();
                continue;
            }

            istringstream ss(tag);
            ss >> tag_name;

            if (tag_name == "script") {
                is_script = true;
            }

            regex r_class("class=\"([^\"]+)\"");
            regex r_id("id=\"([^\"]+)\"");
            smatch m;
            if (regex_search(tag, m, r_class)) {
                class_name = m[1];
            }
            if (regex_search(tag, m, r_id)) {
                id_name = m[1];
            }

            if (unclosed_tag.count(tag_name) != 0) {
                // if the tag doesn't need to be closed
                auto *node = new Node(tag_name, class_name, id_name, tag);
                if (!nodes.empty()) {
                    node->_parent = nodes.top();
                } else {
                    node->_parent = _root;
                }
                if (node->_parent->_left_child == nullptr) {
                    node->_parent->_left_child = node;
                } else {
                    Node *p = node->_parent->_left_child;
                    while (p->_right_sibling != nullptr) {
                        p = p->_right_sibling;
                    }
                    p->_right_sibling = node;
                }
            } else if (tag_name[0] != '/') {
                // if the tag need closing and is an opening tag
                auto *node = new Node(tag_name, class_name, id_name, tag);
                if (!nodes.empty()) {
                    node->_parent = nodes.top();
                } else {
                    node->_parent = _root;
                }
                if (node->_parent->_left_child == nullptr) {
                    node->_parent->_left_child = node;
                } else {
                    Node *p = node->_parent->_left_child;
                    while (p->_right_sibling != nullptr) {
                        p = p->_right_sibling;
                    }
                    p->_right_sibling = node;
                }
                nodes.push(node);
                text.clear();
            } else {
                // if the tag is a closing tag
                if (tag_name.substr(1, tag_name.size()) != nodes.top()->_tag_name) {
                    throw runtime_error("Tag not closed");
                }
                nodes.top()->_close_tag = tag;
                nodes.pop();
            }

            tag.clear();
            class_name.clear();
            id_name.clear();
        } else if (is_tag) {
            tag.push_back(i);
            if (tag == "!--") {
                is_comment = true;
            }
        } else {
            text.push_back(i);
        }
    }
}

DomTree::~DomTree() {
    queue<Node *> nodes;
    nodes.push(_root);
    while (!nodes.empty()) {
        if (nodes.front()->_left_child != nullptr) {
            nodes.push(nodes.front()->_left_child);
        }
        if (nodes.front()->_right_sibling != nullptr) {
            nodes.push(nodes.front()->_right_sibling);
        }
        delete nodes.front();
        nodes.pop();
    }
}
