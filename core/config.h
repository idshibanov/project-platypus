// Project Platypus
// config.h - getting server configuration from a file

#ifndef PLA_CONFIG_H
#define PLA_CONFIG_H

#include <string>
#include <map>

class ServerConfig {
    std::map<std::string, std::string> _config;
public:
    ServerConfig();
    ~ServerConfig();
    std::string& GetSetting(std::string key);
    std::string& GetSetting(const char* key);
};

#endif /* PLA_CONFIG_H */
