/**
 * file: driver-proj4.cpp
 * author: Luke Smith
 * course: CSI 3334
 * assignment: project 4
 * due date: October 23, 2022
 *
 * This file contains the driver for AVL Tree.
 */

#include <iostream>
#include "avl-tree-student-proj4.h"
using namespace std;

int main() {
    EncryptionTree<string> encryptTree;
    string input;
    bool end = false;

    while (getline(cin, input) && !end) {
        string command;
        string value;

        // If line is blank skip
        if (input.empty()) {
            continue;
        }

        // Find location of whitespace and save everything prior
        size_t pos = input.find(' ');
        command = input.at(0);

        // Erase command from input including whitespace
        input.erase(0, pos+1);

        // If the command is not 1 char, skip iteration
        if (command.size() != 1) {
            continue;
        }

        // If command is "i", insert
        if (command == "i") {
            encryptTree.insert(input);
        }

        // If command is "r", remove
        else if (command == "r") {
            encryptTree.remove(input);
        }

        // If command is "e", encrypt
        else if (command == "e") {
            string word;
            bool lastWord = false;
            // Erase first ' character
            input.erase(0, 1);

            if (input.at(0) == '\'') {
                cout << endl;
            }
            else {
                do {
                    pos = input.find(' ');
                    word = input.substr(0, pos);

                    if (pos == 0) {
                        cout << endl;
                        break;
                    }

                    if (pos == string::npos) {
                        pos = input.find('\'');
                        word = input.substr(0, pos);
                        lastWord = true;
                    }
                    if (lastWord) {
                        cout << encryptTree.encrypt(word) << endl;
                    } else {
                        cout << encryptTree.encrypt(word) << " ";
                    }

                    input.erase(0, pos+1);
                } while (!lastWord);
            }

        }

        // If command is "d", decrypt
        else if (command == "d") {
            string word;
            bool lastWord = false;
            // Erase first ' character
            input.erase(0, 1);
            if (input.at(0) == '\'') {
                cout << endl;
            }

            else {
                do {
                    pos = input.find(' ');
                    word = input.substr(0, pos);


                    if (pos == 0) {
                        cout << endl;
                        break;
                    }

                    if (pos == string::npos) {
                        pos = input.find('\'');
                        word = input.substr(0, pos);
                        lastWord = true;
                    }
                    if (encryptTree.decrypt(word) == NULL) {
                        if (lastWord) {
                            cout << "?" << endl;
                        } else {
                            cout << "? ";
                        }
                    } else {
                        if (lastWord) {
                            cout << *encryptTree.decrypt(word) << endl;
                        } else {
                            cout << *encryptTree.decrypt(word) << " ";
                        }
                    }
                    input.erase(0, pos+1);
                } while (!lastWord);
            }
        }

        // If command is "p", print preorder
        else if (command == "p") {
            encryptTree.printPreorder();
        }

        // If command is "l", print level order
        else if (command == "l") {
            encryptTree.printLevelOrder();
        }

        // If command is "q", quit
        else if (command == "q") {
            end = true;
        }
        else {
            continue;
        }
    }
    return 0;
}