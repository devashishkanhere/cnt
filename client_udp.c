#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char **argv) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(1);
    }

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        WSACleanup();
        exit(0);
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);

    int sockfd;
    struct sockaddr_in addr;
    char buffer[1024];
    int addr_size;
    int n;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("[-] Socket error");
        WSACleanup();
        exit(1);
    }

    memset(&addr, '\0', sizeof(addr));  // Replacing bzero with memset
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    // Send message to the server
    memset(buffer, 0, 1024);  // Replacing bzero with memset
    strcpy(buffer, "Hello, This is Client.");
    sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
    printf("[+] Data sent to server successfully\n");

    // Receive response from the server
    memset(buffer, 0, 1024);  // Replacing bzero with memset
    addr_size = sizeof(addr);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&addr, &addr_size);
    printf("[+] Data received from server: %s\n", buffer);

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
