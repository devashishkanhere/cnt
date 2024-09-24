#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Winsock library

int main() {
    WSADATA wsa;
    char *ip = "127.0.0.1";
    int port = 8080;

    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_size;
    char buffer[1024];
    int n;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        exit(1);
    }

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("[-]Socket creation error ");
        WSACleanup();
        exit(1);
    }
    printf("[+] TCP Server Socket created successfully\n");

    // Setup address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Bind the socket
    n = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-]Bind error ");
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
    printf("[+] Bind to the port number: %d\n", port);

    // Listen for connections
    listen(server_socket, 5);
    printf("[+] Listening....\n");

    // Accept incoming connections
    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket == INVALID_SOCKET) {
            perror("[-]Accept error ");
            closesocket(server_socket);
            WSACleanup();
            exit(1);
        }
        printf("[+] Client connected\n");

        // Receive message from client
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        // Send response to client
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "Hello, message received. This is Server.");
        send(client_socket, buffer, strlen(buffer), 0);

        // Close client connection
        closesocket(client_socket);
        printf("[+] Client disconnected\n\n");
    }

    // Clean up Winsock
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
