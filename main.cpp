#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

#include "DomTree.h"

using namespace std;

int main() {
    // read HTML file
    string html_content;
    {
        while (html_content.empty()) {
            cout << "please enter file path > ";
            string path;
            getline(cin, path);
            ifstream html_doc;
            html_doc.open(path);
            if (!html_doc.is_open()) {
                cout << "file does not exist, please try again" << endl;
            } else {
                stringstream buffer;
                buffer << html_doc.rdbuf();
                html_content = buffer.str();    // read all
                html_doc.close();
            }
        }
    }

    // parse HTML file to build a DOM tree
    DomTree tree;
    try {
        tree.build_tree(html_content);
    } catch (const runtime_error &error) {
        cout << error.what() << endl;
        return 0;
    }

    // css selector
    {
        // input format
        // "css.selector("xx")[xx].xx()"
        regex regex_input(R"(css\.selector\(\"(.*)\"\)(\[(\d+)\])?(\.(\w+)\(\))?)");

        // selector format
        vector<regex> regex_selector;
        {
            // 0. *
            regex_selector.emplace_back(R"(\*)");
            // 1. tag.class#id
            regex_selector.emplace_back(R"(([\w-]+)?(\.[\w-]+)?(#[\w-]+)?)");
            // 2. .class1.class2.class3
            regex_selector.emplace_back(R"((\.[\w-]+)(\.[\w-]+))");
            // 3. .class1 .class2
            regex_selector.emplace_back(R"((\.[\w-]+)(\s\.[\w-]+))");
            // 4. tag1, tag2
            regex_selector.emplace_back(R"(([\w-]+)(,\s?[\w-]+))");
            // 5. tag1 tag2
            regex_selector.emplace_back(R"(([\w-]+)(\s[\w-]+))");
            // 6. tag1 > tag2
            regex_selector.emplace_back(R"(([\w-]+)(\s?>\s?[\w-]+))");
            // 7. tag1 + tag2
            regex_selector.emplace_back(R"(([\w-]+)(\s?\+\s?[\w-]+))");
            // 8. tag1 ~ tag2
            regex_selector.emplace_back(R"(([\w-]+)(\s?~\s?[\w-]+))");
        }

        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);

            if (input == "exit") {
                break;
            }

            smatch match_input;
            if (regex_match(input, match_input, regex_input)) {
                string selector = match_input[1];
                string index_str = match_input[3];
                string operation = match_input[5];

                smatch match_selector;
                vector<Node *> results;

                int i_selector;
                bool is_valid = false;
                for (i_selector = 0; i_selector < regex_selector.size(); ++i_selector) {
                    if (regex_match(selector, match_selector, regex_selector[i_selector])) {
                        is_valid = true;
                        break;
                    }
                }

                switch (i_selector) {
                    case 0: {
                        // 0. *
                        results = tree.traversal();
                        break;
                    }
                    case 1: {
                        // 1. tag.class#id
                        string tag_name = match_selector[1];
                        string class_name = match_selector[2];
                        string id_name = match_selector[3];
                        if (!class_name.empty()) {
                            class_name = class_name.substr(1);      // remove '.'
                        }
                        if (!id_name.empty()) {
                            id_name = id_name.substr(1);    // remove '#'
                        }
                        results = tree.select_1(tag_name, class_name, id_name);
                        break;
                    }
                    case 2: {
                        // 2. .class1.class2
                        string class_1 = match_selector[1];
                        string class_2 = match_selector[2];
                        class_1 = class_1.substr(1);    // remove '.'
                        class_2 = class_2.substr(1);    // remove '.'
                        results = tree.select_2(class_1, class_2);
                        break;
                    }
                    case 3: {
                        // 3. .class1 .class2
                        string class_1 = match_selector[1];
                        string class_2 = match_selector[2];
                        class_1 = class_1.substr(1);    // remove '.'
                        class_2 = class_2.substr(2);    // remove ' .'
                        results = tree.select_3(class_1, class_2);
                        break;
                    }
                    case 4: {
                        // 4. tag1, tag2
                        string tag_1 = match_selector[1];
                        string tag_2 = match_selector[2];
                        if (tag_2[1] == ' ') {
                            tag_2 = tag_2.substr(2);    // remove ', '
                        } else {
                            tag_2 = tag_2.substr(1);    // remove ','
                        }
                        results = tree.select_4(tag_1, tag_2);
                        break;
                    }
                    case 5: {
                        // 5. tag1 tag2
                        string tag_1 = match_selector[1];
                        string tag_2 = match_selector[2];
                        tag_2 = tag_2.substr(1);    // remove ' '
                        results = tree.select_5(tag_1, tag_2);
                        break;
                    }
                    case 6: {
                        // 6. tag1 > tag2
                        string tag_1 = match_selector[1];
                        string tag_2 = match_selector[2];
                        if (tag_2[0] == ' ') {
                            tag_2 = tag_2.substr(1);    // remove ' '
                        }
                        if (tag_2[1] == ' ') {
                            tag_2 = tag_2.substr(2);    // remove '> '
                        } else {
                            tag_2 = tag_2.substr(1);    // remove '>'
                        }
                        results = tree.select_6(tag_1, tag_2);
                        break;
                    }
                    case 7: {
                        // 7. tag1 + tag2
                        string tag_1 = match_selector[1];
                        string tag_2 = match_selector[2];
                        if (tag_2[0] == ' ') {
                            tag_2 = tag_2.substr(1);    // remove ' '
                        }
                        if (tag_2[1] == ' ') {
                            tag_2 = tag_2.substr(2);    // remove '+ '
                        } else {
                            tag_2 = tag_2.substr(1);    // remove '+'
                        }
                        results = tree.select_7(tag_1, tag_2);
                        break;
                    }
                    case 8: {
                        // 8. tag1 ~ tag2
                        string tag_1 = match_selector[1];
                        string tag_2 = match_selector[2];
                        if (tag_2[0] == ' ') {
                            tag_2 = tag_2.substr(1);    // remove ' '
                        }
                        if (tag_2[1] == ' ') {
                            tag_2 = tag_2.substr(2);    // remove '~ '
                        } else {
                            tag_2 = tag_2.substr(1);    // remove '~'
                        }
                        results = tree.select_8(tag_1, tag_2);
                        break;
                    }
                    default:
                        break;
                }

                if (is_valid) {
                    if (index_str.empty()) {
                        // operate on all results
                        if (operation.empty()) {
                            cout << "[";
                            bool is_first = true;
                            for (auto & result : results) {
                                if (!result->is_text()) {
                                    if (is_first) {
                                        is_first = false;
                                    } else {
                                        cout << ", " << endl;
                                    }
                                    result->print_open_tag();
                                }
                            }
                            cout << "]" << endl;
                        } else if (operation == "text") {
                            for (const auto &result: results) {
                                result->print_text();
                                cout << endl;
                            }
                        } else if (operation == "html") {
                            for (const auto &result: results) {
                                result->print_outer_html(0);
                                cout << endl;
                            }
                        } else {
                            cout << "invalid operation" << endl;
                        }
                    } else {
                        // operate on results[index]
                        int index = stoi(index_str);
                        if (index < results.size()) {
                            Node *result = results[index];
                            if (operation.empty()) {
                                result->print_open_tag();
                            } else if (operation == "text") {
                                result->print_text();
                            } else if (operation == "html") {
                                result->print_outer_html(0);
                            } else if (operation == "href") {
                                result->print_href();
                            } else {
                                cout << "invalid operation" << endl;
                            }
                        } else {
                            cout << "index out of range" << endl;
                        }
                    }
                } else {
                    cout << "invalid selector" << endl;
                    continue;
                }
            } else {
                cout << "invalid input" << endl;
            }
        }
    }

    return 0;
}
