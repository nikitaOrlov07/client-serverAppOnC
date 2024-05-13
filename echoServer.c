#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>

#define BUFF_SIZE 1024
#define MAX_CLIENTS 5

void *handle_client(void *arg) {
    int socket = *((int *)arg);
    char buffer[BUFF_SIZE] = {0};

    // Read from client
    read(socket, buffer, BUFF_SIZE);
    printf("Received message from client: %s\n", buffer);

    // Convert message to uppercase
    for (int i = 0; buffer[i] != '\0'; i++) {
        buffer[i] = toupper(buffer[i]);
    }

    // Send to client
    send(socket, buffer, strlen(buffer), 0);
    printf("Response sent to client: %s\n", buffer);

    // Write message to file
    FILE *file = fopen("messages.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        close(socket);
        pthread_exit(NULL);
    }
    fprintf(file, "%s\n", buffer);
    fclose(file);

    // Close the socket
    close(socket);

    // Exit the thread
    pthread_exit(NULL);
}

int main(int arguments, char *argv[]) {

   // user must set 2 parameters
    if (arguments != 2) {
        fprintf(stderr, "Usage: %s <server_ip> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1]; // take server ip from arguments
    int server_port = 5000;    // static variable

    int server_socket ;
    int new_socket; // for multiply
    struct sockaddr_in6 address;
    int addrlength = sizeof(address);
    pthread_t thread_id; //  for threads identity

    // Create socket
    if ((server_socket = socket(AF_INET6, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the address structure
    memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    inet_pton(AF_INET6, server_ip, &address.sin6_addr);
    address.sin6_port = htons(server_port);

    // Binding socket to the address
    if (bind(server_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Error: Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error: Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", server_port);

    // Accept incoming connections and handle them in separate threads - To allow multiple clients to connect to the server simultaneously.
    while (1) {
        if ((new_socket = accept(server_socket, (struct sockaddr *) &address, (socklen_t *) &addrlength)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *) &new_socket) < 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected\n");
    }
    // exit from the program with ctrl+C
}