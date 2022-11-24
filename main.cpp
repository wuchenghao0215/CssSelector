#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "DomTree.h"

using namespace std;

int main() {
    // read HTML file
    string html_content;
    {
        while (html_content.empty()) {
            cout << "please enter file path > ";
            string path;
            cin >> path;
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

    // parse HTML file
    DomTree tree(html_content);
    if (tree.check()) {
        cout << "HTML file is valid" << endl;
    } else {
        cout << "HTML file is invalid" << endl;
        return 0;
    }

    // css selector


    return 0;
}
