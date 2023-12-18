//============================================================================
// Name        : CS300ProjectTwo.cpp
// Author      : Jamie Javis
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Final for 7-1
//============================================================================
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

// BinarySearchTree class
class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* right;
        Node* left;

        Node() : left(nullptr), right(nullptr) {}

        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int size; //node size

    void inOrder(Node* node);

public:
    BinarySearchTree() : root(nullptr), size(0) {}
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root;

    if (root == nullptr) {
        root = new Node(aCourse);
    }
    else {
        while (currentNode != nullptr) { 
           
            if (aCourse.courseId < currentNode->course.courseId) {
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    currentNode = nullptr;
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    currentNode = nullptr;
                }
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}

// Function to remove course
void BinarySearchTree::Remove(string courseId) {
    
}

// Function to search for course
Course BinarySearchTree::Search(string courseId) {
    Course aCourse;

    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }

    // not found
    return aCourse;
}

// Function to get size of BT
int BinarySearchTree::Size() {
    return size;
}

// Function to go through the BT
void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

// Split string function
vector<string> Split(string lineFeed) {
    char delim = ',';
    lineFeed += delim;
    vector<string> lineTokens;
    string temp = "";
    for (int i = 0; i < lineFeed.length(); i++) {
        if (lineFeed[i] == delim) {
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

// Function to load courses from file
void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS;
    string line;
    vector<string> stringTokens;

    inFS.open(csvPath);

    if (!inFS.is_open()) {
        cout << "Could not open file. Please check inputs." << endl;
        return;
    }

    while (!inFS.eof()) {
        Course aCourse;
        getline(inFS, line);
        stringTokens = Split(line);

        if (stringTokens.size() < 2) {
            cout << "Error. Skipping line." << endl;
        }
        else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {
                aCourse.preList.push_back(stringTokens.at(i));
            }

            courseList->Insert(aCourse);
        }
    }

    inFS.close();
}

// Function to display course info
void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.preList.empty()) {
        cout << "NONE" << endl;
    }
    else {
        for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
            cout << aCourse.preList.at(i);
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

// Converts string from upper case
void convertCase(string& toConvert) {
    for (unsigned int i = 0; i < toConvert.length(); i++) {
        if (isalpha(toConvert[i])) {
            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main() { // Main Functiom
    string csvPath = "Courses.txt";  // to read my txt file
    string aCourseKey;

    BinarySearchTree* courseList = new BinarySearchTree();
    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) { // Menu Loop
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        aCourseKey = "";
        string anyKey = " ";
        choice = 0;

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            }
            else {
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 2:
                courseList->InOrder();
                cout << "Enter 'y' to continue..." << endl;
                cin >> anyKey;
                break;

            case 3:
                cout << "What course do you want to know about? " << endl;
                cin >> aCourseKey;
                convertCase(aCourseKey);
                course = courseList->Search(aCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "Course ID " << aCourseKey << " not found." << endl;
                }
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 4:
                cout << "What course would you like to delete? " << endl;
                cin >> aCourseKey;
                convertCase(aCourseKey);
                courseList->Remove(aCourseKey);
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 9:
                exit(0);
                break;

            default:
                throw 2;
            }
        }
        catch (int err) {
            std::cout << "Not a Valid Option" << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        cin.clear();
        cin.ignore();
        system("cls");
    }

    cout << "Goodbye." << endl;
    Sleep(GLOBAL_SLEEP_TIME);

    return 0; // END
}
