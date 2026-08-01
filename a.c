#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#define USER_FILE "users.txt"
#define STUDENT_FILE "students.txt"
#define MAX_NAME_SIZE 50
#define MAX_COURSE_SIZE 50
#define MAX_PASSWORD_SIZE 20
#define MAX_ADDRESS_SIZE 100
#define MAX_CONTACT_SIZE 15
#define MAX_DOB_SIZE 12
#define MAX_STUDENTS 1000

struct User {
    char username[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
};

struct Student {
    int studentID;
    char studentName[MAX_NAME_SIZE];
    char fatherName[MAX_NAME_SIZE];
    char motherName[MAX_NAME_SIZE];
    char address[MAX_ADDRESS_SIZE];
    char contactNumber[MAX_CONTACT_SIZE];
    char dateOfBirth[MAX_DOB_SIZE];
    char course[MAX_COURSE_SIZE];
    float attendancePercentage;
    float grade;
};

void clearScreen();
void pressEnterToContinue();
void printHeader(const char* title);
void clearInputBuffer();
int validateNumberInput();
int validateDate(const char* date);
int validatePhone(const char* phone);
int studentExists(int id);
int countStudents();

void setupDefaultUser();
int login();
void changePassword();

void addStudent();
void viewAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudents();
void displayStatistics();

void manageAttendance();
void manageGrades();
void viewPerformanceChart();
void backupData();
void restoreData();
void searchByName();

void mainMenu();
void studentManagementMenu();

int main() {
    FILE *userFile = fopen(USER_FILE, "r");
    if (userFile == NULL) {
        setupDefaultUser();
    } else {
        fclose(userFile);
    }

    while (1) {
        clearScreen();
        printHeader("STUDENT INFORMATION MANAGEMENT SYSTEM - LOGIN");
        
        printf("\n\n\t\t\t1. Login");
        printf("\n\t\t\t2. Exit");
        printf("\n\n\t\t\tEnter your choice: ");

        int choice = validateNumberInput();
        
        switch (choice) {
            case 1: 
                if (login()) {
                    mainMenu();
                } else {
                    printf("\n\t\t\tInvalid username or password.\n");
                    pressEnterToContinue();
                }
                break;
            case 2:
                printf("\n\t\t\tExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\n\t\t\tInvalid choice. Please try again.\n");
                pressEnterToContinue();
        }
    }
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pressEnterToContinue() {
    printf("\n\n\t\t\tPress Enter to continue...");
    while(getchar() != '\n');
}

void printHeader(const char* title) {
    printf("\n\t\t\t===============================================================");
    printf("\n\t\t\t            %s", title);
    printf("\n\t\t\t===============================================================\n");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validateNumberInput() {
    int num;
    while (scanf("%d", &num) != 1) {
        clearInputBuffer();
        printf("\t\t\tInvalid input. Please enter a number: ");
    }
    clearInputBuffer();
    return num;
}

int validateDate(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;
    return 1;
}

int validatePhone(const char* phone) {
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int studentExists(int id) {
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) return 0;
    
    struct Student s;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.studentID == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int countStudents() {
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) return 0;
    
    int count = 0;
    char buffer[500];
    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }
    fclose(file);
    return count;
}

void setupDefaultUser() {
    FILE *file = fopen(USER_FILE, "w");
    if (file == NULL) {
        perror("\n\t\t\tError creating user file");
        exit(1);
    }

    fprintf(file, "admin,password\n");
    fclose(file);
    
    printf("\n\t\t\tDefault user 'admin' created with password 'password'.\n");
    pressEnterToContinue();
}

int login() {
    char username[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    char fileUsername[MAX_NAME_SIZE];
    char filePassword[MAX_PASSWORD_SIZE];

    printf("\n\t\t\tEnter Username: ");
    fgets(username, MAX_NAME_SIZE, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("\t\t\tEnter Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && ch != '\n' && i < MAX_PASSWORD_SIZE - 1) {
        if(ch == '\b') {
            if(i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        perror("\n\t\t\tError opening user file");
        return 0;
    }

    while (fscanf(file, "%[^,],%[^\n]\n", fileUsername, filePassword) != EOF) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void changePassword() {
    clearScreen();
    printHeader("CHANGE PASSWORD");

    char oldPassword[MAX_PASSWORD_SIZE], newPassword[MAX_PASSWORD_SIZE], confirmPassword[MAX_PASSWORD_SIZE];
    char username[MAX_NAME_SIZE], password[MAX_PASSWORD_SIZE];
    FILE *file, *tempFile;

    file = fopen(USER_FILE, "r");
    if (file == NULL) {
        perror("\n\t\t\tError opening user file");
        pressEnterToContinue();
        return;
    }
    
    // Read current credentials
    fscanf(file, "%[^,],%[^\n]\n", username, password);
    fclose(file);

    printf("\n\t\t\tEnter Old Password: ");
    scanf("%s", oldPassword);

    if (strcmp(oldPassword, password) != 0) {
        printf("\n\t\t\tIncorrect old password!\n");
        pressEnterToContinue();
        return;
    }

    printf("\n\t\t\tEnter New Password: ");
    scanf("%s", newPassword);
    printf("\n\t\t\tConfirm New Password: ");
    scanf("%s", confirmPassword);

    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("\n\t\t\tNew passwords do not match!\n");
        pressEnterToContinue();
        return;
    }

    // Write new credentials
    file = fopen(USER_FILE, "w");
    if (file == NULL) {
        perror("\n\t\t\tError updating password");
        pressEnterToContinue();
        return;
    }
    fprintf(file, "%s,%s\n", username, newPassword);
    fclose(file);

    printf("\n\t\t\tPassword changed successfully!\n");
    pressEnterToContinue();
}

void mainMenu() {
    int choice;
    while (1) {
        clearScreen();
        printHeader("MAIN MENU");
        
        printf("\n\t\t\t1. Student Management");
        printf("\n\t\t\t2. Attendance & Grades");
        printf("\n\t\t\t3. Reports & Statistics");
        printf("\n\t\t\t4. System Utilities");
        printf("\n\t\t\t5. Change Password");
        printf("\n\t\t\t6. Logout");
        printf("\n\n\t\t\tEnter your choice: ");

        choice = validateNumberInput();

        switch (choice) {
            case 1: studentManagementMenu(); break;
            case 2: 
                clearScreen();
                printHeader("ATTENDANCE & GRADES");
                printf("\n\t\t\t1. Manage Attendance");
                printf("\n\t\t\t2. Manage Grades");
                printf("\n\t\t\t3. Back to Main Menu");
                printf("\n\n\t\t\tEnter your choice: ");
                
                choice = validateNumberInput();
                if (choice == 1) manageAttendance();
                else if (choice == 2) manageGrades();
                break;
            case 3:
                clearScreen();
                printHeader("REPORTS & STATISTICS");
                printf("\n\t\t\t1. View Performance Chart");
                printf("\n\t\t\t2. View Statistics");
                printf("\n\t\t\t3. Back to Main Menu");
                printf("\n\n\t\t\tEnter your choice: ");
                
                choice = validateNumberInput();
                if (choice == 1) viewPerformanceChart();
                else if (choice == 2) displayStatistics();
                break;
            case 4:
                clearScreen();
                printHeader("SYSTEM UTILITIES");
                printf("\n\t\t\t1. Backup Data");
                printf("\n\t\t\t2. Restore Data");
                printf("\n\t\t\t3. Back to Main Menu");
                printf("\n\n\t\t\tEnter your choice: ");
                
                choice = validateNumberInput();
                if (choice == 1) backupData();
                else if (choice == 2) restoreData();
                break;
            case 5: changePassword(); break;
            case 6: return;
            default:
                printf("\n\t\t\tInvalid choice. Please try again.\n");
                pressEnterToContinue();
        }
    }
}

void studentManagementMenu() {
    int choice;
    while (1) {
        clearScreen();
        printHeader("STUDENT MANAGEMENT");
        
        printf("\n\t\t\t1. Add New Student");
        printf("\n\t\t\t2. View All Students");
        printf("\n\t\t\t3. Search Student");
        printf("\n\t\t\t4. Update Student");
        printf("\n\t\t\t5. Delete Student");
        printf("\n\t\t\t6. Sort Students");
        printf("\n\t\t\t7. Back to Main Menu");
        printf("\n\n\t\t\tEnter your choice: ");

        choice = validateNumberInput();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAllStudents(); break;
            case 3: 
                clearScreen();
                printHeader("SEARCH STUDENT");
                printf("\n\t\t\t1. Search by ID");
                printf("\n\t\t\t2. Search by Name");
                printf("\n\t\t\t3. Back to Menu");
                printf("\n\n\t\t\tEnter your choice: ");
                
                choice = validateNumberInput();
                if (choice == 1) searchStudent();
                else if (choice == 2) searchByName();
                break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 7: return;
            default:
                printf("\n\t\t\tInvalid choice. Please try again.\n");
                pressEnterToContinue();
        }
    }
}

void addStudent() {
    clearScreen();
    printHeader("ADD NEW STUDENT");

    struct Student newStudent;
    FILE *file = fopen(STUDENT_FILE, "a");

    if (file == NULL) {
        perror("\n\t\t\tError opening student file");
        pressEnterToContinue();
        return;
    }

    printf("\n\t\t\tEnter Student ID: ");
    newStudent.studentID = validateNumberInput();

    if(studentExists(newStudent.studentID)) {
        printf("\n\t\t\tError: Student ID %d already exists.\n", newStudent.studentID);
        fclose(file);
        pressEnterToContinue();
        return;
    }

    printf("\t\t\tEnter Student Name: ");
    fgets(newStudent.studentName, MAX_NAME_SIZE, stdin);
    newStudent.studentName[strcspn(newStudent.studentName, "\n")] = 0;

    printf("\t\t\tEnter Father's Name: ");
    fgets(newStudent.fatherName, MAX_NAME_SIZE, stdin);
    newStudent.fatherName[strcspn(newStudent.fatherName, "\n")] = 0;

    printf("\t\t\tEnter Mother's Name: ");
    fgets(newStudent.motherName, MAX_NAME_SIZE, stdin);
    newStudent.motherName[strcspn(newStudent.motherName, "\n")] = 0;

    printf("\t\t\tEnter Address: ");
    fgets(newStudent.address, MAX_ADDRESS_SIZE, stdin);
    newStudent.address[strcspn(newStudent.address, "\n")] = 0;

    do {
        printf("\t\t\tEnter Contact Number: ");
        fgets(newStudent.contactNumber, MAX_CONTACT_SIZE, stdin);
        newStudent.contactNumber[strcspn(newStudent.contactNumber, "\n")] = 0;
        if (!validatePhone(newStudent.contactNumber)) {
            printf("\t\t\tInvalid phone number. Please enter digits only.\n");
        }
    } while (!validatePhone(newStudent.contactNumber));

    do {
        printf("\t\t\tEnter Date of Birth (DD-MM-YYYY): ");
        fgets(newStudent.dateOfBirth, MAX_DOB_SIZE, stdin);
        newStudent.dateOfBirth[strcspn(newStudent.dateOfBirth, "\n")] = 0;
        if (!validateDate(newStudent.dateOfBirth)) {
            printf("\t\t\tInvalid date format. Use DD-MM-YYYY.\n");
        }
    } while (!validateDate(newStudent.dateOfBirth));

    printf("\t\t\tEnter Course: ");
    fgets(newStudent.course, MAX_COURSE_SIZE, stdin);
    newStudent.course[strcspn(newStudent.course, "\n")] = 0;

    do {
        printf("\t\t\tEnter Attendance Percentage (0-100): ");
        scanf("%f", &newStudent.attendancePercentage);
        if (newStudent.attendancePercentage < 0 || newStudent.attendancePercentage > 100) {
            printf("\t\t\tInvalid percentage. Enter between 0-100.\n");
        }
    } while (newStudent.attendancePercentage < 0 || newStudent.attendancePercentage > 100);

    do {
        printf("\t\t\tEnter Overall Grade (0-100): ");
        scanf("%f", &newStudent.grade);
        if (newStudent.grade < 0 || newStudent.grade > 100) {
            printf("\t\t\tInvalid grade. Enter between 0-100.\n");
        }
    } while (newStudent.grade < 0 || newStudent.grade > 100);

    clearInputBuffer();

    fprintf(file, "%d,%s,%s,%s,%s,%s,%s,%s,%.2f,%.2f\n",
            newStudent.studentID, newStudent.studentName, newStudent.fatherName, newStudent.motherName,
            newStudent.address, newStudent.contactNumber, newStudent.dateOfBirth, newStudent.course,
            newStudent.attendancePercentage, newStudent.grade);
    fclose(file);

    printf("\n\t\t\tStudent record added successfully!\n");
    pressEnterToContinue();
}

void viewAllStudents() {
    clearScreen();
    printHeader("ALL STUDENT RECORDS");

    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("\n\t\t\tNo student records found.\n");
        pressEnterToContinue();
        return;
    }

    int count = 0;
    printf("\n\t%-8s %-25s %-20s %-15s %-10s\n", "ID", "Name", "Course", "Contact", "Grade");
    printf("\t----------------------------------------------------------------------------------\n");

    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        printf("\t%-8d %-25s %-20s %-15s %-10.2f\n",
               s.studentID, s.studentName, s.course, s.contactNumber, s.grade);
        count++;
    }

    if (count == 0) {
        printf("\n\t\t\tNo records to display.\n");
    } else {
        printf("\n\t\t\tTotal students: %d\n", count);
    }

    printf("\t----------------------------------------------------------------------------------\n");
    fclose(file);
    pressEnterToContinue();
}

void searchStudent() {
    clearScreen();
    printHeader("SEARCH STUDENT BY ID");

    int id;
    struct Student s;
    printf("\n\t\t\tEnter Student ID to search: ");
    id = validateNumberInput();

    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("\n\t\t\tNo student records found.\n");
        pressEnterToContinue();
        return;
    }

    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.studentID == id) {
            printf("\n\t\t\t--- Student Record Found ---\n");
            printf("\t\t\tStudent ID         : %d\n", s.studentID);
            printf("\t\t\tName               : %s\n", s.studentName);
            printf("\t\t\tFather's Name      : %s\n", s.fatherName);
            printf("\t\t\tMother's Name      : %s\n", s.motherName);
            printf("\t\t\tDate of Birth      : %s\n", s.dateOfBirth);
            printf("\t\t\tAddress            : %s\n", s.address);
            printf("\t\t\tContact Number     : %s\n", s.contactNumber);
            printf("\t\t\tCourse             : %s\n", s.course);
            printf("\t\t\tAttendance         : %.2f%%\n", s.attendancePercentage);
            printf("\t\t\tGrade              : %.2f\n", s.grade);
            printf("\t\t\t----------------------------\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\n\t\t\tStudent with ID %d not found.\n", id);
    }

    fclose(file);
    pressEnterToContinue();
}

void updateStudent() {
    clearScreen();
    printHeader("UPDATE STUDENT RECORD");

    int id;
    struct Student s;
    printf("\n\t\t\tEnter Student ID to update: ");
    id = validateNumberInput();

    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("\n\t\t\tError opening files.\n");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        pressEnterToContinue();
        return;
    }

    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.studentID == id) {
            found = 1;
            printf("\n\t\t\t--- Record Found: %s ---\n", s.studentName);
            printf("\t\t\tWhat do you want to update?\n");
            printf("\t\t\t1. Name\n");
            printf("\t\t\t2. Address\n");
            printf("\t\t\t3. Contact Number\n");
            printf("\t\t\t4. Course\n");
            printf("\t\t\t5. Back to Menu\n");
            printf("\n\t\t\tEnter your choice: ");
            int choice = validateNumberInput();

            switch(choice) {
                case 1:
                    printf("\t\t\tEnter new name: ");
                    fgets(s.studentName, MAX_NAME_SIZE, stdin);
                    s.studentName[strcspn(s.studentName, "\n")] = 0;
                    break;
                case 2:
                    printf("\t\t\tEnter new address: ");
                    fgets(s.address, MAX_ADDRESS_SIZE, stdin);
                    s.address[strcspn(s.address, "\n")] = 0;
                    break;
                case 3:
                    do {
                        printf("\t\t\tEnter new contact number: ");
                        fgets(s.contactNumber, MAX_CONTACT_SIZE, stdin);
                        s.contactNumber[strcspn(s.contactNumber, "\n")] = 0;
                        if (!validatePhone(s.contactNumber)) {
                            printf("\t\t\tInvalid phone number. Please enter digits only.\n");
                        }
                    } while (!validatePhone(s.contactNumber));
                    break;
                case 4:
                    printf("\t\t\tEnter new course: ");
                    fgets(s.course, MAX_COURSE_SIZE, stdin);
                    s.course[strcspn(s.course, "\n")] = 0;
                    break;
                case 5:
                    fclose(file);
                    fclose(tempFile);
                    remove("temp.txt");
                    return;
                default:
                    printf("\n\t\t\tInvalid choice.\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("temp.txt");
                    pressEnterToContinue();
                    return;
            }
        }
        fprintf(tempFile, "%d,%s,%s,%s,%s,%s,%s,%s,%.2f,%.2f\n",
                s.studentID, s.studentName, s.fatherName, s.motherName,
                s.address, s.contactNumber, s.dateOfBirth, s.course,
                s.attendancePercentage, s.grade);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("\n\t\t\tStudent with ID %d not found.\n", id);
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("\n\t\t\tRecord updated successfully!\n");
    }

    pressEnterToContinue();
}

