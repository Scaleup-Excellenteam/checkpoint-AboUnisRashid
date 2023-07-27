#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-----------------------------------DEFINE-----------------------------------
#define NUM_LEVELS 12
#define NUM_CLASSES 10
#define NUM_COURSES 10
#define LINE_SIZE 256
#define NUM_OF_GRADES 10
#define NAME_LENGTH 50
#define PHONE_NUMBER 10
//-----------------------------------STRUCT-----------------------------------
typedef struct Course {
    char c_name[NAME_LENGTH];
    int grade;
} Course;
//-----------------------------------STRUCT-----------------------------------
typedef struct Student {
    char first_name[NAME_LENGTH];
    char last_name[NAME_LENGTH];
    Course course[NUM_COURSES];
    int grades[NUM_OF_GRADES];
    char phone[PHONE_NUMBER];
    struct Student* next;
    int level;
    int class;
} Student;
//-----------------------------------STRUCT-----------------------------------
struct school {
    struct Student* DB[NUM_LEVELS][NUM_CLASSES];
};
static struct school s;
//-----------------------------------FUNCTION-----------------------------------
struct Student* create_student(char* line) {
    // ALLOCATE STUDENT
    struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));
    // IN CASE ALLOCATION SUCCEEDED
    if (new_student != NULL) {
        // Parse the line and fill in student information (NAME, PHONE, LEVEL, CLASS, GRADES)
        sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d ",
               new_student->first_name, new_student->last_name, new_student->phone,
               &new_student->level, &new_student->class, &new_student->grades[0], &new_student->grades[1],
               &new_student->grades[2], &new_student->grades[3], &new_student->grades[4],
               &new_student->grades[5], &new_student->grades[6], &new_student->grades[7],
               &new_student->grades[8], &new_student->grades[9]);

        new_student->next = NULL;

        // FILL COURSES NAMES AND GRADES, COURSES ARE NAMED BY ABC LETTERS STARTING FROM A
        char course_name = 'A';
        for (int i = 0; i < NUM_COURSES; i++) {
            snprintf(new_student->course[i].c_name, sizeof(new_student->course[i].c_name), "%c", course_name++);
            new_student->course[i].grade = new_student->grades[i];
        }
    }
    return new_student;
}
//-----------------------------------FUNCTION-----------------------------------
// Function to insert a student into the appropriate class's linked list
void insert_student(int level, int class, struct Student* new_student) {
    // CHECK IF LEVEL AND CLASS ARE IN RANGE
    if (level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES) {
        // IN CASE DB FOR CLASS IS EMPTY, NEW STUDENT WILL BE THE HEAD OF THE CLASS
        if (s.DB[level][class] == NULL) {
            s.DB[level][class] = new_student;
        } else {
            // LOOP OVER ALL STUDENTS UNTIL LAST ONE, THEN ADD NEW STUDENT
            struct Student* current = s.DB[level][class];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_student;
        }
    }
}
//-----------------------------------FUNCTION-----------------------------------
void print_data_for_cell(int level, int class) {
    if (level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES) {
        struct Student* current = s.DB[level-1][class-1];
        printf("Data for Level %d, Class %d:\n", level - 1 , class - 1 );

        while (current != NULL) {
            printf("Student Name: %s %s\n", current->first_name, current->last_name);
            printf("Phone Number: %s\n", current->phone);
            printf("Grades: ");
            for (int i = 0; i < NUM_OF_GRADES; i++) {
                printf("%d ", current->grades[i]);
            }
            printf("\n");

            // Print course data
            printf("Courses:\n");
            for (int i = 0; i < NUM_COURSES; i++) {
                printf("Course Name: %s, Grade: %d\n", current->course[i].c_name, current->course[i].grade);
            }

            printf("-------------------------------\n");
            current = current->next;
        }
    } else {
        printf("Invalid level or class!\n");
    }
}
//-----------------------------------FUNCTION-----------------------------------
void INITDB() {
    // OPEN FILE
    FILE* file = fopen("/Users/rashidab/Desktop/students.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    char line[LINE_SIZE];
    int level, class;
    // GET LINE
    while (fgets(line, sizeof(line), file)) {
        // Parse the line and create a new student
        struct Student* new_student = create_student(line);

        // FIX the correct level and class
        level = new_student->level - 1;
        class = new_student->class - 1;

        // Insert the student into the appropriate class's linked list
        insert_student(level, class, new_student);
    }
    fclose(file);
}
//-----------------------------------FUNCTION-----------------------------------
void add_new_student(char* first_name, char* last_name, char* phone, int level, int class, int* grades) {

    // CHECK IF ARGUMENTS ARE VALID
    if (!(level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES))
    {
        printf("LEVEL/CLASS ARGUMENTS ARE NOT VALID");
        return;
    }

    struct Student* new_student = (struct Student*) malloc(sizeof (struct Student));
    // IN CASE ALLOCATION OF STUDENT SUCCEEDED
    if (new_student != NULL) {
        strcpy(new_student->first_name, first_name);
        strcpy(new_student->last_name, last_name);
        strcpy(new_student->phone, phone);
        new_student->level = level;
        new_student->class = class;

        // SET GRADES AND COURSES
        char course_name = 'A';
        for (int i = 0; i < NUM_OF_GRADES; ++i) {
            new_student->grades[i] = grades[i];
            new_student->course[i].grade = grades[i];
            snprintf(new_student->course[i].c_name, sizeof(new_student->course[i].c_name), "%c", course_name++);
        }

        new_student->next = NULL;
        insert_student(level-1, class-1, new_student);
        printf("Student Added in Level %d, Class %d.\n", level, class);
        printf("-------------------------------\n");

    }
    else printf("ERROR ALLOCATING MEMORY FOR NEW STUDENT!\n");
}
//-----------------------------------FUNCTION-----------------------------------
void delete_student(char* first_name, char* last_name, int level, int class) {
    // Check if arguments are valid
    if (!(level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES)) {
        printf("LEVEL/CLASS ARGUMENTS ARE NOT VALID");
        return;
    }

    struct Student* curr = s.DB[level - 1][class - 1];
    struct Student* prev = NULL;

    // Find the student location
    while (curr != NULL) {
        if (strcmp(curr->first_name, first_name) == 0 && strcmp(curr->last_name, last_name) == 0) {
            if (prev == NULL) {
                s.DB[level - 1][class - 1] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            printf("Student %s %s removed from Level %d, Class %d.\n", first_name, last_name, level, class);
            printf("-------------------------------\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("Student %s %s not found in Level %d, Class %d.\n", first_name, last_name, level, class);
}

//-----------------------------------FUNCTION-----------------------------------
struct Student* get_student(char* first_name, char* last_name, int level, int class) {
    if (level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES) {
        struct Student* current = s.DB[level - 1][class - 1];

        while (current != NULL) {
            if (strcmp(current->first_name, first_name) == 0 && strcmp(current->last_name, last_name) == 0) {
                return current;
            }
            current = current->next;
        }

        printf("Student %s %s not found in Level %d, Class %d.\n", first_name, last_name, level + 1, class + 1);
        return NULL;
    } else {
        printf("Invalid level or class!\n");
        return NULL;
    }
}
//-----------------------------------FUNCTION-----------------------------------
void edit_student_grade(char* first_name, char* last_name, int level, int class, char course, int grade) {
    struct Student* curr = get_student(first_name, last_name, level, class);
    if (curr == NULL) return; // STUDENT NOT FOUND ----> NOTHING TO CHANGE

    for (int i = 0; i < NUM_COURSES; ++i) {
        if (curr->course[i].c_name[0] == course) { // Compare the first character of course names
            curr->course[i].grade = grade;
            curr->grades[i] = grade;
            printf("Grade for student %s %s in course %c updated to %d\n", first_name, last_name, course, grade);
            printf("-------------------------------\n");
            return;
        }
    }
    printf("Course %c not found for student %s %s in Level %d, Class %d.\n", course, first_name, last_name, level + 1, class + 1);
}
//-----------------------------------FUNCTION-----------------------------------
void search_student(char* first_name, char* last_name) {
    for (int level = 0; level < NUM_LEVELS; ++level) {
        for (int class = 0; class < NUM_CLASSES; ++class) {
            struct Student* curr = s.DB[level][class];
            while (curr != NULL) {
                if (strcmp(curr->first_name, first_name) == 0 && strcmp(curr->last_name, last_name) == 0) {
                    printf("Student Found:\n");
                    printf("Student Name: %s %s\n", curr->first_name, curr->last_name);
                    printf("Phone Number: %s\n", curr->phone);
                    printf("Level: %d\n", curr->level + 1);
                    printf("Class: %d\n", curr->class + 1);

                    // Print course data
                    printf("Courses:\n");
                    for (int i = 0; i < NUM_COURSES; i++) {
                        printf("Course Name: %s, Grade: %d\n", curr->course[i].c_name, curr->course[i].grade);
                    }

                    printf("-------------------------------\n");
                    return;
                }
                curr = curr->next;
            }
        }
    }
    printf("Student %s %s not found.\n", first_name, last_name);
}

//-----------------------------------FUNCTION-----------------------------------
void top_students_in_course(char course) {
    printf("Top 10 Students in Course %c:\n", course);
    for (int level = 0; level < NUM_LEVELS; ++level) {
        struct Student* current = NULL;
        struct Student* top_students[10] = {NULL};
        int top_grades[10] = {0};

        for (int class = 0; class < NUM_CLASSES; ++class) {
            current = s.DB[level][class];

            while (current != NULL) {
                for (int i = 0; i < NUM_COURSES; i++) {
                    if (current->course[i].c_name[0] == course) {
                        int grade = current->course[i].grade;

                        // Insert the student into the top_students array in descending order of grades
                        for (int j = 0; j < 10; j++) {
                            if (grade > top_grades[j]) {
                                for (int k = 9; k > j; k--) {
                                    top_students[k] = top_students[k - 1];
                                    top_grades[k] = top_grades[k - 1];
                                }
                                top_students[j] = current;
                                top_grades[j] = grade;
                                break;
                            }
                        }
                        break;
                    }
                }
                current = current->next;
            }
        }

        printf("Data for Level %d:\n", level + 1);
        for (int i = 0; i < 10; i++) {
            if (top_students[i] != NULL) {
                printf("Student Name: %s %s, Grade: %d\n", top_students[i]->first_name, top_students[i]->last_name, top_grades[i]);
            }
        }
        printf("-------------------------------\n");
    }
}
//-----------------------------------FUNCTION-----------------------------------
void candidates_for_leaving(int max_grades_sum) {
    printf("Students Candidates for Leaving:\n");
    for (int level = 0; level < NUM_LEVELS; ++level) {
        for (int class = 0; class < NUM_CLASSES; ++class) {
            struct Student* current = s.DB[level][class];
            while (current != NULL) {
                int grades_sum = 0;
                for (int i = 0; i < NUM_COURSES; i++) {
                    grades_sum += current->course[i].grade;
                }

                if (grades_sum <= max_grades_sum) {
                    printf("Student Name: %s %s, Level: %d, Class: %d\n", current->first_name, current->last_name, level + 1, class + 1);
                }
                current = current->next;
            }
        }
    }
    printf("-------------------------------\n");
}
//-----------------------------------FUNCTION-----------------------------------
void average_grade_for_course_in_level(char course, int level) {
    printf("Average Grade for Course %c in Level %d:\n", course, level);

    if (level < 1 || level > NUM_LEVELS) {
        printf("Invalid level!\n");
        return;
    }

    int total_grades = 0;
    int num_students = 0;

    // Calculate total grades and number of students for the given course in the specified level
    for (int class = 0; class < NUM_CLASSES; ++class) {
        struct Student* current = s.DB[level - 1][class];
        while (current != NULL) {
            for (int i = 0; i < NUM_COURSES; ++i) {
                if (current->course[i].c_name[0] == course) {
                    total_grades += current->course[i].grade;
                    num_students++;
                    break; // Exit the inner loop since we found the course in the current student
                }
            }
            current = current->next;
        }
    }

    if (num_students > 0) {
        double average_grade = (double)total_grades / num_students;
        printf("Average Grade: %.2f\n", average_grade);
    } else {
        printf("No students enrolled in Course %c in Level %d.\n", course, level);
    }
}
//-----------------------------------FUNCTION-----------------------------------
void displayMainMenu() {
    printf("\nSelect an option:\n");
    printf("1. Add a new student\n");
    printf("2. Edit student grade\n");
    printf("3. Print data for a cell (level, class)\n");
    printf("4. Search for a student\n");
    printf("5. Top students in a course\n");
    printf("6. Candidates for leaving\n");
    printf("7. Average grade for a course in a level\n");
    printf("8. Delete a student\n"); // New option for deleting a student
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

//-----------------------------------FUNCTION-----------------------------------
void processMenuChoice(int choice) {
    char first_name[NAME_LENGTH], last_name[NAME_LENGTH], phone[PHONE_NUMBER];
    int level, class, grades[NUM_OF_GRADES], grade;
    char course;
    int max_grades_sum;

    switch (choice) {
        case 1:
            printf("Enter first name: ");
            scanf("%s", first_name);
            printf("Enter last name: ");
            scanf("%s", last_name);
            printf("Enter phone number: ");
            scanf("%s", phone);
            printf("Enter level: ");
            scanf("%d", &level);
            printf("Enter class: ");
            scanf("%d", &class);
            printf("Enter grades (10 grades separated by spaces): ");
            for (int i = 0; i < NUM_OF_GRADES; i++) {
                scanf("%d", &grades[i]);
            }
            add_new_student(first_name, last_name, phone, level, class, grades);
            break;
        case 2:
            printf("Enter first name: ");
            scanf("%s", first_name);
            printf("Enter last name: ");
            scanf("%s", last_name);
            printf("Enter level: ");
            scanf("%d", &level);
            printf("Enter class: ");
            scanf("%d", &class);
            printf("Enter course name (single character): ");
            scanf(" %c", &course);
            printf("Enter new grade: ");
            scanf("%d", &grade);
            edit_student_grade(first_name, last_name, level, class, course, grade);
            break;
        case 3:
            printf("Enter level: ");
            scanf("%d", &level);
            printf("Enter class: ");
            scanf("%d", &class);
            print_data_for_cell(level, class);
            break;
        case 4:
            printf("Enter first name: ");
            scanf("%s", first_name);
            printf("Enter last name: ");
            scanf("%s", last_name);
            search_student(first_name, last_name);
            break;
        case 5:
            printf("Enter course name (single character): ");
            scanf(" %c", &course);
            top_students_in_course(course);
            break;
        case 6:
            printf("Enter maximum sum of grades: ");
            scanf("%d", &max_grades_sum);
            candidates_for_leaving(max_grades_sum);
            break;
        case 7:
            printf("Enter course name (single character): ");
            scanf(" %c", &course);
            printf("Enter level: ");
            scanf("%d", &level);
            average_grade_for_course_in_level(course, level);
            break;
        case 8: // New case for deleting a student
            printf("Enter first name: ");
            scanf("%s", first_name);
            printf("Enter last name: ");
            scanf("%s", last_name);
            printf("Enter level: ");
            scanf("%d", &level);
            printf("Enter class: ");
            scanf("%d", &class);
            delete_student(first_name, last_name, level, class);
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}
//-----------------------------------MAIN-----------------------------------
int main() {
    INITDB();

    int choice;
    do {
        displayMainMenu();
        scanf("%d", &choice);
        processMenuChoice(choice);
    } while (choice != 0);

    return 0;
}
