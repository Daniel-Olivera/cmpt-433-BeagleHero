#ifndef _SERVER_H
#define _SERVER_H

#define PORT 12345
#define CLIENT_IP "192.168.1.81"
#define MAX_LEN 1024

void Server_start(void);
void Server_waitForShutdown(void);
void Server_stop(void);

#endif