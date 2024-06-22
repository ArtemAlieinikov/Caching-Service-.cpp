#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <iostream>

#include "functions.h"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in sock_addr = {};
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = ntohs(1234);
    sock_addr.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0

    int rv = bind(fd, (const sockaddr *)&sock_addr, sizeof(sock_addr));
    if (rv) {
        die("binding went wrong");
    }

    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listening went wrong");
    }

    while (true) {
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connectionfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);

        if (!connectionfd) {
            continue;
        }
    }
};

static void process_client(int* connectionfd) {
    char clientData[64];
    ssize_t n = read(*connectionfd, clientData, sizeof(clientData) - 1);

    if (n < 0) {
        std::cout << "reading error for "<< *connectionfd << "socket connection" << std::endl;
        return;
    }

    printf("client sent: ", clientData);
    char responsebuf[] = "world";
    write(*connectionfd, responsebuf, strlen(responsebuf));
}

void die(const char *message) {
    perror(message);
    exit(1);
}