void deleteStudent() {
    clearScreen();
    printHeader("DELETE STUDENT RECORD");

    int id;
    struct Student s;
    printf("\n\t\t\tEnter Student ID to delete: ");
    id = validateNumberInput();

    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("\n\t\t\tError opening files.\n");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        pressEnterToContinue();
        return;
    }

    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.studentID == id) {
            found = 1;
        } else {
            fprintf(tempFile, "%d,%s,%s,%s,%s,%s,%s,%s,%.2f,%.2f\n",
                    s.studentID, s.studentName, s.fatherName, s.motherName,
                    s.address, s.contactNumber, s.dateOfBirth, s.course,
                    s.attendancePercentage, s.grade);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("\n\t\t\tStudent record for ID %d deleted.\n", id);
    } else {
        remove("temp.txt");
        printf("\n\t\t\tStudent with ID %d not found.\n", id);
    }

    pressEnterToContinue();
}

void sortStudents() {
    clearScreen();
    printHeader("SORT STUDENT RECORDS");

    printf("\n\t\t\t1. Sort by ID");
    printf("\n\t\t\t2. Sort by Name");
    printf("\n\t\t\t3. Sort by Grade");
    printf("\n\t\t\t4. Back to Menu");
    printf("\n\n\t\t\tEnter your choice: ");
    
    int choice = validateNumberInput();
    if (choice == 4) return;

    struct Student students[MAX_STUDENTS];
    int count = 0;
    
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("\n\t\t\tNo student records found.\n");
        pressEnterToContinue();
        return;
    }

    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &students[count].studentID, students[count].studentName, 
                 students[count].fatherName, students[count].motherName,
                 students[count].address, students[count].contactNumber,
                 students[count].dateOfBirth, students[count].course,
                 &students[count].attendancePercentage, &students[count].grade) != EOF) {
        count++;
        if (count >= MAX_STUDENTS) break;
    }
    fclose(file);

    if (count == 0) {
        printf("\n\t\t\tNo records to sort.\n");
        pressEnterToContinue();
        return;
    }

    for (int i = 0; i < count-1; i++) {
        for (int j = 0; j < count-i-1; j++) {
            int swap = 0;
            
            switch(choice) {
                case 1:
                    if (students[j].studentID > students[j+1].studentID) swap = 1;
                    break;
                case 2:
                    if (strcmp(students[j].studentName, students[j+1].studentName) > 0) swap = 1;
                    break;
                case 3:
                    if (students[j].grade < students[j+1].grade) swap = 1;
                    break;
                default:
                    printf("\n\t\t\tInvalid choice.\n");
                    pressEnterToContinue();
                    return;
            }
            
            if (swap) {
                struct Student temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }

    printf("\n\t%-8s %-25s %-20s %-10s\n", "ID", "Name", "Course", "Grade");
    printf("\t--------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("\t%-8d %-25s %-20s %-10.2f\n",
               students[i].studentID, students[i].studentName, 
               students[i].course, students[i].grade);
    }
    
    printf("\t--------------------------------------------------------\n");
    pressEnterToContinue();
}

