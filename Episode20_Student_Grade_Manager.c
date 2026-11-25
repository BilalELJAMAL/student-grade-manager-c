#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STUDENTS 3
#define NAME_SIZE 50

typedef struct {
    char name[NAME_SIZE];
    double grade;
} Student;

static void remove_newline(char *text) {
    text[strcspn(text, "\n")] = '\0';
}

static int read_grade(double *result) {
    char line[100];
    char *end;
    if (!fgets(line, sizeof line, stdin)) return 0;
    errno = 0;
    double value = strtod(line, &end);
    if (line == end || errno == ERANGE) return 0;
    while (*end == ' ' || *end == '\t') end++;
    if (*end != '\n' && *end != '\0') return 0;
    if (value < 0 || value > 100) return 0;
    *result = value;
    return 1;
}

static double average(const Student students[], int count) {
    double total = 0;
    for (int i = 0; i < count; i++) total += students[i].grade;
    return total / count;
}

static int top_student_index(const Student students[], int count) {
    int top = 0;
    for (int i = 1; i < count; i++) {
        if (students[i].grade > students[top].grade) top = i;
    }
    return top;
}

int main(void) {
    Student students[MAX_STUDENTS];
    puts("STUDENT GRADE MANAGER");
    for (int i = 0; i < MAX_STUDENTS; i++) {
        printf("Name %d: ", i + 1);
        if (!fgets(students[i].name, NAME_SIZE, stdin)) return 1;
        remove_newline(students[i].name);
        if (students[i].name[0] == '\0') {
            fputs("Name cannot be empty.\n", stderr);
            return 1;
        }
        printf("Grade (0 to 100): ");
        if (!read_grade(&students[i].grade)) {
            fputs("Invalid grade. Enter a number from 0 to 100.\n", stderr);
            return 1;
        }
    }
    double avg = average(students, MAX_STUDENTS);
    int top = top_student_index(students, MAX_STUDENTS);
    printf("\nClass average: %.2f\n", avg);
    printf("Top student: %s (%.2f)\n", students[top].name, students[top].grade);

    FILE *file = fopen("grade_report.txt", "w");
    if (!file) { perror("Unable to save report"); return 1; }
    fprintf(file, "Student Grade Report\n");
    for (int i = 0; i < MAX_STUDENTS; i++) {
        fprintf(file, "%s: %.2f\n", students[i].name, students[i].grade);
    }
    fprintf(file, "Average: %.2f\nTop: %s\n", avg, students[top].name);
    if (fclose(file) != 0) { perror("Unable to finish report"); return 1; }
    puts("Report saved to grade_report.txt");
    return 0;
}
