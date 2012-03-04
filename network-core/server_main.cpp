#include "server.h"

// code for custom interface
// server forking & server management
// goes here

// so we can start/shutdown server manually
// without losing any data + catch signals

int main()
{
   GameServer serv;
   serv.start_service();
   return 0;
}


