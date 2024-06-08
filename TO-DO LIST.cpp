#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE m = GetStdHandle(STD_OUTPUT_HANDLE);
const int MAX_TASKS = 100;

// Function to get a hidden password
string getHiddenPassword() {
    string password;
    char ch;

    cout << "Enter password (minimum 6 characters): ";

    while ((ch = _getch()) != 13) {  // 13 is the ASCII code for Enter
        if (ch == '\b' && !password.empty()) {
            cout << "\b \b";  // Handle backspace to erase the last character
            password = password.substr(0, password.length() - 1);
        } else {
            cout << '*';  // Display a placeholder character (e.g., '*') instead of the actual character
            password.push_back(ch);
        }
    }
    cout << endl;
    return password;
}

// Task class definition
class Task {
public:
    string name;
    string dueDate;
    bool isComplete;

    Task(const string& name, const string& dueDate) : name(name), dueDate(dueDate), isComplete(false) {}
};

// User class definition
class User {
public:
    string username;
    string password;
    string securityQuestion;
    string securityAnswer;
    vector<Task> tasks; // Use vector to dynamically manage tasks

    User(const string& username, const string& password, const string& securityQuestion, const string& securityAnswer)
        : username(username), password(password), securityQuestion(securityQuestion), securityAnswer(securityAnswer) {}

    void addTask(const string& taskName, const string& dueDate) {
        Task newTask(taskName, dueDate);
        tasks.push_back(newTask);
    }

    void markTaskComplete(int taskIndex) {
        if (taskIndex >= 0 && taskIndex < tasks.size()) {
            tasks[taskIndex].isComplete = true;
        }
    }

    void deleteTask(int taskIndex) {
        if (taskIndex >= 0 && taskIndex < tasks.size()) {
            tasks.erase(tasks.begin() + taskIndex);
        }
    }

    bool verifySecurityAnswer(const string& enteredAnswer) const {
        return securityAnswer == enteredAnswer;
    }
};

// ToDoList class definition
class ToDoList {
public:
    vector<User> users;
    int loggedInUserIndex;

    ToDoList() : loggedInUserIndex(-1) {}

    void registerUser() {
        string username, password, securityQuestion, securityAnswer;
        SetConsoleTextAttribute(m, 4);
        cout << "Enter username: ";
        cin >> username;

        // Check if the username contains only alphabetic characters
        for (size_t i = 0; i < username.length(); i++) {
            if (!isalpha(username[i])) {
                SetConsoleTextAttribute(m, 4);
                cout << endl << "Invalid username. Please use only alphabetic characters." << endl << endl;
                return;
            }
        }

        // Check if the username already exists
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == username) {
                SetConsoleTextAttribute(m, 4);
                cout << endl << "Username already exists. Please choose a different username." << endl << endl;
                return;
            }
        }

        SetConsoleTextAttribute(m, 4);
        cout << "Enter security question: ";
        cin.ignore();
        getline(cin, securityQuestion);

        SetConsoleTextAttribute(m, 4);
        cout << "Enter security answer: ";
        getline(cin, securityAnswer);

        do {
            SetConsoleTextAttribute(m, 4);
            password = getHiddenPassword();  // Use the function to get a hidden password

            if (password.length() < 6) {
                SetConsoleTextAttribute(m, 4);
                cout << "Password must be at least 6 characters long. Please try again." << endl;
            }
        } while (password.length() < 6);

        // Register the user
        users.push_back(User(username, password, securityQuestion, securityAnswer));
        cout << endl << "User registered successfully!" << endl;
        cout << endl;
    }

    void loginUser() {
        string username, password;
        SetConsoleTextAttribute(m, 4);
        cout << "Enter username: ";
        cin >> username;
        password = getHiddenPassword(); // Use the function to get a hidden password

        // Check if the user exists and the password is correct
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == username && users[i].password == password) {
                loggedInUserIndex = static_cast<int>(i);
                cout << endl << "Logged in successfully as " << users[i].username << "." << endl;
                cout << endl;
                return;
            }
        }

        cout << "Invalid username or password. Please try again." << endl;
        cout << endl;
    }

    void forgotPassword() {
        if (users.empty()) {
            cout << "No users registered. Cannot recover password." << endl;
            cout << endl;
            return;
        }

        string username;
        SetConsoleTextAttribute(m, 4);
        cout << "Enter your username: ";
        cin >> username;

        int userIndex = findUserByUsername(username);

        if (userIndex == -1) {
            cout << "User not found. Cannot recover password." << endl;
            cout << endl;
            return;
        }

        string enteredSecurityAnswer;
        SetConsoleTextAttribute(m, 4);
        cout << "Security Question: " << users[userIndex].securityQuestion << endl;
        cout << "Enter your answer: ";
        cin.ignore();
        getline(cin, enteredSecurityAnswer);

        if (users[userIndex].verifySecurityAnswer(enteredSecurityAnswer)) {
            SetConsoleTextAttribute(m, 4);
            string newPassword;
            do {
                cout << "Option For New Password\n";
                newPassword = getHiddenPassword();

                if (newPassword.length() < 6) {
                    SetConsoleTextAttribute(m, 4);
                    cout << "Password must be at least 6 characters long. Please try again." << endl;
                }
            } while (newPassword.length() < 6);

            users[userIndex].password = newPassword;
            SetConsoleTextAttribute(m, 4);
            cout << "Password changed successfully!" << endl;
        } else {
            cout << "Incorrect security answer. Cannot recover password." << endl;
        }

        cout << endl;
    }

    int findUserByUsername(const string& username) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == username) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    void logoutUser() {
        loggedInUserIndex = -1;
        cout << endl << "Logged out successfully." << endl;
        cout << endl;
    }

    // ... (rest of your code remains unchanged)
    
    // Function to add a task
