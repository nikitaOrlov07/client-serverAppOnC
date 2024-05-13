#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// cheking it`s ipv4 or ipv6 address
int ipv6Oripv4(const char *ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &(sa.sin_addr)) != 0;
}

int main(int argc, char *argv[]) {
    // user must set 3 arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server ip> <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1]; // the first passed argument is server ip
    char *message_to_server = argv[2]; // second argument is a message
    int server_port = 5000;  // static variable

    int socket_cl = 0; // declare socket
    char buff[BUFFER_SIZE] = {0};// all array elements are 0

    if (ipv6Oripv4(server_ip)) {
        //if user set ipv4 address
        struct sockaddr_in sock_addr;
        // Set up the  address structure
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(server_port);

        if ((socket_cl = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // create socket and check for errors
            perror("Error: socket creation");
            exit(EXIT_FAILURE);
        }
        // convert ipv4 address to binary form
        if (inet_pton(AF_INET, server_ip, &sock_addr.sin_addr) <= 0) {
            perror("Address Error");
            exit(EXIT_FAILURE);
        }
        // connection error check
        if (connect(socket_cl, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // if user set ipv6 address
        struct sockaddr_in6 serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin6_family = AF_INET6;
        serv_addr.sin6_port = htons(server_port);

        if ((socket_cl = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
            perror("Error: socket creation");
            exit(EXIT_FAILURE);
        }
        // Convert IPv6 address to binary form
        if (inet_pton(AF_INET6, server_ip, &serv_addr.sin6_addr) <= 0) {
            perror("Address Error");
            exit(EXIT_FAILURE);
        }

        if (connect(socket_cl, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }
    }

    // Sending message to the server
    send(socket_cl, message_to_server , strlen(message_to_server ), 0);
    printf("Message sent to server: %s\n", message_to_server );

    // read uppercase message from the server(response from server)
    read(socket_cl, buff, BUFFER_SIZE);
    printf("Server response: %s\n", buff);

    close(socket_cl); // close socket

    return 0; // program end
}
