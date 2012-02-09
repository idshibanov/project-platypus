// Project Platypus
// command.h - console commands from the server side

#ifndef PLA_COMMAND_H
#define PLA_COMMAND_H

void msg_local(int, const char*);
void msg_global(const char*);
bool kill_client(int);
void serv_shutdown();
unsigned int who (const char*);
const char* who (unsigned int);

#endif /* PLA_COMMAND_H */