void addTask() {
    if (loggedInUserIndex == -1) {
    SetConsoleTextAttribute(m,4);
        cout << endl << "Please log in to add a task." << endl;
        return;
    }

    if (users[loggedInUserIndex].tasks.size() == MAX_TASKS) {
    SetConsoleTextAttribute(m,4);
        cout << "Task limit reached. Unable to add a new task." << endl;
        return;
    }

    string name, dueDate;
     SetConsoleTextAttribute(m,14);
    cout << "Enter task name: ";
    cin.ignore();
    getline(cin, name);
     SetConsoleTextAttribute(m,14);
    cout << "Enter due date or Time: ";
    getline(cin,dueDate);

    Task newTask(name, dueDate);
    users[loggedInUserIndex].tasks.push_back(newTask);
  SetConsoleTextAttribute(m,14);
    cout << endl << "Task added successfully!" << endl;
    cout << endl;
}

// Function to mark a task as complete
void markTaskComplete() {
    if (loggedInUserIndex == -1) {
        cout << endl << "Please log in to mark a task as complete." << endl;
        cout << endl;
        return;
    }

    int taskIndex;
     SetConsoleTextAttribute(m,11);
    cout << "Enter the task number: ";
    cin >> taskIndex;
    taskIndex--;

    if (taskIndex < 0 || taskIndex >= users[loggedInUserIndex].tasks.size()) {
    SetConsoleTextAttribute(m,4);
        cout << endl << "Invalid task number. Please try again." << endl;
        return;
    }
if(users[loggedInUserIndex].tasks[taskIndex].isComplete){
SetConsoleTextAttribute(m,4);
cout << endl << "Task is already marked as complete!" << endl;
return;
}
    users[loggedInUserIndex].tasks[taskIndex].isComplete = true;
     SetConsoleTextAttribute(m,11);
    cout << endl << "Task marked as complete!" << endl;
    cout << endl;
}

// Function to delete a task
void deleteTask() {
    if (loggedInUserIndex == -1) {
        cout << endl << "Please log in to delete a task." << endl;
        return;
    }

    int taskIndex;    
SetConsoleTextAttribute(m,14);
    cout << "Enter the task number: ";
    cin >> taskIndex;
    taskIndex--;

    if (taskIndex < 0 || taskIndex >= users[loggedInUserIndex].tasks.size()) {
        cout << "Invalid task number. Please try again." << endl;
        return;
    }

    users[loggedInUserIndex].tasks.erase(users[loggedInUserIndex].tasks.begin() + taskIndex);
    cout << endl << "Task deleted successfully!" << endl;
    cout << endl;
}

