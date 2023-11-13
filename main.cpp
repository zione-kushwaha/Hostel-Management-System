#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;

// creation of the enum
enum MenuOption
{
    ADMIT_STUDENT = 1,
    SEARCH_RECORDS,
    DISPLAY_RECORDS,
    DELETE_RECORDS,
    MODIFY_RECORDS,
    EXIT
};
class Person
{
protected:
    string name;
    string address;
    string phone;

public:
    Person(const string &n, const string &a, const string &p)
        : name(n), address(a), phone(p) {}

    void displayDetails() const
    {
        cout << "\n Name: " << name;
        cout << "\n Address: " << address;
        cout << "\n Phone no: " << phone;
    }
    void updateDetails(const string &newName, const string &newAddress, const string &newPhone)
    {
        name = newName;
        address = newAddress;
        phone = newPhone;
    }
};

class Student : public Person
{
private:
    int room_num;
    int feepaid;
    int feedue;
    string messalloted;

public:
    Student(const string &n, const string &a, const string &p)
        : Person(n, a, p), room_num(-1), feepaid(0), feedue(0), messalloted("No") {}
    // Getter functions
    string getName() const { return name; }
    int getRoom_num() const { return room_num; }
    string getPhone() const { return phone; }
    int getFeePaid() const { return feepaid; }
    string getAddress() const { return address; }
    int getFeeDue() const { return feedue; }
    string getMessAllotmentStatus() const { return messalloted; }
    void admitStudent(int r);
    void displayStudentDetails() const;
    // Setter functions
    void setFeePaid(int paid) { feepaid = paid; }
    void setFeeDue(int due) { feedue = due; }
    void setMessAllotmentStatus(const string &status) { messalloted = status; }
};

void Student::displayStudentDetails() const
{
    cout << "\n Student Details";
    cout << "\n ****************";

    ifstream inFile("student_data.txt");
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;

            vector<string> studentData;
            while (getline(ss, token, ','))
            {
                studentData.push_back(token);
            }

            if (studentData.size() == 7)
            {
                int studentRoomNum = stoi(studentData[0]);
                if (studentRoomNum == room_num)
                {
                    cout << "\n Room no: " << studentRoomNum;
                    cout << "\n Name: " << studentData[1];
                    cout << "\n Address: " << studentData[2];
                    cout << "\n Phone: " << studentData[3];
                    cout << "\n Fee paid: " << studentData[4];
                    cout << "\n Fee due: " << studentData[5];
                    cout << "\n Mess allotment status: " << studentData[6];
                    break; // No need to continue searching after finding the student
                }
            }
        }
        inFile.close();
    }
    else
    {
        cout << "Error opening file for reading!" << endl;
    }
}

class Hostel
{
private:
    vector<Student> students;
    const string dataFilePath = "student_data.txt"; // File path for data storage

public:
    void menu();
    void admitStudent();
    void searchRecords();
    void displayRecords();
    void edit();
    void modify();
    int checkAvailability(int r);
};

// Implementation for the Hostel class functions
void Hostel::menu()
{
    int choice = 0;
    while (choice != EXIT)
    { 
        cout<<endl<<endl;
        cout << "1. Admit student " << endl;
        cout << "2. Search records" << endl;
        cout << "3. Display Records" << endl;
        cout << "4. Delete records" << endl;
        cout << "5. Modify records" << endl;
        cout << "6. Exit" <<endl;
        cout << "\n Enter your choice: ";
        cin >> choice;
        if (cin.fail())
        {
            cout << "Invalid input.number must be an integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; // Retry the loop to get a valid input
        }
        cin.ignore();

        switch (static_cast<MenuOption>(choice))
        {
        case ADMIT_STUDENT:
            admitStudent();
            break;
        case SEARCH_RECORDS:
            searchRecords();
            break;
        case DISPLAY_RECORDS:
            displayRecords();
            break;
        case DELETE_RECORDS:
            edit();
            break;
     case MODIFY_RECORDS:
           modify();
           break;

        case EXIT:
            return;
        default:
            cout << "\n\n\t\t\tWrong choice!!!";
            break;
        }
        cin.ignore();
    }
}

