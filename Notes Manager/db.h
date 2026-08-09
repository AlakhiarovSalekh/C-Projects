#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int init_db();
void close_db();
void add_note(const char *title, const char *content);
void list_notes();
void view_note(int id);
void edit_note(int id, const char *new_title, const char *new_content);
void delete_note(int id);

#endif