void displayStatistics() {
    clearScreen();
    printHeader("SYSTEM STATISTICS");

    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("\n\t\t\tNo student records found.\n");
        pressEnterToContinue();
        return;
    }

    int count = 0;
    float totalGrade = 0, totalAttendance = 0;
    float minGrade = 100, maxGrade = 0;
    float minAttendance = 100, maxAttendance = 0;

    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        count++;
        totalGrade += s.grade;
        totalAttendance += s.attendancePercentage;
        
        if (s.grade < minGrade) minGrade = s.grade;
        if (s.grade > maxGrade) maxGrade = s.grade;
        
        if (s.attendancePercentage < minAttendance) minAttendance = s.attendancePercentage;
        if (s.attendancePercentage > maxAttendance) maxAttendance = s.attendancePercentage;
    }
    fclose(file);

    if (count == 0) {
        printf("\n\t\t\tNo records to analyze.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n\t\t\tTotal Students      : %d", count);
    printf("\n\t\t\tAverage Grade       : %.2f", totalGrade/count);
    printf("\n\t\t\tHighest Grade       : %.2f", maxGrade);
    printf("\n\t\t\tLowest Grade        : %.2f", minGrade);
    printf("\n\t\t\tAverage Attendance  : %.2f%%", totalAttendance/count);
    printf("\n\t\t\tHighest Attendance  : %.2f%%", maxAttendance);
    printf("\n\t\t\tLowest Attendance   : %.2f%%", minAttendance);
    printf("\n\n\t\t\tGrade Distribution:");
    
    int excellent = 0, good = 0, average = 0, needsImprovement = 0, poor = 0;
    file = fopen(STUDENT_FILE, "r");
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.grade >= 90) excellent++;
        else if (s.grade >= 75) good++;
        else if (s.grade >= 50) average++;
        else if (s.grade >= 33) needsImprovement++;
        else poor++;
    }
    fclose(file);
    
    printf("\n\t\t\tExcellent (90-100)  : %d (%.1f%%)", excellent, (float)excellent/count*100);
    printf("\n\t\t\tGood (75-89)        : %d (%.1f%%)", good, (float)good/count*100);
    printf("\n\t\t\tAverage (50-74)     : %d (%.1f%%)", average, (float)average/count*100);
    printf("\n\t\t\tNeeds Improvement (33-49): %d (%.1f%%)", needsImprovement, (float)needsImprovement/count*100);
    printf("\n\t\t\tPoor (0-32)         : %d (%.1f%%)", poor, (float)poor/count*100);
    
    printf("\n");
    pressEnterToContinue();
}

