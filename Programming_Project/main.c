#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

#define COLOR_CYAN  "\033[36m"
#define COLOR_RED   "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

typedef struct {
    int complete;
    int index;
    char name[100];
} ToDoItem;

void UpdateInputFile(ToDoItem *list, int count) {
    FILE *file = fopen("input.txt", "w");
    if (file == NULL) {
        printf("Can't not open file\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", list[i].name, list[i].complete);
    }

    fclose(file);
}

void AddItem(ToDoItem **list, int *k, char *name, int complete) {
    *list = realloc(*list, (*k + 1) * sizeof(ToDoItem));
    if (*list == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    (*list)[*k].index = *k;
    strncpy((*list)[*k].name, name, 99);
    (*list)[*k].name[99] = '\0';
    (*list)[*k].complete = complete;

    (*k)++;

    UpdateInputFile(*list, *k);
}

void ViewList(ToDoItem *list, int k) {
    printf("\n~~~ To Do List ~~~\n");
    for (int i = 0; i < k; i++) {
        if (list[i].complete > 0) {
            printf(COLOR_GREEN "[x]" COLOR_RESET " %d: %s\n", list[i].index, list[i].name);
        } else {
            printf(COLOR_RED "[]" COLOR_RESET " %d: %s\n", list[i].index, list[i].name);
        }
    }
    printf("\nPress Enter to return to main menu...");
    getchar();
    getchar();
}

void CompleteItem(ToDoItem *list, int index, int k) {
    CLEAR_SCREEN();
    printf("~~~ Complete Item ~~~\n");
    if (index >= 0 && index < k) {
        list[index].complete = 1;
        printf("Item %d marked as complete!\n", index);
    } else {
        printf("Invalid index.\n");
    }
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void ShowMainMenu() {
    CLEAR_SCREEN();
    printf(COLOR_CYAN "~~~ Main Menu ~~~" COLOR_RESET "\n");
    printf("1. Add Item\n");
    printf("2. View List\n");
    printf("3. Complete Item\n");
    printf("4. Load list\n");
    printf("5. Exit\n");
    printf(COLOR_CYAN "Enter your choice: " COLOR_RESET "\n");
}

void AddItemMenu(ToDoItem **list, int *count) {
    CLEAR_SCREEN();
    printf("~~~ Add New Item ~~~\n");
    char name[100];
    printf(COLOR_CYAN "Enter the name of the item: " COLOR_RESET "\n");
    getchar();
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = '\0';
    AddItem(list, count, name, 0);
    printf("\nItem added!\n");
    printf("Press Enter to continue...");
    getchar();
}

void LoadFromFile(ToDoItem **list, int *count) {
    CLEAR_SCREEN();
    printf("~~~ Load from File ~~~\n");

    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open input.txt\n");
        printf(COLOR_CYAN "\nPress Enter to continue..." COLOR_RESET "\n");
        getchar();
        getchar();
        return;
    }

    char line[100];
    int items_loaded = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) > 0) {
            int complete = 0;
            char *last_space = strrchr(line, ' ');
            if (last_space != NULL) {
                if (strcmp(last_space + 1, "1") == 0) {
                    complete = 1;
                    *last_space = '\0';
                } else if (strcmp(last_space + 1, "0") == 0) {
                    complete = 0;
                    *last_space = '\0';
                }
            }
            AddItem(list, count, line, complete);
            items_loaded++;
        }
    }

    fclose(file);

    printf("Successfully loaded %d items from input.txt\n", items_loaded);
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

int main() {
    ToDoItem *list = NULL;
    int count = 0;
    int input;

    while (1) {
        ShowMainMenu();
        scanf("%d", &input);

        switch(input) {
            case 1:
                AddItemMenu(&list, &count);
                break;

            case 2:
                CLEAR_SCREEN();
                ViewList(list, count);
                break;

            case 3:
                CLEAR_SCREEN();
                printf("~~~ Complete Item ~~~\n");  // Fixed incorrect menu title
                if (count == 0) {
                    printf("List is empty.\n");
                    printf("\nPress Enter to continue...");
                    getchar();
                    getchar();
                    break;
                }
                printf(COLOR_CYAN "Enter the index of the item to complete: " COLOR_RESET "\n");
                int index;
                scanf("%d", &index);
                CompleteItem(list, index, count);
                break;

            case 4:
                LoadFromFile(&list, &count);
                break;

            case 5:
                free(list);
                CLEAR_SCREEN();
                return 0;

            default:
                CLEAR_SCREEN();
                printf("Invalid choice.\n");
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
        }
    }

    free(list);
    return 0;
}
