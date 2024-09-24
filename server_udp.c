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
    struct sockaddr_in server_addr, client_addr;
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

    memset(&server_addr, '\0', sizeof(server_addr));  // Replacing bzero with memset
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Bind the socket
    n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-] Bind error");
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }

    // Receive data from the client
    memset(buffer, 0, 1024);  // Replacing bzero with memset
    addr_size = sizeof(client_addr);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_size);
    printf("[+] Data received from client: %s\n", buffer);

    // Send data back to client
    memset(buffer, 0, 1024);  // Replacing bzero with memset
    strcpy(buffer, "Welcome to Server. This is UDP Server.");
    sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("[+] Data sent to client: %s\n", buffer);

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