void manageAttendance() {
    clearScreen();
    printHeader("UPDATE ATTENDANCE");

    int id;
    struct Student s;
    printf("\n\t\t\tEnter Student ID to update attendance: ");
    id = validateNumberInput();

    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("\n\t\t\tError opening files.\n");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        pressEnterToContinue();
        return;
    }

    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.studentID == id) {
            found = 1;
            printf("\n\t\t\t--- Record Found: %s ---", s.studentName);
            printf("\n\t\t\tCurrent Attendance: %.2f%%\n", s.attendancePercentage);
            
            do {
                printf("\n\t\t\tEnter new attendance percentage: ");
                scanf("%f", &s.attendancePercentage);
                if (s.attendancePercentage < 0 || s.attendancePercentage > 100) {
                    printf("\t\t\tInvalid percentage. Enter between 0-100.\n");
                }
            } while (s.attendancePercentage < 0 || s.attendancePercentage > 100);

            clearInputBuffer();
        }
        fprintf(tempFile, "%d,%s,%s,%s,%s,%s,%s,%s,%.2f,%.2f\n",
                s.studentID, s.studentName, s.fatherName, s.motherName,
                s.address, s.contactNumber, s.dateOfBirth, s.course,
                s.attendancePercentage, s.grade);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("\n\t\t\tStudent with ID %d not found.\n", id);
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("\n\t\t\tAttendance updated successfully!\n");
    }

    pressEnterToContinue();
}

