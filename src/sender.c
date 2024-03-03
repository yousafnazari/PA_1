/** @file sender.c
 * @brief Sends data over UDP. 
 * 
 * Sends packets of raw bytes from the given file to the destination provided.
 * Uses a loop to send the entire file. Sends one packet then waits for ACK before 
 * sending the nect packet until the entire file is sent.
 *
 *
 * @author Jayden Sahl (jaydensahl)
 * @author Yousaf Nazari (yousafnazari)
 *
 * @bug No known Bugs.
 *
 *
 */
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

#include "packet_struct.h"

// SENDER = CLIENT
// RECEIVER = SERVER

/** @brief sends file over UDP to given destination.
* @param hostname - host IP address
* @param hostUDPport - host port number
* @param filename - file to read from
* @param bytesToTransfer
*
* @retval void.
*/

void rsend(char* hostname, 
            unsigned short int hostUDPport, 
            char* filename, 
            unsigned long long int bytesToTransfer) 
{
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
    // read from a text file
    FILE *file;
    char *message;
    long filelen;
    file = fopen(filename, "rb");  // Open the file in binary mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fseek(file, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(file);             // Get the current byte offset in the file
    rewind(file);                      // Jump back to the beginning of the file

    struct packetUDP send_packet;
    struct packetUDP received_pack;
    send_packet.sequence_number = 0;
    int bytesRead = 0;
    int bytesPerPacket = sizeof(send_packet.payload);
    unsigned char buffer[bytesPerPacket];
    while ((bytesRead = fread(buffer,1,bytesPerPacket,file)) > 0)
    {
        printf("%d bytes read\n",bytesRead);
        memcpy(send_packet.payload,buffer,bytesPerPacket); // copy from buffer to payload
        // Send the message to server:
        if(sendto(socket_desc, &send_packet, sizeof(send_packet), 0,
            (struct sockaddr*)&server_addr, server_struct_length) < 0){
            printf("Unable to send message\n");
            perror("Error");
            return EXIT_FAILURE;
        }
        printf("%d - sent\n", send_packet.sequence_number);

        // Receive the server's response:
        if(recvfrom(socket_desc, &received_pack, sizeof(received_pack), 0,
            (struct sockaddr*)&server_addr, &server_struct_length) < 0){
            printf("Error while receiving server's msg\n");
            perror("Error");
            return EXIT_FAILURE;
        }
        printf("%d - %s\n", received_pack.sequence_number,received_pack.payload); // assumes payload is a string
        send_packet.sequence_number++;
        memset(buffer,0,sizeof(buffer));
        memset(send_packet.payload,0,sizeof(send_packet.payload));
    }
    
    //int bytesRead = fread(buffer, 1, bytesToTransfer, file); // Read in the entire file
    
    // end connection - FIN
    memcpy(send_packet.payload,"FIN",bytesPerPacket);
    sendto(socket_desc, &send_packet, sizeof(send_packet), 0,
            (struct sockaddr*)&server_addr, server_struct_length);

    fclose(file); // Close the file
    close(socket_desc); //close the socket

}
/** @brief Calls the send function to transfer data
 * 
 * 
 *
 * @return 0 value.
 *
 */


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