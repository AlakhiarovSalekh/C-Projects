#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

#define MAX_URL_LEN 2048
#define SHORT_CODE_LEN 7

typedef struct {
    sqlite3* db;
} Shortener;

static const char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

void generate_code(char* buf) {
    for (int i = 0; i < SHORT_CODE_LEN - 1; i++) {
        buf[i] = chars[rand() % 62];
    }
    buf[SHORT_CODE_LEN - 1] = '\0';
}

int init_db(Shortener* s, const char* dbpath) {
    int rc = sqlite3_open(dbpath, &s->db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(s->db));
        return 1;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS urls ("
        "code TEXT PRIMARY KEY,"
        "url TEXT NOT NULL,"
        "created INTEGER NOT NULL"
        ");";

    char* err = NULL;
    rc = sqlite3_exec(s->db, sql, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
        return 1;
    }

    return 0;
}

char* shorten(Shortener* s, const char* url) {
    char code[SHORT_CODE_LEN];
    sqlite3_stmt* stmt = NULL;

    // Try generating a unique code
    for (int tries = 0; tries < 10; tries++) {
        generate_code(code);

        const char* check = "SELECT code FROM urls WHERE code = ?;";
        if (sqlite3_prepare_v2(s->db, check, -1, &stmt, NULL) != SQLITE_OK) {
            return NULL;
        }

        sqlite3_bind_text(stmt, 1, code, -1, SQLITE_STATIC);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc == SQLITE_DONE) {
            // Code is unique
            break;
        }
    }

    // Insert the URL
    const char* insert = "INSERT INTO urls (code, url, created) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(s->db, insert, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, code, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, url, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, (sqlite3_int64)time(NULL));

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return strdup(code);
}

char* expand(Shortener* s, const char* code) {
    sqlite3_stmt* stmt = NULL;
    const char* sql = "SELECT url FROM urls WHERE code = ?;";

    if (sqlite3_prepare_v2(s->db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, code, -1, SQLITE_STATIC);

    char* url = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* result = (const char*)sqlite3_column_text(stmt, 0);
        url = strdup(result);
    }

    sqlite3_finalize(stmt);
    return url;
}

void cleanup(Shortener* s) {
    if (s->db) {
        sqlite3_close(s->db);
    }
}

int main(void) {
    srand(time(NULL));

    Shortener s = { 0 };
    if (init_db(&s, "urls.db") != 0) {
        return 1;
    }

    printf("URL Shortener - Commands: shorten <url>, expand <code>, quit\n\n");

    char cmd[16];
    char input[MAX_URL_LEN];

    while (1) {
        printf("> ");
        if (scanf("%15s", cmd) != 1) break;

        if (strcmp(cmd, "quit") == 0) {
            break;
        }
        else if (strcmp(cmd, "shorten") == 0) {
            if (scanf("%2047s", input) != 1) continue;

            char* code = shorten(&s, input);
            if (code) {
                printf("Short code: %s\n", code);
                free(code);
            }
            else {
                printf("Error creating short URL\n");
            }
        }
        else if (strcmp(cmd, "expand") == 0) {
            if (scanf("%2047s", input) != 1) continue;

            char* url = expand(&s, input);
            if (url) {
                printf("URL: %s\n", url);
                free(url);
            }
            else {
                printf("Code not found\n");
            }
        }
        else {
            printf("Unknown command\n");
        }
    }

    cleanup(&s);
    return 0;
}