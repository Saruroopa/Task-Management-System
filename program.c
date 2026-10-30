#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TASKS 100
#define TITLE_LEN 100
#define DESC_LEN 250
#define DATE_LEN 20
typedef struct {
    int id;
    char title[TITLE_LEN];
    char description[DESC_LEN];
    int priority;
    char dueDate[DATE_LEN];
    int completed;
} Task;
Task tasks[MAX_TASKS];
int taskCount = 0;
int nextId = 1;
/* Read a line safely and remove the trailing newline. */
void readLine(const char *prompt, char *buffer, int size) {
    int ch;
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard extra input. */
            }
        }
    }
}
/* Read an integer with input validation. */
int readInt(const char *prompt, int min, int max) {
    char input[50];
    char extra;
    int value;
    while (1) {
        readLine(prompt, input, sizeof(input));
        if (sscanf(input, " %d %c", &value, &extra) == 1 &&
            value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number from %d to %d.\n",
               min, max);
    }
}
/* Find a task by its ID. */
int findTaskIndex(int id) {
    int i;
    for (i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            return i;
        }
    }
    return -1;
}
/* Display one task. */
void displayTask(Task task) {
    printf("\n------------------------------\n");
    printf("Task ID     : %d\n", task.id);
    printf("Title       : %s\n", task.title);
    printf("Description : %s\n", task.description);
    printf("Priority    : ");
    switch (task.priority) {
        case 1:
            printf("High\n");
            break;
        case 2:
            printf("Medium\n");
            break;
        case 3:
            printf("Low\n");
            break;
    }
    printf("Due Date    : %s\n", task.dueDate);
    printf("Status      : %s\n",
           task.completed ? "Completed" : "Pending");
    printf("------------------------------\n");
}
/* Add a new task. */
void addTask(void) {
    Task newTask;
    if (taskCount >= MAX_TASKS) {
        printf("\nTask limit reached. Cannot add more tasks.\n");
        return;
    }
    newTask.id = nextId++;
    printf("\n--- Add New Task ---\n");
    readLine("Enter task title: ",
             newTask.title, sizeof(newTask.title));
    readLine("Enter task description: ",
             newTask.description, sizeof(newTask.description));
    newTask.priority = readInt(
        "Priority (1 = High, 2 = Medium, 3 = Low): ", 1, 3);
    readLine("Enter due date (DD-MM-YYYY): ",
             newTask.dueDate, sizeof(newTask.dueDate));
    newTask.completed = 0;
    tasks[taskCount] = newTask;
    taskCount++;
    printf("\nTask added successfully! Task ID: %d\n", newTask.id);
}
/* View all tasks. */
void viewAllTasks(void) {
    int i;
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }
    printf("\n===== ALL TASKS =====\n");
    for (i = 0; i < taskCount; i++) {
        displayTask(tasks[i]);
    }
}
/* View pending or completed tasks. */
void viewTasksByStatus(int completedStatus) {
    int i;
    int found = 0;
    printf("\n===== %s TASKS =====\n",
           completedStatus ? "COMPLETED" : "PENDING");
    for (i = 0; i < taskCount; i++) {
        if (tasks[i].completed == completedStatus) {
            displayTask(tasks[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No %s tasks found.\n",
               completedStatus ? "completed" : "pending");
    }
}
/* Search tasks by ID or title. */
void searchTask(void) {
    int choice;
    int id;
    int index;
    int i;
    int found = 0;
    char searchTitle[TITLE_LEN];
    printf("\n--- Search Task ---\n");
    printf("1. Search by task ID\n");
    printf("2. Search by title\n");
    choice = readInt("Enter choice: ", 1, 2);
    if (choice == 1) {
        id = readInt("Enter task ID: ", 1, 1000000000);
        index = findTaskIndex(id);
        if (index != -1) {
            displayTask(tasks[index]);
        } else {
            printf("Task not found.\n");
        }
    } else {
        readLine("Enter title to search: ",
                 searchTitle, sizeof(searchTitle));
        for (i = 0; i < taskCount; i++) {
            if (strstr(tasks[i].title, searchTitle) != NULL) {
                displayTask(tasks[i]);
                found = 1;
            }
        }
        if (!found) {
            printf("No matching tasks found.\n");
        }
    }
}
/* Update an existing task. */
void updateTask(void) {
    int id;
    int index;
    printf("\n--- Update Task ---\n");
    id = readInt("Enter task ID to update: ", 1, 1000000000);
    index = findTaskIndex(id);
    if (index == -1) {
        printf("Task not found.\n");
        return;
    }
    printf("Enter the new details below.\n");
    readLine("New title: ",
             tasks[index].title, sizeof(tasks[index].title));
    readLine("New description: ",
             tasks[index].description,
             sizeof(tasks[index].description));
    tasks[index].priority = readInt(
        "New priority (1 = High, 2 = Medium, 3 = Low): ", 1, 3);
    readLine("New due date (DD-MM-YYYY): ",
             tasks[index].dueDate, sizeof(tasks[index].dueDate));
    printf("Task updated successfully!\n");
}
/* Mark a task as completed. */
void completeTask(void) {
    int id;
    int index;
    printf("\n--- Complete Task ---\n");
    id = readInt("Enter task ID: ", 1, 1000000000);
    index = findTaskIndex(id);
    if (index == -1) {
        printf("Task not found.\n");
        return;
    }
    if (tasks[index].completed) {
        printf("This task is already completed.\n");
    } else {
        tasks[index].completed = 1;
        printf("Task marked as completed!\n");
    }
}
/* Delete a task. */
void deleteTask(void) {
    int id;
    int index;
    int i;
    printf("\n--- Delete Task ---\n");
    id = readInt("Enter task ID to delete: ", 1, 1000000000);
    index = findTaskIndex(id);
    if (index == -1) {
        printf("Task not found.\n");
        return;
    }
    for (i = index; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    taskCount--;
    printf("Task deleted successfully!\n");
}
/* Display the main menu. */
void displayMenu(void) {
    printf("\n=================================\n");
    printf("     TASK MANAGEMENT SYSTEM\n");
    printf("=================================\n");
    printf("1. Add Task\n");
    printf("2. View All Tasks\n");
    printf("3. Search Task\n");
    printf("4. Update Task\n");
    printf("5. Mark Task as Completed\n");
    printf("6. Delete Task\n");
    printf("7. View Pending Tasks\n");
    printf("8. View Completed Tasks\n");
    printf("9. Exit\n");
    printf("=================================\n");
}
/* Main function. */
int main(void) {
    int choice;
    printf("Welcome to the Task Management System!\n");
    do {
        displayMenu();
        choice = readInt("Enter your choice (1-9): ", 1, 9);
        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewAllTasks();
                break;
            case 3:
                searchTask();
                break;
            case 4:
                updateTask();
                break;
            case 5:
                completeTask();
                break;
            case 6:
                deleteTask();
                break;
            case 7:
                viewTasksByStatus(0);
                break;
            case 8:
                viewTasksByStatus(1);
                break;
            case 9:
                printf("\nThank you for using the Task Management System!\n");
                break;
        }
    } while (choice != 9);
    return 0;
}