void Hostel::admitStudent()
{
    string n, a, p;

    cout << "\n Enter student's name: ";
    getline(cin, n);
    cout << " Enter address: ";
    getline(cin, a);
    cout << " Enter phone number: ";
    getline(cin, p);

    int roomNum;
    do
    {
        cout << "\n Enter room number: ";
        cin >> roomNum;

        if (cin.fail())
        {
            cout << "Invalid input. Room number must be an integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; // Retry the loop to get a valid input
        }

        cin.ignore(); // Clear the newline character left in the input buffer

        if (checkAvailability(roomNum) == -1)
        {
            cout << "\n Room number " << roomNum << " is already taken. Choose another room number." << endl;
        }
        else
        {
            break;
        }
    } while (true);

    Student newStudent(n, a, p);
    newStudent.admitStudent(roomNum);

    students.push_back(newStudent);

    // You can save the data here instead of in the Student class
    ofstream outFile(dataFilePath, ios::app);
    if (outFile.is_open())
    {
        outFile << newStudent.getRoom_num() << ","
                << newStudent.getName() << ","
                << newStudent.getAddress() << ","
                << newStudent.getPhone() << ","
                << newStudent.getFeePaid() << ","
                << newStudent.getFeeDue() << ","
                << newStudent.getMessAllotmentStatus() << "\n";
        outFile.close();
    }
    else
    {
        cout << "Error opening file for writing!" << endl;
    }

    cout << "\n Student admitted successfully!";
    cout << "\n Press Enter to continue!!";
    cin.ignore();
}

void Hostel::searchRecords()
{
    int roomNum;
    cout << "\n Enter room number to search: ";
    cin >> roomNum;

    ifstream inFile(dataFilePath);
    if (inFile.is_open())
    {
        string line;
        bool found = false;

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;

            vector<string> studentData;
            while (getline(ss, token, ','))
            {
                studentData.push_back(token);
            }

            if (studentData.size() == 7)
            {
                int studentRoomNum = stoi(studentData[0]);
                if (studentRoomNum == roomNum)
                {
                    cout << "\n Student details found:" << endl;
                    cout << " \nRoom no: " << studentRoomNum << endl;
                    cout << " Name: " << studentData[1] << endl;
                    cout << " Address: " << studentData[2] << endl;
                    cout << " Phone: " << studentData[3] << endl;
                    cout << " Fee paid: " << studentData[4] << endl;
                    cout << " Fee due: " << studentData[5] << endl;
                    cout << " Mess allotment status: " << studentData[6] << endl;
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            cout << "\n Sorry, student with room number " << roomNum << " not found." << endl;
        }

        inFile.close();
    }
    else
    {
        cout << "Error opening file for reading!" << endl;
    }

    cout << "\n Press  Enter to continue!!";
    cin.ignore();
}

int Hostel::checkAvailability(int r)
{
    ifstream inFile(dataFilePath);
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;

            vector<string> studentData;
            while (getline(ss, token, ','))
            {
                studentData.push_back(token);
            }

            if (studentData.size() >= 1)
            { // At least room number should be present
                int studentRoomNum = stoi(studentData[0]);
                if (studentRoomNum == r)
                {
                    inFile.close();
                    return -1; // Room number is already taken
                }
            }
        }
        inFile.close();
    }
    else
    {
        cout << "Error opening file for reading!" << endl;
    }

    return 0; // Room number is available
}
void Hostel::displayRecords() {
    ifstream inFile(dataFilePath);
    if (inFile.is_open()) {
        cout << "\n List of Students";
        cout << "\n ****************";

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string token;

            vector<string> studentData;
            while (getline(ss, token, ',')) {
                studentData.push_back(token);
            }

            if (studentData.size() >= 7) {
                int roomNum = stoi(studentData[0]);
                string name = studentData[1];
                string address = studentData[2];
                string phone = studentData[3];
                int feePaid = stoi(studentData[4]);
                int feeDue = stoi(studentData[5]);
                string messAllotmentStatus = studentData[6];

                cout << "\n\n Room no: " << roomNum;
                cout << "\n Name: " << name;
                cout << "\n Address: " << address;
                cout << "\n Phone no: " << phone;
                cout << "\n Fee paid: " << feePaid;
                cout << "\n Fee due: " << feeDue;
                cout << "\n Mess allotment status: " << messAllotmentStatus;
            }
        }

        inFile.close();
    }
    else {
        cout << "Error opening file for reading!" << endl;
    }

    cout << "\n Press Enter  to continue!!";
    
}

void Hostel::edit() {
    int roomNumToDelete;
    cout << "\n Enter room number to delete: ";
    cin >> roomNumToDelete;

    ifstream inputFile(dataFilePath);
    if (!inputFile) {
        cout << "Error opening input file." << endl;
        return;
    }

    vector<string> dataLines;
    string line;

    // Read data from the file into the vector
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;
        vector<string> studentData;

        while (getline(ss, token, ',')) {
            studentData.push_back(token);
        }

        if (!studentData.empty()) {
            int studentRoomNum = stoi(studentData[0]);
            if (studentRoomNum != roomNumToDelete) {
                dataLines.push_back(line);
            }
        }
    }

    inputFile.close();

    // Write the updated data back to the file
    ofstream outputFile(dataFilePath);
    if (!outputFile) {
        cout << "Error opening output file." << endl;
        return;
    }

    for (const string &data : dataLines) {
        outputFile << data << endl;
    }

    outputFile.close();

    cout << "Record deletion complete." << endl;
    cout<<endl<<"Press enter to continue"<<endl;
    cin.ignore();
}

