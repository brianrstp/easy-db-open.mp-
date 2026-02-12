# EasyDB ORM for open.mp / SA-MP

EasyDB is an Object-Relational Mapping (ORM) library for open.mp / SA-MP, designed to simplify MySQL database operations in Pawn scripts. Inspired by modern ORM concepts, EasyDB automates common database tasks, improves code maintainability, and provides robust error handling.

## Features

- **Automated MySQL Mapping**: Easily map Pawn variables to MySQL columns and tables.
- **Smart Dirty Checking**: Only updates the database when data has actually changed, reducing unnecessary queries.
- **Batch Update**: Update multiple columns in a single query for efficiency.
- **Auto Error Logging**: Automatically logs SQL errors and failed queries to the server log for easier debugging.

## Usage

### 1. Register MySQL Handle
```pawn
EasyDB_RegisterHandle(MySQL:handle);
```

### 2. Toggle Dirty Checking
```pawn
EasyDB_ToggleDirtyCheck(true); // Enable
EasyDB_ToggleDirtyCheck(false); // Disable
```

### 3. Insert Data
```pawn
ORM_Insert("table_name", "col1, col2", "'val1', 'val2'");
```

### 4. Save/Update Data
```pawn
ORM_SaveInt("table", "id", id_value, "column", current_val, old_val);
ORM_SaveFloat("table", "id", id_value, "column", current_val, old_val);
```

### 5. Delete Data
```pawn
ORM_Delete("table", "id", id_value);
```

### 6. Batch Update
```pawn
ORM_StartBatch("table", "id", id_value);
ORM_AddInt(var1);
ORM_AddFloat(var2);
ORM_AddString(var3);
ORM_FinishBatch();
```

### 7. Load Data from Cache
```pawn
ORM_LoadInt(var1);
ORM_LoadFloat(var2);
ORM_LoadString(var3);
```

## Error Handling
All SQL errors and failed queries are automatically logged to the server log for easier debugging.

## Requirements
- [a_mysql](https://github.com/pBlueG/SA-MP-MySQL) include for Pawn
- open.mp or SA-MP server

## License
MIT License

## Author
Brian Restu Pratiwa (Inspired Version)
