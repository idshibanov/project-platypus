#include <my_global.h>
#undef min
#undef max
#include <mysql.h>
#include <string.h>
#include <string>

#include "database.h"
using namespace std;

DatabaseServer::DatabaseServer(string host, string user, string password)
{
    _statement = NULL;
    _conn = mysql_init(NULL);
    mysql_real_connect(_conn, host.c_str(), user.c_str(), password.c_str(),
        "test", 0, NULL, 0);
}

DatabaseServer::~DatabaseServer()
{
    mysql_close(_conn);
}

bool DatabaseServer::compare(string user_id, string password)
{
    _statement = new char[512];
    MYSQL_RES* result;
    MYSQL_ROW row;
    int num_rows;
    bool retval = false;
    
    snprintf(_statement, 512, "SELECT Password FROM Passwords WHERE User_Id='%s'", user_id.c_str());
    mysql_query(_conn, _statement);
    result = mysql_store_result(_conn);
    //num_fields = mysql_num_fields(result);
    
    if (result) {
        num_rows = mysql_num_rows( result );
    } else {
        num_rows = 0;
    }

    
    if (num_rows == 1) {
        row = mysql_fetch_row(result);
        string pwd_db(row[0]);
        if (pwd_db.compare(password) == 0)
            retval = true;
    }
    
    delete [] _statement;
    _statement = NULL;
    return retval;
}

void DatabaseServer::update(string user_id, int score)
{
    _statement = new char[512];
    
    snprintf(_statement, 512, "UPDATE Users SET Score='%d' WHERE User_Id='%s'", score, user_id.c_str());
    mysql_query(_conn, _statement);
    
    delete [] _statement;
    _statement = NULL;
}

void DatabaseServer::insert(string user_id, string lastname, string firstname, string address)
{
    int score = 0;
    _statement = new char[512];
    
    snprintf(_statement, 512, "INSERT INTO Users VALUES ('%s', '%s', '%s', '%s', %d)", 
             user_id.c_str(), lastname.c_str(), firstname.c_str(), address.c_str(), score);
    mysql_query(_conn, _statement);
    
    delete [] _statement;
    _statement = NULL;
}

