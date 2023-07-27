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
    // CHECK IF ARGUMENTS ARE VALID
    if (!(level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES))
        printf("LEVEL/CLASS ARGUMENTS ARE NOT VALID");

    struct Student* curr = s.DB[level-1][class-1];
    struct Student* prev = NULL;
    // FIND THE STUDENT LOCATION
    while(curr != NULL) {
        if(strcmp(curr->first_name, first_name) == 0 && strcmp(curr->last_name, last_name) == 0) {
            if (prev == NULL) {
                s.DB[level][class] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            printf("Student %s %s removed from Level %d, Class %d.\n", first_name, last_name, level + 1, class + 1);
            printf("-------------------------------\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Student %s %s not found in Level %d, Class %d.\n", first_name, last_name, level + 1, class + 1);
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



//-----------------------------------MAIN-----------------------------------
int main() {
    INITDB();

    // Add a new student and link courses
    int new_grades[NUM_OF_GRADES] = {80, 85, 90, 77, 95, 88, 93, 82, 91, 89};
    add_new_student("John", "Doe", "123456789", 3, 4, new_grades);

    // Edit student grade
    edit_student_grade("John", "Doe", 3 ,4, 'A', 0);

    // Print data after adding the new student
    //print_data_for_cell(3, 4); // Level 3, Class 4 (updated)
    //search_student("John", "Doe");
    top_students_in_course('A');
    return 0;
}
