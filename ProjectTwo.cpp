#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// ===== Course Structure =====
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// ===== Binary Search Tree Node =====
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// ===== Binary Search Tree Class =====
class BinarySearchTree {
private:
    Node* root;

    // Helper for inserting a course
    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    // Helper for inorder traversal (sorted output)
    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    // Helper for searching for a course
    Node* search(Node* node, const string& courseNumber) const {
        if (node == nullptr) return nullptr;
        if (node->course.courseNumber == courseNumber) return node;
        if (courseNumber < node->course.courseNumber)
            return search(node->left, courseNumber);
        else
            return search(node->right, courseNumber);
    }

    // Helper to delete nodes (cleanup)
    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroy(root); }

    void Insert(Course course) {
        insert(root, course);
    }

    void InOrder() const {
        inOrder(root);
    }

    Course* Search(const string& courseNumber) const {
        Node* node = search(root, courseNumber);
        if (node != nullptr)
            return &node->course;
        return nullptr;
    }

    bool IsEmpty() const {
        return root == nullptr;
    }
};

// ===== Helper Functions =====

// Parse one line from file into a Course
Course ParseCourseLine(const string& line) {
    Course course;
    stringstream ss(line);
    string word;
    vector<string> tokens;

    while (getline(ss, word, ',')) {
        // Trim spaces
        word.erase(0, word.find_first_not_of(" \t"));
        word.erase(word.find_last_not_of(" \t") + 1);
        tokens.push_back(word);
    }

    if (tokens.size() >= 2) {
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }
    }

    return course;
}

// Load data into BST
void LoadDataStructure(BinarySearchTree& bst) {
    string filename;
    cout << "Enter the file name to load (e.g., course.txt): ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file.\n";
        return;
    }

    bst = BinarySearchTree(); // reset BST

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            Course c = ParseCourseLine(line);
            if (!c.courseNumber.empty()) {
                bst.Insert(c);
            }
        }
    }

    file.close();
    cout << "Data successfully loaded into BST.\n";
}

// Print sorted course list
void PrintCourseList(const BinarySearchTree& bst) {
    if (bst.IsEmpty()) {
        cout << "Error: No data loaded. Please load file first.\n";
        return;
    }
    cout << "\n=== Course List (Alphanumeric Order) ===\n";
    bst.InOrder();
}

// Print info for one course
void PrintCourseInfo(const BinarySearchTree& bst) {
    if (bst.IsEmpty()) {
        cout << "Error: No data loaded. Please load file first.\n";
        return;
    }

    string courseNumber;
    cout << "Enter the course number (e.g., CS300): ";
    cin >> courseNumber;

    Course* course = bst.Search(courseNumber);
    if (course == nullptr) {
        cout << "Course not found.\n";
        return;
    }

    cout << "\nCourse Number: " << course->courseNumber << endl;
    cout << "Course Title : " << course->courseTitle << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None\n";
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// Display menu
void DisplayMenu() {
    cout << "\n=== ABCU Advising Assistance Program ===\n";
    cout << "1. Load Data Structure\n";
    cout << "2. Print Course List\n";
    cout << "3. Print Course Information\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

// ===== Main Function =====
int main() {
    BinarySearchTree bst;
    int choice = 0;

    while (choice != 9) {
        DisplayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            LoadDataStructure(bst);
            break;
        case 2:
            PrintCourseList(bst);
            break;
        case 3:
            PrintCourseInfo(bst);
            break;
        case 9:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please enter 1, 2, 3, or 9.\n";
        }
    }

    return 0;
}
