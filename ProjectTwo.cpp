//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Thi Hong Van Nguyen
// Version     : 1.0
// Description : ABCU course management application
//============================================================================

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm> 

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================
// define a structure to hold bid information
struct Course {
    string courseNumber; // unique identifier
    string title;
    vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void deleteNodes(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Print the given course with course number, title & Prerequisites
 *
 * @param course
 */
void printCourse(Course course) {
    cout << course.courseNumber << ", " << course.title << endl; // Print course number and title

    // Print Prerequisites if any
    if (course.prerequisites.size() > 0) {
        cout << "Prerequisites: ";

        // loop through prerequisites and print each item
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];

            // Check if the current element is the last one
            if (i != course.prerequisites.size() - 1) {
                std::cout << ", ";
            }
        }
    }
    cout << "\n" << endl;
}

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // FixMe (1): initialize housekeeping variables
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    deleteNodes(root);
}

void BinarySearchTree::deleteNodes(Node* node) {
    // recurse from root deleting every node
    if (node != nullptr) {
        // Delete left subtree
        deleteNodes(node->left);

        // Delete right subtree
        deleteNodes(node->right);

        // Delete the current node
        delete node;
    }
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        addNode(root, course);
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);

        //output course
        printCourse(node->course);

        //InOder right
        inOrder(node->right);
    }
}

/**
 * Print a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching courseNumber found
    while (current != nullptr) {
        // Convert both strings to uppercase before comparison
        std::string courseNumberUpperCase = courseNumber;
        std::string currentCourseNumberUpperCase = current->course.courseNumber;
        std::transform(courseNumberUpperCase.begin(), courseNumberUpperCase.end(), courseNumberUpperCase.begin(), ::toupper);
        std::transform(currentCourseNumberUpperCase.begin(), currentCourseNumberUpperCase.end(), currentCourseNumberUpperCase.begin(), ::toupper);

        // if match found, print current course
        if (currentCourseNumberUpperCase == courseNumberUpperCase) {            
            return current->course;
        }
        // if course is smaller than current node then traverse left
        if (currentCourseNumberUpperCase > courseNumberUpperCase) {
            current = current->left;

        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    
    cout << "Course Number not found!" << endl;
    Course course;
    return course;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

void showErrorMessage(string message) {
    cout << "*** ERROR ***" << endl;
    cout << message << endl;
}

/**
 * Validate the given csv file. 
 * Each row should have course number and title
 * Prerequisites should exists in the course list
 * 
 * @param csvPath the path to the CSV file to open
 * @return true if the file is valid, else false
 */
bool validateInputFile(string csvPath) {
    // Open file
    ifstream file(csvPath);

    // Check if the file is successfully opened
    if (!file.is_open()) {
        cout << "Error opening file: " << csvPath << endl;
        return false;
    }

    string line;
    vector<string> courses;
    vector<string> prerequisites;

    // Read each line from the CSV file
    while (getline(file, line)) {
        string field;
        istringstream lineStream(line);

        // Skip the line if it's empty or contains only whitespace
        if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace)) {
            continue;
        }

        // Throw error if there is no parameter or the course Number is missing
        if (!getline(lineStream, field, ',') || field == "") {  
            showErrorMessage("One of the course is missing course number.");
            file.close();
            return false;
        }
        // Add the course number to courses vector to validate prerequisites later.
        courses.push_back(field); 

        // Throw eror if the second parameter or the title is missing
        if (!getline(lineStream, field, ',') || field == "") {
            showErrorMessage("One of the course is missing course title.");
            file.close();
            return false;
        }

        // Loop through the rest of the parameters
        while (getline(lineStream, field, ',')) {
            // Store the prerequisites in a vector to check the validity of the prerequisite
            // However, only store the one that is not already in prerequisites vector.
            if (find(prerequisites.begin(), prerequisites.end(), field) == prerequisites.end() && field != "") {
                prerequisites.push_back(field);
            }
        }
    }

    // Check if all the presequisites are in courses vector
    for (vector<string>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++it) {
        if (find(courses.begin(), courses.end(), *it) == courses.end()) {
            showErrorMessage("Invalid prerequisite.");
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    // Open file
    ifstream file(csvPath);

    // Check if the file is successfully opened
    if (!file.is_open()) {
        cout << "Error opening file: " << csvPath << endl;
        return;
    }

    string line;
    vector<string> courses;
    vector<string> prerequisites;

    // Read each line from the CSV file
    while (getline(file, line)) {
        Course course;
        string field;
        istringstream lineStream(line);

        // Store course number
        getline(lineStream, field, ',');
        course.courseNumber = field;

        // Store title
        getline(lineStream, field, ',');
        course.title = field;

        // Store prerequisites
        vector<string> pre;
        // Loop through the rest of the parameters
        while (getline(lineStream, field, ',') && field != "") {
            pre.push_back(field);
        }
        course.prerequisites = pre;

        // Insert in bst
        bst->Insert(course);
    }

    file.close();
    cout << "Courses loaded successfully!" << endl;
}

int main()
{
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    cout << "Welcome to the course planner." << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "\nWhat would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string csvPath;
            cout << "Please enter the file name to load: ";
            cin >> csvPath; // Read the file name

            // Validate the file data
            bool isValid = validateInputFile(csvPath);
            if (isValid) {
                loadCourses(csvPath, bst);
            }
            break;
        }

        case 2:
            bst->InOrder();
            break;

        case 3:
        {
            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber; // Read the course number input
            course = bst->Search(courseNumber);

            if (!course.courseNumber.empty()) {
                printCourse(course);
            }
            break;
        }

        case 9:
            break;

        default: {
            cout << to_string(choice) + " is not a valid option." << endl;
            // Clear cin's error flags
            cin.clear();
            // Clear the input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        }
    }
}
