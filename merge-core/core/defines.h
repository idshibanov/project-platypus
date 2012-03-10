// Project Platypus
// defines.h - global define statements

#ifndef PLA_DEFINES_H
#define PLA_DEFINES_H

// uncomment to exit debug mode - disables all assert() and prn_debug functions
//#define NDEBUG
#include <assert.h>

#define MAX_CLIENTS 3
#define MIN_CLIENT_SOCKFD 4
#define SERV_IP "127.0.0.1"
#define SERV_PORT 8030
#define MAX_PACKET_SIZE 1000
#define MAX_KB_INPUT 100

#endif /* PLA_DEFINES_H */
