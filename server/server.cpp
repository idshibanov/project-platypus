// Project Platypus
// server.cpp - implements GameServer class

#include "../core/config.h"
#include "../core/defines.h"
#include "../game/game.h"
#include "server.h"
#include "database.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

using namespace std;

GameServer::GameServer(int port)
{
    _port = port;
    _connected = false;

    FD_ZERO(&_readfds);
    FD_ZERO(&_testfds);
    
    ServerConfig conf;
    string db_host(conf.GetSetting("db_host"));
    string db_user(conf.GetSetting("db_user"));
    string db_pass(conf.GetSetting("db_pass"));

    _game = new GameInstance(this, 15, 78);
    _db = new DatabaseServer(db_host, db_user, db_pass);
    _client_sock = new ServerSocketArray(this, _game, _db);
}

GameServer::~GameServer()
{
    delete _game;
    delete _client_sock;
    delete _db;
}

bool GameServer::start_service()
{
    sockaddr_in serv_addr;
    int serv_addr_len;
    int reuse = 1;
    bool retval;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(_port);
    serv_addr_len = sizeof(serv_addr);

    // TODO: replace perror() with proper log function

    if((_serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket call failed");
        return false;
    }
    if (setsockopt(_serv_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
        perror("setsockopt call failed");
        return false;
    }
    if(bind(_serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind call failed");
        return false;
    }
    if(listen(_serv_fd, 5) == -1) {
        perror("listen call failed");
        return false;
    }

    // INIT: fd sets

    FD_ZERO(&_readfds);
    FD_ZERO(&_testfds);
    FD_SET(_serv_fd, &_readfds);
    FD_SET(STDIN_FILENO, &_readfds);
    _min_client_fd  = _max_client_fd = _serv_fd + 1;

    _connected = true;

    retval = run_select();

    return retval;
}


void GameServer::stop_service()
{
    int fd_cur;

    for (fd_cur = _min_client_fd; fd_cur < _max_client_fd; fd_cur++) {
        if (FD_ISSET(fd_cur, &_readfds)) {
            _client_sock->GetClient(fd_cur)->SendAck(PACKET_SERVER_SHUTDOWN);
            close(fd_cur);
        }
    }

    _connected = false;
    FD_ZERO(&_readfds);
    FD_ZERO(&_testfds);

    // clear our array
    _client_sock->ClearList();
}

bool GameServer::run_select()
{
    // MUST return false if error happens
    // WILL return true if server gets offline properly

    int client_fd, fd_cur, nread;

    while(_connected) {
        _testfds = _readfds;

        // remember to add 1 to fd_max
        // select ( highest_fd_number+1, &read_fd_set, &write_fd_set, &except_fd_set, &timeval)
        select( _max_client_fd, &_testfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);

        for(fd_cur = 0; fd_cur < _max_client_fd; fd_cur++) {
            if (FD_ISSET(fd_cur, &_testfds)) {
                if (fd_cur == _serv_fd) {
                    // EVENT: something on server socket

                    accept_client();

                } else if (fd_cur == STDIN_FILENO) {
                    // EVENT: server got keyboard input

                    parse_cmd();

                } else {
                    // EVENT: something on client socket

                    // check if client is alive
                    ioctl(fd_cur, FIONREAD, &nread);

                    if (nread == 0) {
                        // EVENT: got junk, kill client
                        kill_client(fd_cur);

                    } else {
                        // get client message and process it
                        if (!_client_sock->GetClient(fd_cur)->RecvPacket()) {
                            // EVENT: dropped some data
                        }
                    }
                }

            } // end of if(fd_isset) condition

        } // end of for(fd_set iteration) loop

    } // end of while(1) loop

    return false;
}

bool GameServer::accept_client()
{
    int client_fd;
    bool retval = false;

    if((client_fd = accept(_serv_fd, (struct sockaddr *) 0, (socklen_t *)0)) == -1) {
        // EVENT: can not accept connection

        // TODO: replace with proper log function
        perror("accept call failed");

    } else if (_client_sock->AddClient(client_fd) == false) {
        // EVENT: failed to add a client, server is full

        close(client_fd);

        // TODO: replace with proper log function
        printf("server is full\n");

    } else {
        // EVENT: connection is accepted and client is added to client_sock list

        FD_SET(client_fd, &_readfds);
        if ( client_fd + 1 > _max_client_fd )
            _max_client_fd = client_fd + 1;

        _game->AddPlayer(client_fd);
        broadcast_movement(client_fd);

        // TODO: replace with proper log function
        printf("adding client on fd %d\n", client_fd);

        retval = true;
    }

    return retval;
}

bool GameServer::kick_client(int fd)
{
    // false if that client is not in our list
    bool retval = false;

    if (FD_ISSET(fd, &_readfds)) {
        _client_sock->GetClient(fd)->SendAck(PACKET_SERVER_KICK);
        retval = kill_client(fd);
    }

    return retval;
}

bool GameServer::kill_client(int fd)
{
    // false if that client is not in our list
    bool retval = false;

    if (FD_ISSET(fd, &_readfds)) {
        _game->RemovePlayer(fd);
        _client_sock->RemoveClient(fd);
        close(fd);
        FD_CLR(fd, &_readfds);
        retval = true;
    }

    return retval;
}

bool GameServer::parse_cmd()
{
    // TODO: replace/improve fgets
    char kb_msg[MAX_KB_INPUT+1];
    int fd_cur;

    fgets(kb_msg, MAX_KB_INPUT, stdin);

    if (strcmp(kb_msg, "quit\n") == 0) {
        // EVENT: shutdown command

        stop_service();

    } else {
        // EVENT: its not a command
        //return false;

        // TODO: add proper command
        broadcast(kb_msg);
    }

    return true;
}

void GameServer::broadcast(const char* str, int fd)
{
    for (int fd_cur = _min_client_fd; fd_cur < _max_client_fd; fd_cur++) {
        // default fd is -1
        // printf("broadcasting to %d, min: %d, max: %d\n", fd_cur, _min_client_fd, _max_client_fd);

        if (FD_ISSET(fd_cur, &_readfds)) {
            if (fd_cur != fd)
                _client_sock->GetClient(fd_cur)->SendChatMsg(str);
        }
    }
}

void GameServer::broadcast_movement(int fd)
{
    MapCoords mvm = _game->GetPlayer(fd);
    for (int fd_cur = _min_client_fd; fd_cur < _max_client_fd; fd_cur++) {
        if (FD_ISSET(fd_cur, &_readfds)) {
            if (fd_cur != fd) {
                _client_sock->GetClient(fd_cur)->SendCharData(fd, mvm);
                //printf("Broadcast %d %d, lvl %d\n", mvm._row, mvm._col, mvm._lvl);
            }
        }
    }
}

void GameServer::log (string message)
{
    if (message.empty())
        return;

    printf("%s\n", message.c_str());
}

