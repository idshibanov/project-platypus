// Project Platypus
// database.h - holds DatabaseServer class definition

#ifndef PLA_DATABASE_H
#define PLA_DATABASE_H

#include <my_global.h>
#undef min
#undef max
#include <mysql.h>
#include <string>

class DatabaseServer {
    MYSQL* _conn;
    char* _statement;
    
    public:
    DatabaseServer(std::string host, std::string user, std::string password);
    ~DatabaseServer();
    bool compare(std::string user_id, std::string password);
    void update(std::string user_id, int score);
    void insert(std::string user_id, std::string lastname, 
                std::string firstname, std::string address);
};

#endif
