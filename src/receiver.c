#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <pthread.h>
#include <errno.h>

// SENDER = CLIENT
// RECEIVER = SERVER

void rrecv(unsigned short int myUDPport, 
            char* destinationFile, 
            unsigned long long int writeRate) {
    
    //code from https://www.educative.io/answers/how-to-implement-udp-sockets-in-c
    char* server_message[2000], client_message[2000];
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    //create socket and save descriptor
    int socket_desc = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return;
    }
    printf("Socket created successfully\n");


    // initialize server address
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myUDPport); 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind socket
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return;
    }
    printf("Done with binding\n");

    // recieve from client address
    int client_struct_length = sizeof(client_addr);
    printf("Listening for incoming messages...\n\n");
    // Receive client's message:
    if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
         (struct sockaddr*)&client_addr, &client_struct_length) < 0){
        printf("Couldn't receive\n");
        return;
    }
    printf("Received message from IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    printf("Sender Message: %s\n", client_message);
    // Respond to client:
    //    strcpy(server_message, client_message);
    char return_message[] = "ACK";
    if (sendto(socket_desc, return_message, strlen(return_message), 0,
         (struct sockaddr*)&client_addr, client_struct_length) < 0){
        printf("Can't send\n");
        return;
    }


}

int main(int argc, char** argv) {
    // This is a skeleton of a main function.
    // You should implement this function more completely
    // so that one can invoke the file transfer from the
    // command line.

    unsigned short int udpPort;

    if (argc != 3) {
        fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
        exit(1);
    }

    udpPort = (unsigned short int) atoi(argv[1]);
    char* filename = argv[2];
    
    rrecv(udpPort,filename,1);
    //close socket
    //close(socket_desc);

}