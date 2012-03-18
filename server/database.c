#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>

void insert(char User_Id[20], char LastName[20], char FirstName[20], char Address[20], int Score)
{
    MYSQL *conn;
    char statement[512];
    conn = mysql_init(NULL);
    mysql_real_connect(conn, "japan.proximity.on.ca", "teamb", "seneca99",
        "test", 0, NULL, 0);

    snprintf(statement, 512, "INSERT INTO Users VALUES ('%s', '%s', '%s', '%s', %d)", User_Id, LastName, FirstName, Address, Score);
    mysql_query(conn, statement);

    mysql_close(conn);
}

void update(char User_Id[20], int Score) 
{
    MYSQL *conn;
    char statement[512];
    conn = mysql_init(NULL);

    mysql_real_connect(conn, "japan.proximity.on.ca", "teamb", "seneca99",
        "test", 0, NULL, 0);

    snprintf(statement, 512, "UPDATE Users SET Score='%d' WHERE User_Id='%s'", Score, User_Id);
    mysql_query(conn, statement);

    mysql_close(conn);
}

void selectScore(char User_Id[20]) 
{
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int num_fields;
    int i=0;

    char statement[512], s[3];
    conn = mysql_init(NULL);

    mysql_real_connect(conn, "japan.proximity.on.ca", "teamb", "seneca99",
        "test", 0, NULL, 0);

    snprintf(statement, 512, "SELECT Score FROM Users WHERE User_Id='%s'", User_Id);
    mysql_query(conn, statement);
    result = mysql_store_result(conn);
    num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result)))
    {
        snprintf(s, 3,  "%s ", row[i] ? row[i] : "NULL");
        printf("s: %s", s);
    }
    mysql_close(conn);
    //return s;
}

void insertPassword(char User_Id[20], char Password[20])
{
    MYSQL *conn;
    char statement[512];
    conn = mysql_init(NULL);
    mysql_real_connect(conn, "japan.proximity.on.ca", "teamb", "seneca99",
        "test", 0, NULL, 0);

    snprintf(statement, 512, "INSERT INTO Passwords VALUES ('%s', '%s')", User_Id, Password);
    mysql_query(conn, statement);

    mysql_close(conn);
}

int comparePassword(char User_Id[20], char testPassword[20]) 
{
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int num_fields;
    int i=0;

    char statement[512], s[255], s1[255];
    conn = mysql_init(NULL);

    mysql_real_connect(conn, "japan.proximity.on.ca", "teamb", "seneca99",
        "test", 0, NULL, 0);

    snprintf(statement, 512, "SELECT Password FROM Passwords WHERE User_Id='%s'", User_Id);
    mysql_query(conn, statement);
    result = mysql_store_result(conn);
    num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result)))
    {
        snprintf(s, 255,  "%s ", row[i] ? row[i] : "NULL");
    }
    mysql_close(conn);
    strncpy (s1,s,strlen(s)-1);
    if(strncmp (testPassword,s,strlen(s)-1) == 0)
    {
        return 1;
    }
    else{
        return 0;	  
    }
}

main()
{
    //insert("AlexM99", "Mack", "Alexander", "AM@gmail.com", 99);
    //update("AlexM99", 2);
    //char s[3];
    //s = selectScore("AlexM99");??
    //insertPassword("AlexM99", "FglO9099");
    int flag = comparePassword("AlexM99", "FglO90");
    printf("flag: %d", flag);
}
