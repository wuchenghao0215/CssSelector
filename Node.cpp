//
// Created by Wu Chenghao on 2022/11/24.
//

#include "Node.h"

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

Node::Node(std::string tag_name, std::string class_name, std::string id_name, std::string open_tag, std::string close_tag, std::string text) {
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
