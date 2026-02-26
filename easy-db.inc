/*
 * EasyDB ORM for open.mp / SA-MP
 * Author: brianrstp (Inspired Version)
 * * Features: 
 * - Automated MySQL Mapping
 * - Smart Dirty Checking (Only Save when data changed)
 * - Batch Update (Multiple columns in one query)
 * - Auto Error Logging to server log
 */

#if defined _easy_db_included
    #endinput
#endif
#define _easy_db_included

#include <a_mysql>

#define EASYDB_VERSION "1.0.0"

// --- Internal Variables ---
static MySQL:_edb_handle = MYSQL_INVALID_HANDLE;
static bool:_edb_dirtyCheck = true;
static bool:_edb_logErrors = true;

// forward for scripts to catch errors themselves (optional)
forward OnEasyDBError(query[], error_id, error_msg[]);

// --- Core Functions ---

stock EasyDB_IsReady() {
    return _edb_handle != MYSQL_INVALID_HANDLE;
}
stock EasyDB_RegisterHandle(MySQL:handle) {
    _edb_handle = handle;
    return 1;
}

stock EasyDB_ToggleDirtyCheck(bool:status) {
    _edb_dirtyCheck = status;
}

stock EasyDB_ToggleErrorLog(bool:status) {
    _edb_logErrors = status;
}

stock EasyDB_SetMode(bool:dirtyCheck, bool:logErrors) {
    _edb_dirtyCheck = dirtyCheck;
    _edb_logErrors = logErrors;
}

forward _OnEasyDBQueryFinish(query[]);
public _OnEasyDBQueryFinish(query[]) {
    new error_id = mysql_errno(_edb_handle);
    if(error_id != 0) {
        new error_msg[256];
        mysql_error(error_msg, sizeof(error_msg), _edb_handle);
        if(_edb_logErrors) {
            printf("[EasyDB SQL Error] ID: %d | Message: %s", error_id, error_msg);
            printf("[EasyDB Failed Query] %s", query);
        }
        OnEasyDBError(query, error_id, error_msg);
    }
    return 1;
}

static stock _DB_Execute(query[]) {
    if(_edb_handle == MYSQL_INVALID_HANDLE) {
        print("[EasyDB Error] Handle MySQL belum diregistrasi!");
        return 0;
    }
    return mysql_tquery(_edb_handle, query, "_OnEasyDBQueryFinish", "s", query);
}

stock ORM_Query(const query[], const callback[]) {
    if(_edb_handle == MYSQL_INVALID_HANDLE) return 0;
    return mysql_tquery(_edb_handle, query, callback, "s", query);
}

// --- ORM Functions ---
stock ORM_Insert(const table[], const columns[], const values[]) {
    new query[512];
    mysql_format(_edb_handle, query, sizeof(query), "INSERT INTO %e (%s) VALUES (%s)", table, columns, values);
    return _DB_Execute(query);
}

stock ORM_SaveInt(const table[], const pk_col[], pk_id, const col_name[], &current_val, old_val) {
    if(_edb_dirtyCheck && current_val == old_val) return 0;
    
    new query[256];
    mysql_format(_edb_handle, query, sizeof(query), "UPDATE %e SET %e = %d WHERE %e = %d", table, col_name, current_val, pk_col, pk_id);
    return _DB_Execute(query);
}

stock ORM_SaveFloat(const table[], const pk_col[], pk_id, const col_name[], &Float:current_val, Float:old_val) {
    if(_edb_dirtyCheck && current_val == old_val) return 0;
    
    new query[256];
    mysql_format(_edb_handle, query, sizeof(query), "UPDATE %e SET %e = '%f' WHERE %e = %d", table, col_name, current_val, pk_col, pk_id);
    return _DB_Execute(query);
}

stock ORM_SaveString(const table[], const pk_col[], pk_id, const col_name[], const value[], const old_val[]) {
    if(_edb_dirtyCheck && strcmp(value, old_val) == 0) return 0;
    
    new query[512];
    mysql_format(_edb_handle, query, sizeof(query), "UPDATE %e SET %e = '%e' WHERE %e = %d", table, col_name, value, pk_col, pk_id);
    return _DB_Execute(query);
}

stock ORM_SaveBool(const table[], const pk_col[], pk_id, const col_name[], bool:current_val, bool:old_val) {
    if(_edb_dirtyCheck && current_val == old_val) return 0;
    
    new query[256];
    mysql_format(_edb_handle, query, sizeof(query), "UPDATE %e SET %e = %d WHERE %e = %d", table, col_name, current_val?1:0, pk_col, pk_id);
    return _DB_Execute(query);
}

stock ORM_Delete(const table[], const pk_col[], pk_id) {
    new query[128];
    mysql_format(_edb_handle, query, sizeof(query), "DELETE FROM %e WHERE %e = %d", table, pk_col, pk_id);
    return _DB_Execute(query);
}

static _batch_q[1024];
static _batch_active = 0;

#define ORM_StartBatch(%0,%1,%2) \
    _batch_active = 1; \
    mysql_format(_edb_handle, _batch_q, 1024, "UPDATE %e SET ", %0); \
    new _b_pk[32], _b_id; format(_b_pk, 32, %1); _b_id = %2;

#define ORM_AddInt(%0) \
    format(_batch_q, 1024, "%s %s = %d, ", _batch_q, #%0, %0)

#define ORM_AddFloat(%0) \
    format(_batch_q, 1024, "%s %s = '%f', ", _batch_q, #%0, %0)

#define ORM_AddString(%0) \
    format(_batch_q, 1024, "%s %s = '%e', ", _batch_q, #%0, %0)

#define ORM_AddBool(%0) \
    format(_batch_q, 1024, "%s %s = %d, ", _batch_q, #%0, %0 ? 1 : 0)

#define ORM_FinishBatch() \
    if(_batch_active) { \
        strdel(_batch_q, strlen(_batch_q)-2, strlen(_batch_q)); \
        format(_batch_q, 1024, "%s WHERE %e = %d", _batch_q, _b_pk, _b_id); \
        _DB_Execute(_batch_q); \
        _batch_active = 0; \
    }

#define ORM_LoadInt(%0) cache_get_value_name_int(#%0, %0)
#define ORM_LoadFloat(%0) cache_get_value_name_float(#%0, %0)
#define ORM_LoadString(%0) cache_get_value_name(#%0, %0, sizeof(%0))
#define ORM_LoadBool(%0) \
    cache_get_value_name_int(#%0, %0) // stored as 0/1

