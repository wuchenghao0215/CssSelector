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
                        p->_right_sibling = node;
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

            regex r_attr(R"((\w+)=\"([^\"]*)\")");
            smatch m;
            string attr_name, attr_value;
            map<string, string> attributes;
            string tag_copy = tag;
            while (regex_search(tag_copy, m, r_attr)) {
                attr_name = m[1];
                attr_value = m[2];
                if (attr_name == "class") {
                    class_name = attr_value;
                } else if (attr_name == "id") {
                    id_name = attr_value;
                } else {
                    attributes[attr_name] = attr_value;
                }
                tag_copy = m.suffix();
            }

            if (unclosed_tag.count(tag_name) != 0) {
                // if the tag doesn't need to be closed
                auto *node = new Node(tag_name, class_name, id_name, tag);
                node->_attributes = attributes;
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
                node->_attributes = attributes;
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

std::vector<Node *> DomTree::traversal() const {
    vector<Node *> nodes;
    queue<Node *> q;
    q.push(_root);
    while (!q.empty()) {
        if (q.front()->_left_child != nullptr) {
            q.push(q.front()->_left_child);
        }
        if (q.front()->_right_sibling != nullptr) {
            q.push(q.front()->_right_sibling);
        }
        nodes.push_back(q.front());
        q.pop();
    }
    return nodes;
}

std::vector<string> split(const string &class_name) {
    vector<string> classes;
    // format: "class1 class2 class3"
    istringstream ss(class_name);
    string class_name_;
    while (ss >> class_name_) {
        classes.push_back(class_name_);
    }
    return classes;
}

std::vector<Node *>
DomTree::select_1(const std::string &tag_name, const std::string &class_name, const std::string &id_name) const {
    vector<Node *> nodes;
    queue<Node *> q;
    q.push(_root);
    while (!q.empty()) {
        if (q.front()->_left_child != nullptr) {
            q.push(q.front()->_left_child);
        }
        if (q.front()->_right_sibling != nullptr) {
            q.push(q.front()->_right_sibling);
        }
        // class_name format: class_name="class1 class2 class3"
        // split class_name by space
        vector<string> classes = split(q.front()->_class_name);
        if ((tag_name.empty() or q.front()->_tag_name == tag_name) and
            (class_name.empty() or find(classes.begin(), classes.end(), class_name) != classes.end()) and
            (id_name.empty() or q.front()->_id_name == id_name)) {
            nodes.push_back(q.front());
        }
        q.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_2(const std::string &class_1, const std::string &class_2) const {
    vector<Node *> nodes;
    queue<Node *> q;
    q.push(_root);
    while (!q.empty()) {
        if (q.front()->_left_child != nullptr) {
            q.push(q.front()->_left_child);
        }
        if (q.front()->_right_sibling != nullptr) {
            q.push(q.front()->_right_sibling);
        }
        // class_name format: class_name="class1 class2 class3"
        // split class_name by space
        vector<string> classes = split(q.front()->_class_name);
        bool has_class_1 = false, has_class_2 = false;
        for (const auto &class_: classes) {
            if (class_ == class_1) {
                has_class_1 = true;
            }
            if (class_ == class_2) {
                has_class_2 = true;
            }
            if (has_class_1 and has_class_2) {
                nodes.push_back(q.front());
                break;
            }
        }
        q.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_3(const std::string &class_1, const std::string &class_2) const {
    vector<Node *> nodes;
    queue<Node *> q1, q2;
    q1.push(_root);
    while (!q1.empty()) {
        if (q1.front()->_left_child != nullptr) {
            q1.push(q1.front()->_left_child);
        }
        if (q1.front()->_right_sibling != nullptr) {
            q1.push(q1.front()->_right_sibling);
        }
        // class_name format: class_name="class1 class2 class3"
        // split class_name by space
        vector<string> classes = split(q1.front()->_class_name);
        for (const auto &class_: classes) {
            if (class_ == class_1) {
                if (q1.front()->_left_child != nullptr) {
                    q2.push(q1.front()->_left_child);
                }
                break;
            }
        }
        q1.pop();
    }
    while (!q2.empty()) {
        if (q2.front()->_right_sibling != nullptr) {
            q2.push(q2.front()->_right_sibling);
        }
        // class_name format: class_name="class1 class2 class3"
        // split class_name by space
        vector<string> classes = split(q2.front()->_class_name);
        for (const auto &class_: classes) {
            if (class_ == class_2) {
                nodes.push_back(q2.front());
                break;
            }
        }
        q2.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_4(const std::string &tag_1, const std::string &tag_2) const {
    vector<Node *> nodes;
    queue<Node *> q;
    q.push(_root);
    while (!q.empty()) {
        if (q.front()->_left_child != nullptr) {
            q.push(q.front()->_left_child);
        }
        if (q.front()->_right_sibling != nullptr) {
            q.push(q.front()->_right_sibling);
        }
        if (q.front()->_tag_name == tag_1 or q.front()->_tag_name == tag_2) {
            nodes.push_back(q.front());
        }
        q.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_5(const std::string &tag_1, const std::string &tag_2) const {
    vector<Node *> nodes;
    queue<Node *> q1, q2;
    q1.push(_root);
    while (!q1.empty()) {
        if (q1.front()->_left_child != nullptr) {
            q1.push(q1.front()->_left_child);
        }
        if (q1.front()->_right_sibling != nullptr) {
            q1.push(q1.front()->_right_sibling);
        }
        if (q1.front()->_tag_name == tag_1) {
            if (q1.front()->_left_child != nullptr) {
                q2.push(q1.front()->_left_child);
            }
        }
        q1.pop();
    }
    while (!q2.empty()) {
        if (q2.front()->_left_child != nullptr) {
            q2.push(q2.front()->_left_child);
        }
        if (q2.front()->_right_sibling != nullptr) {
            q2.push(q2.front()->_right_sibling);
        }
        if (q2.front()->_tag_name == tag_2) {
            nodes.push_back(q2.front());
        }
        q2.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_6(const std::string &tag_1, const std::string &tag_2) const {
    vector<Node *> nodes;
    queue<Node *> q1, q2;
    q1.push(_root);
    while (!q1.empty()) {
        if (q1.front()->_left_child != nullptr) {
            q1.push(q1.front()->_left_child);
        }
        if (q1.front()->_right_sibling != nullptr) {
            q1.push(q1.front()->_right_sibling);
        }
        if (q1.front()->_tag_name == tag_1) {
            if (q1.front()->_left_child != nullptr) {
                q2.push(q1.front()->_left_child);
            }
        }
        q1.pop();
    }
    while (!q2.empty()) {
        if (q2.front()->_right_sibling != nullptr) {
            q2.push(q2.front()->_right_sibling);
        }
        if (q2.front()->_tag_name == tag_2) {
            nodes.push_back(q2.front());
        }
        q2.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_7(const std::string &tag_1, const std::string &tag_2) const {
    vector<Node *> nodes;
    queue<Node *> q;
    q.push(_root);
    while (!q.empty()) {
        if (q.front()->_left_child != nullptr) {
            q.push(q.front()->_left_child);
        }
        if (q.front()->_right_sibling != nullptr) {
            q.push(q.front()->_right_sibling);
        }
        if (q.front()->_tag_name == tag_1 and
            q.front()->_right_sibling != nullptr and
            q.front()->_right_sibling->_tag_name == tag_2) {
            nodes.push_back(q.front()->_right_sibling);
        }
        q.pop();
    }
    return nodes;
}

std::vector<Node *> DomTree::select_8(const std::string &tag_1, const std::string &tag_2) const {
    vector<Node *> nodes;
    queue<Node *> q;
    q.push(_root);
    while (!q.empty()) {
        Node *p1 = q.front(), *p2;
        bool flag = false;
        while (p1 != nullptr) {
            if (p1->_left_child != nullptr) {
                q.push(p1->_left_child);
            }
            if (!flag and p1->_tag_name == tag_1) {
                flag = true;
                p2 = p1->_right_sibling;
                while (p2 != nullptr) {
                    if (p2->_tag_name == tag_2) {
                        nodes.push_back(p2);
                    }
                    p2 = p2->_right_sibling;
                }
            }
            p1 = p1->_right_sibling;
        }
        q.pop();
    }
    return nodes;
}
