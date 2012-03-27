// Project Platypus
// config.cpp - implements ServerConfig class

#include "config.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

ServerConfig::ServerConfig()
{
    ifstream ifs ( "config" , ifstream::in );
    char buf[256];
    string key, val, line;
    size_t found;
    
    if (!ifs.good()) {
        perror("Can't find config file");
        exit(1);
    }

    while (ifs.good()) {
        ifs.getline(buf, 256);
        line.assign(buf);
        found=line.find("=");
        if (found != string::npos) {
            key = line.substr(0, found);
            val = line.substr(found+1);
            //cout << key << val << endl;
            _config.insert( pair<string,string>(key,val) );
        }
    }
}

ServerConfig::~ServerConfig()
{

}

string& ServerConfig::GetSetting(string key)
{
    return _config.find(key)->second;
}


string& ServerConfig::GetSetting(const char* key)
{
    return GetSetting(string(key));
}
