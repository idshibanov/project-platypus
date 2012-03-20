#include <my_global.h>
#include <mysql.h>
#include <string.h>

class DatabaseServer {
    MYSQL* _conn;
    char* _statement;
    
    public:
    DatabaseServer(std::string host, std::string user, std::string password);
    ~DatabaseServer();
    bool compare(string user_id, string password);
    void update(string user_id, int score);
    void insert(string user_id, string lastname, string firstname, string address);
}
