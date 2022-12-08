//
// Created by Wu Chenghao on 2022/11/24.
//

#include "Node.h"

#include <iostream>

using namespace std;

Node::Node() {
    _tag_name = "_root";
    _class_name = "";
    _open_tag = "";
    _close_tag = "";
    _text = "";
    _parent = nullptr;
    _left_child = nullptr;
    _right_sibling = nullptr;
}

Node::Node(std::string tag_name, std::string class_name, std::string id_name, std::string open_tag) {
    _tag_name = std::move(tag_name);
    _class_name = std::move(class_name);
    _id_name = std::move(id_name);
    _open_tag = std::move(open_tag);
    _close_tag = "";
    _text = "";
    _parent = nullptr;
    _left_child = nullptr;
    _right_sibling = nullptr;
}

Node::Node(std::string tag_name, std::string class_name, std::string id_name, std::string open_tag,
           std::string close_tag, std::string text) {
    _tag_name = std::move(tag_name);
    _class_name = std::move(class_name);
    _id_name = std::move(id_name);
    _open_tag = std::move(open_tag);
    _close_tag = std::move(close_tag);
    _text = std::move(text);
    _parent = nullptr;
    _left_child = nullptr;
    _right_sibling = nullptr;
}

void Node::print_outer_html(int indent) const {
    for (int i = 0; i < indent; ++i) {
        cout << "    ";
    }
    if (!_open_tag.empty()) {
        cout << '<' << _open_tag << '>' << endl;
    } else {    // is iText
        cout << _text << endl;
        return;
    }
    if (!_close_tag.empty()) {  // has child(ren)
        Node *p = _left_child;
        while (p != nullptr) {
            p->print_outer_html(indent + 1);
            p = p->_right_sibling;
        }
        for (int i = 0; i < indent; ++i) {
            cout << "    ";
        }
        cout << '<' << _close_tag << '>' << endl;
    }
}

void Node::print_text() const {
    if (_open_tag.empty()) {    // iText
        cout << _text << endl;
    } else {
        Node *p = _left_child;
        while (p != nullptr) {
            p->print_text();
            p = p->_right_sibling;
        }
    }
}

void Node::print_open_tag() const {
    if (_open_tag.empty()) {
        return;
    }
    if (_open_tag.size() < 80) {
        cout << '<' << _open_tag << '>';
    } else {
        cout << '<' << _open_tag.substr(0, 80) << "...>";
    }
}

bool Node::is_text() const {
    return _open_tag.empty();
}

void Node::print_href() const {
    for (const auto &attr: _attributes) {
        if (attr.first == "href") {
            cout << attr.second << endl;
            return;
        }
    }
    cout << "href not found" << endl;
}

void Node::print_attribute(const std::string &attr) const {
    for (const auto &attribute: _attributes) {
        if (attribute.first == attr) {
            cout << attribute.second << endl;
            return;
        }
    }
    cout << attr << " not found" << endl;
}
