#include <a_samp>
#include <a_mysql>
#include <easy-db.inc>

new MySQL:dbHandle;

public OnGameModeInit()
{
    dbHandle = mysql_connect("127.0.0.1", "user", "pass", "database");
    if(dbHandle == MYSQL_INVALID_HANDLE) {
        print("Failed to connect to MySQL");
        return 0;
    }

    EasyDB_RegisterHandle(dbHandle);
    print("EasyDB version %s ready: %d", EASYDB_VERSION, EasyDB_IsReady());
    EasyDB_ToggleDirtyCheck(true);
    EasyDB_ToggleErrorLog(true);

    // insert example
    ORM_Insert("players", "name, score", "'Alice', 100");

    // save examples
    new currentScore = 150;
    new oldScore = 100;
    ORM_SaveInt("players", "id", 1, "score", currentScore, oldScore);

    new Float:currentMoney = 123.45;
    new Float:oldMoney = 0.0;
    ORM_SaveFloat("players", "id", 1, "money", currentMoney, oldMoney);

    new name[32] = "Bob";
    new oldName[32] = "";
    ORM_SaveString("players", "id", 2, "name", name, oldName);

    new playerActive = true;
    new oldActive = false;
    ORM_SaveBool("players", "id", 3, "active", playerActive, oldActive);

    // batch update
    ORM_StartBatch("players", "id", 1);
    ORM_AddInt(currentScore);
    ORM_AddFloat(currentMoney);
    ORM_AddString(name);
    ORM_AddBool(playerActive);
    ORM_FinishBatch();

    // delete
    ORM_Delete("players", "id", 3);

    // arbitrary select example
    ORM_Query("SELECT name,score FROM players WHERE id=1", "OnArbitraryQuery");

    return 1;
}

public OnEasyDBError(query[], error_id, error_msg[])
{
    printf("Callback: Query '%s' failed (ID %d) - %s", query, error_id, error_msg);
    return 1;
}

public OnArbitraryQuery(query[])
{
    print("Ran custom query: %s", query);
    return 1;
}
