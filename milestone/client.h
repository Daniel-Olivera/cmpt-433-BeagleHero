#ifndef _CLIENT_H_
#define _CLIENT_H_

#define PORT 12345
#define SERVER_IP "192.168.1.81"
#define MAX_LEN 1024

void Client_start(void);
void Client_waitForShutdown(void);
void Client_stop(void);

#endif
