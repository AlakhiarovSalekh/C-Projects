#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "db.h"

static sqlite3 *db = NULL;

int init_db() {
    int rc = sqlite3_open("notes.db", &db);
    if (rc != SQLITE_OK) {
        printf("Error: Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS notes ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "title TEXT NOT NULL,"
                     "content TEXT NOT NULL,"
                     "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP);";

    rc = sqlite3_exec(db, sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        printf("Error: Cannot create table: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    return 0;
}

void close_db() {
    if (db) {
        sqlite3_close(db);
    }
}

void add_note(const char *title, const char *content) {
    const char *sql = "INSERT INTO notes (title, content) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error: Failed to prepare statement\n");
        return;
    }

    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, content, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Note added successfully!\n");
    } else {
        printf("Error: Failed to add note\n");
    }

    sqlite3_finalize(stmt);
}

void list_notes() {
    const char *sql = "SELECT id, title, created_at FROM notes ORDER BY id;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error: Failed to prepare statement\n");
        return;
    }

    printf("ID | Title                    | Created At\n");
    printf("---|--------------------------|-------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char*)sqlite3_column_text(stmt, 1);
        const char *created_at = (const char*)sqlite3_column_text(stmt, 2);

        printf("%-2d | %-24s | %s\n", id, title, created_at);
    }

    sqlite3_finalize(stmt);
}

void view_note(int id) {
    const char *sql = "SELECT title, content FROM notes WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error: Failed to prepare statement\n");
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *title = (const char*)sqlite3_column_text(stmt, 0);
        const char *content = (const char*)sqlite3_column_text(stmt, 1);

        printf("Title: %s\n", title);
        printf("Content:\n%s\n", content);
    } else {
        printf("Error: Note with ID %d not found\n", id);
    }

    sqlite3_finalize(stmt);
}

void edit_note(int id, const char *new_title, const char *new_content) {
    const char *sql = "UPDATE notes SET title = ?, content = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error: Failed to prepare statement\n");
        return;
    }

    sqlite3_bind_text(stmt, 1, new_title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_content, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            printf("Note updated successfully!\n");
        } else {
            printf("Error: Note with ID %d not found\n", id);
        }
    } else {
        printf("Error: Failed to update note\n");
    }

    sqlite3_finalize(stmt);
}

void delete_note(int id) {
    const char *sql = "DELETE FROM notes WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error: Failed to prepare statement\n");
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            printf("Note deleted successfully!\n");
        } else {
            printf("Error: Note with ID %d not found\n", id);
        }
    } else {
        printf("Error: Failed to delete note\n");
    }

    sqlite3_finalize(stmt);
}
