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
        // selector format:
        vector<regex> selector_regex;
        {
            // 0. *
            selector_regex.emplace_back(R"(\*)");
            // 1. tag.class#id
            selector_regex.emplace_back(R"((\w+)?(\.\w+)?(#\w+)?(\[\w+\])?)");
            // 2. .class1.class2
            selector_regex.emplace_back(R"((\.\w+)+)");
            // 3. .class1 .class2
            selector_regex.emplace_back(R"((\.\w+)(\s\.\w+)*)");
            // 4. tag1, tag2
            selector_regex.emplace_back(R"((\w+)(,\s?\w+)*)");
            // 5. tag1 tag2
            selector_regex.emplace_back(R"((\w+)(\s\w+)*)");
            // 6. tag1 > tag2
            selector_regex.emplace_back(R"((\w+)(\s?>\s?\w+)*)");
            // 7. tag1 + tag2
            selector_regex.emplace_back(R"((\w+)(\s?\+\s?\w+)*)");
            // 8. tag1 ~ tag2
            selector_regex.emplace_back(R"((\w+)(\s?~\s?\w+)*)");
        }

        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);
            if (input == "exit") {
                break;
            }
            // input format:  "css.selector("*")"
            regex input_regex("css\\.selector\\(\"(.*)\"\\)");
            smatch match_input;
            if (regex_match(input, match_input, input_regex)) {
                string selector = match_input[1];
                bool is_valid = false;
                for (int i = 0; i < selector_regex.size(); ++i) {
                    smatch match_selector;
                    if (regex_match(selector, match_selector, selector_regex[i])) {
                        is_valid = true;
                        switch (i) {
                            case 0:
                                cout << "valid selector: *" << endl;
                                break;
                            case 1:
                                cout << "valid selector: tag.class#id" << endl;
                                break;
                            case 2:
                                cout << "valid selector: .class1.class2" << endl;
                                break;
                            case 3:
                                cout << "valid selector: .class1 .class2" << endl;
                                break;
                            case 4:
                                cout << "valid selector: tag1, tag2" << endl;
                                break;
                            case 5:
                                cout << "valid selector: tag1 tag2" << endl;
                                break;
                            case 6:
                                cout << "valid selector: tag1 > tag2" << endl;
                                break;
                            case 7:
                                cout << "valid selector: tag1 + tag2" << endl;
                                break;
                            case 8:
                                cout << "valid selector: tag1 ~ tag2" << endl;
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                }
                if (!is_valid) {
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
