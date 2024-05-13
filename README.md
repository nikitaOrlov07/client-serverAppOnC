Server-Client Communication
This repository contains a server-client communication system implemented in C programming language. The system allows clients to connect to a server over IPv4 or IPv6 addresses and exchange messages. The server handles multiple client connections concurrently using threads.

Features:
* Supports both IPv4 and IPv6 addresses.
* Allows clients to connect to the server and send messages.
* The server processes messages received from clients, converts them to uppercase, and sends the response back to the clients.
* Handles multiple client connections concurrently using pthreads.
* Logs all incoming messages to a file named messages.txt.

Usage:
To run the server: ./server <server_ip>
To run the client: ./client <server_ip> <message>

Repozitory Structure:
* server.c: Contains the implementation of the server.
* client.c: Contains the implementation of the client.
* messages.txt: Stores all incoming messages received by the server.

