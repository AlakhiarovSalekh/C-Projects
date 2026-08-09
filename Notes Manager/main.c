#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

void show_help() {
    printf("Notes Manager - Simple CLI\n\n");
    printf("Usage:\n");
    printf("  notes add \"title\" \"content\"    - Add a new note\n");
    printf("  notes list                     - List all notes\n");
    printf("  notes view <id>                - View a specific note\n");
    printf("  notes edit <id> \"title\" \"content\" - Edit a note\n");
    printf("  notes delete <id>              - Delete a note\n");
    printf("  notes help                     - Show this help\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    if (init_db() != 0) {
        printf("Error: Failed to initialize database\n");
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "add") == 0) {
        if (argc != 4) {
            printf("Usage: notes add \"title\" \"content\"\n");
            return 1;
        }
        add_note(argv[2], argv[3]);
    }
    else if (strcmp(command, "list") == 0) {
        list_notes();
    }
    else if (strcmp(command, "view") == 0) {
        if (argc != 3) {
            printf("Usage: notes view <id>\n");
            return 1;
        }
        int id = atoi(argv[2]);
        view_note(id);
    }
    else if (strcmp(command, "edit") == 0) {
        if (argc != 5) {
            printf("Usage: notes edit <id> \"new title\" \"new content\"\n");
            return 1;
        }
        int id = atoi(argv[2]);
        edit_note(id, argv[3], argv[4]);
    }
    else if (strcmp(command, "delete") == 0) {
        if (argc != 3) {
            printf("Usage: notes delete <id>\n");
            return 1;
        }
        int id = atoi(argv[2]);
        delete_note(id);
    }
    else if (strcmp(command, "help") == 0) {
        show_help();
    }
    else {
        printf("Unknown command: %s\n", command);
        show_help();
        return 1;
    }

    close_db();
    return 0;
}