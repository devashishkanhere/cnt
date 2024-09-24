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

    SOCKET client_socket;
    struct sockaddr_in addr;
    char buffer[1024];
    int n;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        exit(1);
    }

    // Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        perror("[-]Socket creation error ");
        WSACleanup();
        exit(1);
    }
    printf("[+] TCP Client Socket created successfully\n");

    // Setup address structure
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // Convert port to network byte order
    addr.sin_addr.s_addr = inet_addr(ip);

    // Connecting to the server
    if (connect(client_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[-]Connection failed ");
        closesocket(client_socket);
        WSACleanup();
        exit(1);
    }
    printf("[+] Connected to the server\n");

    // Send message to server
    memset(buffer, 0, sizeof(buffer)); // Clear the buffer
    strcpy(buffer, "Hello, This is Client.");
    printf("Client: %s\n", buffer);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive message from server
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    // Close the connection
    closesocket(client_socket);
    WSACleanup();
    printf("Disconnected from the server.\n");

    return 0;
}
