// Project Platypus
// database.h - holds DatabaseServer class definition

#ifndef PLA_DATABASE_H
#define PLA_DATABASE_H

#include <my_global.h>
#undef min
#undef max
#include <mysql.h>
#include <string>
using namespace std;

class DatabaseServer {
    MYSQL* _conn;
    char* _statement;
    
    public:
    DatabaseServer(string host, string user, string password);
    ~DatabaseServer();
    bool compare(string user_id, string password);
    void update(string user_id, int score);
    void insert(string user_id, string lastname, string firstname, string address);
};

#endif
