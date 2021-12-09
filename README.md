# TCP Server & Multiple-Clients Messenger

## Overview

This is a C program which focuses on creating a messenger on same machine. We create a server which accepts multiple clients at a time based of FD function by using UNIX Socket Programming. The client program connects to the server program via socket and tcp protocol. 

<p align="center">
<img src="/images/tcp1.png" "TCP Protocol">
</p>

## Pre-Requisites

### Header Files

The following header files are needed for full functionality of the program.
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

### Ncurses Library

We need to install the following packages first for running ncurses:
* sudo apt install libncurses5-dev libncursesw5-dev

The ncurses library is a special interface which helps to do special operations on a separate shell window. This can provide a variety of special functions like changing screen and keyboard functionality to a new default. This encases library functions like getchar() and kbhit() which will be useful for the client_tcp.c to run. kbhit() and getchar() is used to record key press or take invisible input. 

To compile a program which uses ncurses library we compile as :
gcc clienttcp client_tcp.c -lncurses

### To run code

* gcc -o mserver mserver.c
./mserver

* gcc -o clienttcp client_tcp.c -lncurses
./clienttcp

We can run as many clients as we want to connect with the server and message can be sent from any client to another.

## Main Code

### SERVER

TCP server uses the following basic socket function to run:
* using create(), Create TCP socket.
* using bind(), Bind the socket to server address.
* using listen(), put the server socket in a passive mode, where it waits for the client to approach the server to make a connection
* using accept(), At this point, connection is established between client and server, and they are ready to transfer data.

We create a mutiple socket function to receive multiple client_socket in maser_socket. For this we use the FD functions. 

#### Steps for Server Functionality 

* First we use fd_sets to create socket descriptors, after which we create the master socket.
* We then we use setsockopt to ensure availablity of mutiple connections by REUSEADDR and REUSEPORT
* Bind the Socket from the address struct variables to localhost PORT:8888
* We let the server to listen for incoming connecions.
* Check if a new Client has been setup and we count the activ client while discarding the clients who left.
* We create a list of the clients and the name they have assigned themselves and giving each of these client a token number.
* Keep checking if any of the active clients want to send any message to any other active client.
* If yes save the client the message is directed to and also save the message in a message buffer.
* If no send a message "nothing" so that the client is not stuck at read() funcion.
* We then send this message and the name of the sending client to the client the message is directed to.
* A list of message buffer for each client socket, a list of clients and a list of pointers is mainted continuously.

### CLIENT

TCP Client runs these basic socket functions:
* Create a socket
* Connect socket to the server.

####Steps for Client Functionality

* we do not require multiple functionality for this
* Read the assigned Client number to unique identification of the client.
* Then we send the name of the client we assign ourselves.
* Press any key to send a message to any of the other clients and always listen for incoming messages from server
* If some message is received we get the name of the client that is sending and the message itself.
* If we wawnt to send some message we just press any key, type the client we want to send it to then we type the message

## Future Updates

I intended to make a group creation and encryption feature in this program as well as provide a persistent messenger without having to type the name of the client for long lasting conversations.