// Function to move a task
void moveTask() {
    if (loggedInUserIndex == -1) {
    SetConsoleTextAttribute(m,4);
        cout << endl << "Please log in to move a task." << endl;
        return;
    }

    int taskIndex;
     SetConsoleTextAttribute(m,5);
    cout << "Enter the task number: ";
    cin >> taskIndex;
    taskIndex--;

    if (taskIndex < 0 || taskIndex >= users[loggedInUserIndex].tasks.size()) {
    SetConsoleTextAttribute(m,5);
        cout << "Invalid task number. Please try again." << endl;
        return;
    }

    int moveDirection;
         SetConsoleTextAttribute(m,5);
    cout << "Move this task up OR down? (1 for up, 2 for down): ";
    cin >> moveDirection;

    if (moveDirection == 1 && taskIndex > 0) {
        Task temp = users[loggedInUserIndex].tasks[taskIndex];
        users[loggedInUserIndex].tasks[taskIndex] = users[loggedInUserIndex].tasks[taskIndex - 1];
        users[loggedInUserIndex].tasks[taskIndex - 1] = temp;
        SetConsoleTextAttribute(m,5);
        cout << "Task moved up successfully!" << endl;
    } else if (moveDirection == 2 && taskIndex < users[loggedInUserIndex].tasks.size() - 1) {
        Task temp = users[loggedInUserIndex].tasks[taskIndex];
        users[loggedInUserIndex].tasks[taskIndex] = users[loggedInUserIndex].tasks[taskIndex + 1];
        users[loggedInUserIndex].tasks[taskIndex + 1] = temp;
        SetConsoleTextAttribute(m,5);
        cout << "Task moved down successfully!" << endl;
    } else {
        cout << "Invalid Move Direction or Position. Unable to Move task" << endl;
    }
}

// Function to arrange tasks with completed first
void arrangeCompletedFirst() {
    if (loggedInUserIndex == -1) {
        cout << "Please log in to arrange tasks." << endl;
        return;
    }

    if (users[loggedInUserIndex].tasks.empty()) {
        cout << "No tasks found." << endl;
        return;
    }

    bool hasCompletedTasks = false;
    for (size_t i = 0; i < users[loggedInUserIndex].tasks.size(); i++) {
        if (users[loggedInUserIndex].tasks[i].isComplete) {
            hasCompletedTasks = true;
            break;
        }
    }

    if (!hasCompletedTasks) {
        cout << "No completed tasks found." << endl;
        return;
    }

    // Rearrange tasks with completed tasks first
    size_t completedIndex = 0;
    size_t incompleteIndex = users[loggedInUserIndex].tasks.size() - 1;

    while (completedIndex < incompleteIndex) {
        if (users[loggedInUserIndex].tasks[completedIndex].isComplete) {
            completedIndex++;
        } else {
            swap(users[loggedInUserIndex].tasks[completedIndex], users[loggedInUserIndex].tasks[incompleteIndex]);
            incompleteIndex--;
        }
    }

    cout << endl << "Tasks arranged with completed tasks first!" << endl;
    cout << endl;
}

// Function to delete all completed tasks
void deleteAllCompletedTasks() {
    if (loggedInUserIndex == -1) {
        cout << "Please log in to delete completed tasks." << endl;
        return;
    }

    if (users[loggedInUserIndex].tasks.empty()) {
        cout << "No tasks found." << endl;
        return;
    }

    bool anyCompletedTasks = false;

    // Check if any tasks are completed
    for (size_t i = 0; i < users[loggedInUserIndex].tasks.size(); i++) {
        if (users[loggedInUserIndex].tasks[i].isComplete) {
            anyCompletedTasks = true;
            break;
        }
    }

    if (!anyCompletedTasks) {
        cout << "No completed tasks found." << endl;
        return;
    }

    // Delete completed tasks
    size_t i = 0;

    while (i < users[loggedInUserIndex].tasks.size()) {
        if (users[loggedInUserIndex].tasks[i].isComplete) {
            users[loggedInUserIndex].tasks.erase(users[loggedInUserIndex].tasks.begin() + i);
        } else {
            i++;
        }
    }

    cout << users[loggedInUserIndex].tasks.size() << "task remaining, and completed tasks deleted from the list." << endl;
}

