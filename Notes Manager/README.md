# Notes Manager CLI

A simple command-line notes manager written in C with SQLite3 for persistent storage.

## Features

- Add, list, view, edit, and delete notes
- SQLite3 database for persistent storage
- Simple and clean command-line interface
- Automatic database creation

## Build

Make sure you have SQLite3 development libraries installed:

```bash
# Ubuntu/Debian
sudo apt-get install libsqlite3-dev

# macOS
brew install sqlite3

# Fedora/CentOS
sudo yum install sqlite-devel
```

Then build the project:

```bash
make
```

## Usage

### Add a note
```bash
./notes add "Shopping List" "Buy milk, eggs, bread"
```

### List all notes
```bash
./notes list
```

### View a specific note
```bash
./notes view 1
```

### Edit a note
```bash
./notes edit 1 "Updated List" "Milk, eggs, bread, cheese"
```

### Delete a note
```bash
./notes delete 1
```

### Show help
```bash
./notes help
```

## Example Session

```bash
$ ./notes add "Shopping List" "Buy milk, eggs, bread"
Note added successfully!

$ ./notes list
ID | Title                    | Created At
---|--------------------------|-------------------
1  | Shopping List            | 2025-09-13 12:00:00

$ ./notes view 1
Title: Shopping List
Content:
Buy milk, eggs, bread

$ ./notes edit 1 "Updated List" "Milk, eggs, bread, cheese"
Note updated successfully!

$ ./notes delete 1
Note deleted successfully!
```

## Database

The application uses SQLite3 with the following schema:

```sql
CREATE TABLE notes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    content TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

The database file (`notes.db`) is created automatically in the current directory.

## Clean Build

To clean the build:

```bash
make clean
```
