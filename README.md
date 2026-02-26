# 🎯 EasyDB ORM for open.mp / SA-MP

**EasyDB** adalah library Object-Relational Mapping (ORM) untuk open.mp / SA-MP yang membuat operasi MySQL di skrip Pawn menjadi lebih mudah dan terstruktur. Terinspirasi oleh konsep ORM modern, EasyDB mengotomatisasi tugas database umum, meningkatkan pemeliharaan kode, dan menyediakan penanganan error yang handal.

## ✨ Features

- 🗄️ **Automated MySQL Mapping**: Mudah memetakan variabel Pawn ke kolom/tabel MySQL.
- 🧠 **Smart Dirty Checking**: Hanya melakukan update ketika data benar-benar berubah, mengurangi query berlebih.
- 🔄 **Batch Update**: Perbarui beberapa kolom dalam satu query untuk efisiensi.
- 🚨 **Auto Error Logging**: Mencatat error SQL dan query gagal secara otomatis di log server.

## 🛠️ Usage

### Version
The library exposes a version string constant `EASYDB_VERSION` which can be printed for debugging.

### 1. Register MySQL Handle
```pawn
EasyDB_RegisterHandle(MySQL:handle);
```
### 2. Toggle Dirty Checking & Logging
```pawn
EasyDB_ToggleDirtyCheck(true); // Enable
EasyDB_ToggleDirtyCheck(false); // Disable
EasyDB_ToggleErrorLog(true);   // Enable console error messages
EasyDB_ToggleErrorLog(false);  // Disable logging

// or set both at once:
EasyDB_SetMode(true, false);  // dirty check on, logging off
```
### 3. Insert Data
```pawn
ORM_Insert("table_name", "col1, col2", "'val1', 'val2'");
```

### 4. Save/Update Data
```pawn
ORM_SaveInt("table", "pk_col", pk_id, "column", current_val, old_val);
ORM_SaveFloat("table", "pk_col", pk_id, "column", current_val, old_val);
ORM_SaveString("table", "pk_col", pk_id, "column", value, oldValue);
ORM_SaveBool("table", "pk_col", pk_id, "column", boolVal, oldBool);
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
ORM_LoadBool(var4);    // returns 0/1 stored by cache
```

## ⚠️ Error Handling
By default SQL errors and failed queries are logged to the server console. You can disable or re-enable logging with `EasyDB_ToggleErrorLog`.

An optional forward `OnEasyDBError(query[], error_id, error_msg[])` fires whenever a query fails, allowing your script to react programmatically.

## 🧩 Additional Helpers

- `EasyDB_IsReady()` returns 1 if a valid handle is registered.
- `ORM_Query(query, callback)` sends any SQL statement where callback is the public function name to be invoked with the original query string.

## ✅ Requirements
- [a_mysql](https://github.com/pBlueG/SA-MP-MySQL) include for Pawn
- open.mp or SA-MP server

## 📄 License
MIT License

## 👤 Author
brianrstp (Inspired Version)