void manageGrades() {
    clearScreen();
    printHeader("UPDATE GRADES");

    int id;
    struct Student s;
    printf("\n\t\t\tEnter Student ID to update grade: ");
    id = validateNumberInput();

    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("\n\t\t\tError opening files.\n");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        pressEnterToContinue();
        return;
    }

    int found = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (s.studentID == id) {
            found = 1;
            printf("\n\t\t\t--- Record Found: %s ---", s.studentName);
            printf("\n\t\t\tCurrent Grade: %.2f\n", s.grade);
            
            do {
                printf("\n\t\t\tEnter new grade (0-100): ");
                scanf("%f", &s.grade);
                if (s.grade < 0 || s.grade > 100) {
                    printf("\t\t\tInvalid grade. Enter between 0-100.\n");
                }
            } while (s.grade < 0 || s.grade > 100);

            clearInputBuffer();
        }
        fprintf(tempFile, "%d,%s,%s,%s,%s,%s,%s,%s,%.2f,%.2f\n",
                s.studentID, s.studentName, s.fatherName, s.motherName,
                s.address, s.contactNumber, s.dateOfBirth, s.course,
                s.attendancePercentage, s.grade);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("\n\t\t\tStudent with ID %d not found.\n", id);
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("\n\t\t\tGrade updated successfully!\n");
    }

    pressEnterToContinue();
}