void Student::admitStudent(int r)
{
    room_num = r;

    do
    {
        cout << "\n Enter fee paid: ";
        cin >> feepaid;
        if (cin.fail())
        {
            cout << "Invalid input. fee must be an integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; // Retry the loop to get a valid input
        }
        else
        {
            break;
        }
    } while (true);

    do
    {
        cout << " Enter fee due: ";
        cin >> feedue;
        if (cin.fail())
        {
            cout << "Invalid input. fee due must be an integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; // Retry the loop to get a valid input
        }
        else
        {
            break;
        }
    } while (true);

    cout << " Mess allotment status (y/n): ";
    cin >> messalloted;
}

// modification
void Hostel::modify() {
    int roomNumToEdit;
    cout << "\n Enter room number to edit: ";
    cin >> roomNumToEdit;

    ifstream inputFile(dataFilePath);
    if (!inputFile) {
        cout << "Error opening input file." << endl;
        return;
    }

    vector<string> dataLines;
    string line;

    // Read data from the file into the vector
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;
        vector<string> studentData;

        while (getline(ss, token, ',')) {
            studentData.push_back(token);
        }

        if (!studentData.empty()) {
            int studentRoomNum = stoi(studentData[0]);
            if (studentRoomNum != roomNumToEdit) {
                dataLines.push_back(line);
            } else {
                // Edit this line
                cout << "\n Editing student record for room number " << studentRoomNum << endl;

                cout << "Enter updated name: ";
                string updatedName;
                cin.ignore(); // Clear the newline character left in the input buffer
                getline(cin, updatedName);

                cout << "Enter updated address: ";
                string updatedAddress;
                getline(cin, updatedAddress);

                cout << "Enter updated phone number: ";
                string updatedPhone;
                getline(cin, updatedPhone);

                cout << "Enter updated fee paid: ";
                int updatedFeePaid;
                cin >> updatedFeePaid;

                cout << "Enter updated fee due: ";
                int updatedFeeDue;
                cin >> updatedFeeDue;

                cout << "Enter updated mess allotment status (y/n): ";
                string updatedMessStatus;
                cin >> updatedMessStatus;

                // Update the relevant fields
                studentData[1] = updatedName;
                studentData[2] = updatedAddress;
                studentData[3] = updatedPhone;
                studentData[4] = to_string(updatedFeePaid);
                studentData[5] = to_string(updatedFeeDue);
                studentData[6] = updatedMessStatus;

                // Join the updated data back into a line
                string updatedLine = "";
                for (const string &data : studentData) {
                    updatedLine += data + ",";
                }
                updatedLine.pop_back(); // Remove the trailing comma
                dataLines.push_back(updatedLine);
            }
        }
    }

    inputFile.close();

    // Write the updated data back to the file
    ofstream outputFile(dataFilePath);
    if (!outputFile) {
        cout << "Error opening output file." << endl;
        return;
    }

    for (const string &data : dataLines) {
        outputFile << data << endl;
    }

    outputFile.close();

    cout << "Record modification complete." << endl;
}


void animateWelcomeScreen() {
    cout << "\n\n\n\n\n\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    cout << "\t\t\t\t\t@@ ___________________________________________________________________________ @@\n";
    cout << "\t\t\t\t\t@@|                                  WELCOME TO                               |@@\n";
    cout << "\t\t\t\t\t@@|                                                                           |@@\n";
    cout << "\t\t\t\t\t@@|                          HOSTEL MANAGEMENT SYSTEM                         |@@\n";
    cout << "\t\t\t\t\t@@|                                                                           |@@\n";
    cout << "\t\t\t\t\t@@|               Developed by  TEAM 'SHOCKERS'                               |@@\n";
    cout << "\t\t\t\t\t@@|                                                                           |@@\n";
    cout << "\t\t\t\t\t@@|                                                                           |@@\n";
    cout << "\t\t\t\t\t@@|             TEAM MEMBERS NAME:                                            |@@\n";
    cout << "\t\t\t\t\t@@|                                                                           |@@\n";
    cout << "\t\t\t\t\t@@|             1. Bimlendra Pandit                                           |@@\n";
    cout << "\t\t\t\t\t@@|             2. Debendra shekhar Gupta                                     |@@\n";
    cout << "\t\t\t\t\t@@|             3. Dipesh Acharya                                             |@@\n";
    cout << "\t\t\t\t\t@@|             4. Jeevan Kumar Kushwaha                                      |@@\n";
    cout << "\t\t\t\t\t@@|                                                                           |@@\n";
    cout << "\t\t\t\t\t@@|___________________________________________________________________________|@@\n";
    cout << "\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n\n\t\t\t\t\t";
    cout << "Press any key to continue...";
}

int main()
{
    Hostel h;

   animateWelcomeScreen();
   cin.ignore();

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    try
    {
        h.menu();
    }
    catch (const exception &e)
    {
        cerr << "An error occurred: " << e.what() << endl;
    }
    return 0;
}