// ...

   

  void displayTasks() {
    if (loggedInUserIndex == -1) {
        cout << endl << "Please log in to view your tasks." << endl;
        return;
    }
SetConsoleTextAttribute(m,14);
    cout << endl << "==== Your Tasks ====" << endl;
    cout << endl;

    if (users[loggedInUserIndex].tasks.empty()) {
        cout << "No tasks found." << endl;
        cout << endl;
        return;
    }
    for (size_t i = 0; i < users[loggedInUserIndex].tasks.size(); i++) {
        cout << i + 1 << ". " << users[loggedInUserIndex].tasks[i].name << " (Due: " << users[loggedInUserIndex].tasks[i].dueDate << ")";
       
        if (users[loggedInUserIndex].tasks[i].isComplete)
            cout << " [Complete]";
        cout << endl;
    }
    cout << endl;
}

    void saveDataToFile() {
        ofstream file("data.txt");
        if (file.is_open()) {
            // Save user information
            file << users.size() << endl;
            for (size_t i = 0; i < users.size(); i++) {
                file << users[i].username << "," << users[i].password << "," << users[i].securityQuestion << ","
                     << users[i].securityAnswer << endl;
            }

            // Save task data
            for (size_t i = 0; i < users.size(); i++) {
                file << users[i].tasks.size() << endl;
                for (size_t j = 0; j < users[i].tasks.size(); j++) {
                    file << users[i].tasks[j].name << "," << users[i].tasks[j].dueDate << ","
                         << users[i].tasks[j].isComplete << endl;
                }
            }

            file.close();
            SetConsoleTextAttribute(m, 3);
            cout << "Data saved to file successfully!" << endl;
        } else {
            SetConsoleTextAttribute(m, 3);
            cout << "Unable to open the file to save data." << endl;
        }
    }

    void loadDataFromFile() {
        ifstream file("data.txt");
        if (file.is_open()) {
            string line;

            // Load user information
            getline(file, line);
            int loadedUserCount;
            stringstream(line) >> loadedUserCount;

            for (int i = 0; i < loadedUserCount; i++) {
                getline(file, line);
                stringstream ss(line);
                string username, password, securityQuestion, securityAnswer;
                getline(ss, username, ',');
                getline(ss, password, ',');
                getline(ss, securityQuestion, ',');
                getline(ss, securityAnswer, ',');
                users.push_back(User(username, password, securityQuestion, securityAnswer));
            }

            // Load task data
            for (size_t i = 0; i < users.size(); i++) {
                getline(file, line);
                int loadedTaskCount;
                stringstream(line) >> loadedTaskCount;
                for (int j = 0; j < loadedTaskCount; j++) {
                    getline(file, line);
                    stringstream ss(line);
                    string name, dueDate;
                    bool isComplete;
                    getline(ss, name, ',');
                    getline(ss, dueDate, ',');
                    ss >> isComplete;
                    users[i].tasks.push_back(Task(name, dueDate));
                    users[i].tasks.back().isComplete = isComplete;
                }
            }

            file.close();
            SetConsoleTextAttribute(m, 3);
            cout << "Data loaded from file successfully!" << endl;
        } else {
            SetConsoleTextAttribute(m, 3);
            cout << "Unable to open the file to load data." << endl;
        }
    }

    // ... (rest of your existing code)
};

// Main program
int main() {
    ToDoList toDoList;

    // Load user information and task data from a file (if any)
    toDoList.loadDataFromFile();

    // Main menu
    int choice;
    do {

        cout << endl;

        SetConsoleTextAttribute(m, 3);
        cout << "==== To-Do List ====" << endl;
        SetConsoleTextAttribute(m, 10);
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Forgot Password" << endl;
        cout << "4. Logout" << endl;
        cout << "5. Add Task" << endl;
        cout << "6. Mark Task Complete" << endl;
        cout << "7. Delete Task" << endl;
        cout << "8. Display Tasks" << endl;
        cout << "9. Move A Task" << endl;
        cout << "10. Arrange tasks with completed first" << endl;
        cout << "11. Delete all the completed tasks from your list" << endl;
        cout << "0. Save & Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                toDoList.registerUser();
                break;
            case 2:
                toDoList.loginUser();
                break;
            case 3:
                toDoList.forgotPassword();
                break;
            case 4:
                toDoList.logoutUser();
                break;
            case 5:
                toDoList.addTask();
                break;
            case 6:
                toDoList.markTaskComplete();
                break;
            case 7:
                toDoList.deleteTask();
                break;
            case 8:
                toDoList.displayTasks();
                break;
            case 9:
                toDoList.moveTask();
                break;
            case 10:
                toDoList.arrangeCompletedFirst();
                break;
            case 11:
                toDoList.deleteAllCompletedTasks();
                break;
        }
    } while (choice != 0);

    // Save user information and task data to a file (if any)
    toDoList.saveDataToFile();

    return 0;
}