void viewPerformanceChart() {
    clearScreen();
    printHeader("STUDENT PERFORMANCE REPORT");

    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("\n\t\t\tNo student records found.\n");
        pressEnterToContinue();
        return;
    }

    int count = 0;
    printf("\n\t%-8s %-25s %-15s %-15s %-15s\n", "ID", "Name", "Grade", "Attendance", "Performance");
    printf("\t-----------------------------------------------------------------------------------\n");

    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        char performance[20];
        if (s.grade >= 90) strcpy(performance, "Excellent");
        else if (s.grade >= 75) strcpy(performance, "Good");
        else if (s.grade >= 50) strcpy(performance, "Average");
        else if (s.grade >= 33) strcpy(performance, "Needs Improvement");
        else strcpy(performance, "Poor");

        printf("\t%-8d %-25s %-15.2f %-15.2f%% %-15s\n",
               s.studentID, s.studentName, s.grade, s.attendancePercentage, performance);
        count++;
    }

    if (count == 0) {
        printf("\n\t\t\tNo records to display.\n");
    }

    printf("\t-----------------------------------------------------------------------------------\n");
    fclose(file);
    pressEnterToContinue();
}

void backupData() {
    clearScreen();
    printHeader("BACKUP DATA");

    FILE *sourceFile = fopen(STUDENT_FILE, "r");
    if (sourceFile == NULL) {
        printf("\n\t\t\tNo student data file to back up.\n");
        pressEnterToContinue();
        return;
    }

    char backupFileName[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(backupFileName, "students_backup_%04d%02d%02d_%02d%02d%02d.txt",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE *backupFile = fopen(backupFileName, "w");
    if (backupFile == NULL) {
        printf("\n\t\t\tError creating backup file.\n");
        fclose(sourceFile);
        pressEnterToContinue();
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), sourceFile)) {
        fputs(buffer, backupFile);
    }

    fclose(sourceFile);
    fclose(backupFile);
    printf("\n\t\t\tBackup created successfully!\n");
    printf("\t\t\tFile saved as: %s\n", backupFileName);
    pressEnterToContinue();
}

