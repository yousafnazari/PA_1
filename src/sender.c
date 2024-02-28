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

void rsend(char* hostname, 
            unsigned short int hostUDPport, 
            char* filename, 
            unsigned long long int bytesToTransfer) 
{
    // code from https://www.educative.io/answers/how-to-implement-udp-sockets-in-c
    char server_message[2000], client_message[2000];
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    struct sockaddr_in server_addr;

    //create socket
    int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0){
        printf("Error while creating sender socket\n");
        return;
    }   
    printf("Sender socket created successfully\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(hostUDPport);
    server_addr.sin_addr.s_addr = inet_addr(hostname);
    int server_struct_length = sizeof(server_addr);

    // message to send
    //char message[] = "Hello, receiver!";
    // or read from a file
    FILE *file;
    char *message;
    long filelen;
    printf("%s\n", filename);
    file = fopen(filename, "rb");  // Open the file in binary mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fseek(file, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(file);             // Get the current byte offset in the file
    rewind(file);                      // Jump back to the beginning of the file

    message = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(message, filelen, 1, file); // Read in the entire file
    fclose(file); // Close the file
    printf("%s\n",message);
    
    // Send the message to server:
    if(sendto(socket_desc, message, strlen(message), 0,
         (struct sockaddr*)&server_addr, server_struct_length) < 0){
        printf("Unable to send message\n");
        return;
    }
    printf("message sent\n");

    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
         (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        printf("Error while receiving server's msg\n");
        return;
    }
    printf("message received: %s\n", server_message);




}

int main(int argc, char** argv) {
    // This is a skeleton of a main function.
    // You should implement this function more completely
    // so that one can invoke the file transfer from the
    // command line.
    int hostUDPport;
    unsigned long long int bytesToTransfer;
    char* hostname = NULL;
    char* filename = NULL;

    if (argc != 5) {
        fprintf(stderr, "usage: %s receiver_hostname receiver_port filename_to_xfer bytes_to_xfer\n\n", argv[0]);
        exit(1);
    }
    hostUDPport = (unsigned short int) atoi(argv[2]);
    hostname = argv[1];
    bytesToTransfer = atoll(argv[4]);
    filename = argv[3];

    rsend(hostname,hostUDPport,filename,bytesToTransfer);

    return (EXIT_SUCCESS);
}