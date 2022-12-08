//
// Created by Wu Chenghao on 2022/11/24.
//

#ifndef ASSIGNMENT3_NODE_H
#define ASSIGNMENT3_NODE_H


#include <string>
#include <vector>
#include <map>

class Node {
public:
    friend class DomTree;

    Node();

    Node(std::string tag_name, std::string class_name, std::string id_name, std::string open_tag);

    Node(std::string tag_name, std::string class_name, std::string id_name,
         std::string open_tag, std::string close_tag, std::string text);

    void print_outer_html(int indent) const;

    void print_text() const;

    void print_open_tag() const;

    bool is_text() const;

    void print_href() const;

    void print_attribute(const std::string &attr) const;

private:
    std::string _tag_name;
    std::string _class_name;
    std::string _id_name;
    std::map<std::string, std::string> _attributes;
    std::string _open_tag;
    std::string _close_tag;
    std::string _text;
    Node *_parent;
    Node *_left_child;
    Node *_right_sibling;
};


#endif //ASSIGNMENT3_NODE_H