void restoreData() {
    clearScreen();
    printHeader("RESTORE DATA");

    printf("\n\t\t\tWarning: This will overwrite current student data!");
    printf("\n\t\t\tAre you sure you want to continue? (y/n): ");
    
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (tolower(confirm) != 'y') {
        printf("\n\t\t\tRestore canceled.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n\t\t\tEnter backup filename to restore: ");
    char filename[100];
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    FILE *backupFile = fopen(filename, "r");
    if (backupFile == NULL) {
        printf("\n\t\t\tError opening backup file.\n");
        pressEnterToContinue();
        return;
    }

    FILE *studentFile = fopen(STUDENT_FILE, "w");
    if (studentFile == NULL) {
        printf("\n\t\t\tError opening student file.\n");
        fclose(backupFile);
        pressEnterToContinue();
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), backupFile)) {
        fputs(buffer, studentFile);
    }

    fclose(backupFile);
    fclose(studentFile);
    printf("\n\t\t\tData restored successfully from %s!\n", filename);
    pressEnterToContinue();
}

void searchByName() {
    clearScreen();
    printHeader("SEARCH BY NAME");

    char searchTerm[MAX_NAME_SIZE];
    printf("\n\t\t\tEnter name or part of name to search: ");
    fgets(searchTerm, MAX_NAME_SIZE, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("\n\t\t\tNo student records found.\n");
        pressEnterToContinue();
        return;
    }

    int found = 0;
    printf("\n\t%-8s %-25s %-20s %-15s\n", "ID", "Name", "Course", "Contact");
    printf("\t--------------------------------------------------------\n");

    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f\n",
                 &s.studentID, s.studentName, s.fatherName, s.motherName,
                 s.address, s.contactNumber, s.dateOfBirth, s.course,
                 &s.attendancePercentage, &s.grade) != EOF) {
        if (strstr(s.studentName, searchTerm) != NULL) {
            printf("\t%-8d %-25s %-20s %-15s\n",
                   s.studentID, s.studentName, s.course, s.contactNumber);
            found++;
        }
    }

    if (!found) {
        printf("\n\t\t\tNo students found matching '%s'.\n", searchTerm);
    } else {
        printf("\n\t\t\tFound %d matching students.\n", found);
    }

    printf("\t--------------------------------------------------------\n");
    fclose(file);
    pressEnterToContinue();
}