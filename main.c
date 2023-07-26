#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//------------------DEFINE------------------
#define NUM_LEVELS 12
#define NUM_CLASSES 10
#define NUM_COURSES 10

typedef struct Course {
    char c_name[50];
    int grade;
} Course;

typedef struct Student {
    char first_name[50];
    char last_name[50];
    int grades[10];
    int level;
    int class;
    char phone[10];
    Course course[NUM_COURSES]; // Use the Course structure directly
    struct Student* next;
} Student;

struct school {
    struct Student* DB[NUM_LEVELS][NUM_CLASSES];
};
static struct school s;

struct Student* create_student(char* line) {
    struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));
    if (new_student != NULL) {
        // Parse the line and fill in student information
        sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d ",
               new_student->first_name, new_student->last_name, new_student->phone,
               &new_student->level, &new_student->class, &new_student->grades[0], &new_student->grades[1],
               &new_student->grades[2], &new_student->grades[3], &new_student->grades[4],
               &new_student->grades[5], &new_student->grades[6], &new_student->grades[7],
               &new_student->grades[8], &new_student->grades[9], &new_student->course[0].grade,
               &new_student->course[1].grade, &new_student->course[2].grade, &new_student->course[3].grade,
               &new_student->course[4].grade, &new_student->course[5].grade, &new_student->course[6].grade,
               &new_student->course[7].grade, &new_student->course[8].grade, &new_student->course[9].grade);

        new_student->next = NULL;

        // Populate course names (A, B, C, etc.) and grades for each course
        char course_name = 'A';
        for (int i = 0; i < NUM_COURSES; i++) {
            snprintf(new_student->course[i].c_name, sizeof(new_student->course[i].c_name), "%c", course_name++);
            new_student->course[i].grade = new_student->grades[i];
        }
    }
    return new_student;
}

// Function to insert a student into the appropriate class's linked list
void insert_student(int level, int class, struct Student* new_student) {
    if (level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES) {
        if (s.DB[level][class] == NULL) {
            s.DB[level][class] = new_student;
        } else {
            struct Student* current = s.DB[level][class];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_student;
        }
    }
}

void add_grades_to_course(struct Student* student, int course_index, int grade) {
    if (course_index >= 0 && course_index < NUM_COURSES) {
        student->course[course_index].grade = grade;
    }
}

void print_data_for_cell(int level, int class) {
    if (level >= 0 && level < NUM_LEVELS && class >= 0 && class < NUM_CLASSES) {
        struct Student* current = s.DB[level][class];
        printf("Data for Level %d, Class %d:\n", level + 1, class + 1);

        while (current != NULL) {
            printf("Student Name: %s %s\n", current->first_name, current->last_name);
            printf("Phone Number: %s\n", current->phone);
            printf("Grades: ");
            for (int i = 0; i < 10; i++) {
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

void INITDB() {
    FILE* file = fopen("/Users/rashidab/Desktop/students.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[256];
    int level, class;

    while (fgets(line, sizeof(line), file)) {
        // Parse the line and create a new student
        struct Student* new_student = create_student(line);


        // Determine the correct level and class
        level = new_student->level - 1; // Adjust level to 0-based index
        class = new_student->class - 1; // Assuming phone number determines class, adjust to 0-based index

        // Insert the student into the appropriate class's linked list
        insert_student(level, class, new_student);
    }

    fclose(file);
}

int main() {
    INITDB();

    print_data_for_cell(0, 0); // Level 1, Class 1
    print_data_for_cell(2, 3); // Level 3, Class 4
    // Add more calls to print data for other cells...



    return 0;
}
