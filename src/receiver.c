/** @file receiver.c
 * @brief Receives data over UDP. 
 * 
 * Receives packets of raw bytes from the sender and writes them to the
 * destination provided. Uses a loop to read whats incoming until a FIN signal
 * is received. Responds to every received packet with ACK.
 *
 *
 * @author Jayden Sahl (jaydensahl)
 * @author Yousaf Nazari (yousafnazari)
 * 
 * @bug No known Bugs.
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
/** @brief  Receives packets over UDP and writes to given destination file
 * 
 * @param myUDPport - port number
 * @param destinationFile - file to write to 
 * @param writeRate - unused
 *
 * @return void.
 */

void rrecv(unsigned short int myUDPport, 
            char* destinationFile, 
            unsigned long long int writeRate) {

    struct packetUDP rec_pack;
    struct packetUDP ACK_pack;
    FILE* outFile = fopen(destinationFile,"wb"); 
    //create socket and save descriptor
    int socket_desc = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
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
    int bindv;
    if(bindv = bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port Bind value: %d\n",bindv);
        perror("Error");
        return EXIT_FAILURE;
    }
    printf("Done with binding\n");

    // recieve from client address
    int client_struct_length = sizeof(client_addr);
    printf("Listening for incoming messages...\n\n");
    int recVal = 0;
    int bytesWritten = 0;
    // Receive client's message:
    while (1)
    {
        recVal = recvfrom(socket_desc, &rec_pack, sizeof(rec_pack), 0,(struct sockaddr*)&client_addr, &client_struct_length);

        // check for end of transmission message
        if (strcmp(rec_pack.payload,"FIN") == 0)
        {
            break;
        }
        
        printf("Sender Seq #: %d\n", rec_pack.sequence_number);
        
        // Respond to client with ACK:
        ACK_pack.sequence_number = rec_pack.sequence_number;
        strcpy(ACK_pack.payload, "ACK");
        //char return_message[] = "ACK";
        if (sendto(socket_desc, &ACK_pack, sizeof(ACK_pack), 0,
             (struct sockaddr*)&client_addr, client_struct_length) < 0){
            printf("Can't send\n");
            return;
        }

        // write received message to file
        bytesWritten = fwrite(rec_pack.payload,1,sizeof(rec_pack.payload),outFile);
        printf("%d bytes written\n",bytesWritten);
        
    };
 
    fclose(outFile);
    close(socket_desc);
}
/** @brief Calls the rrecv function to transfer data
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

    unsigned short int udpPort;

    if (argc != 3) {
        fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
        exit(1);
    }

    udpPort = (unsigned short int) atoi(argv[1]);
    char* filename = argv[2];
    
    rrecv(udpPort,filename,1);

}