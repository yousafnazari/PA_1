/** @file packet_struct.h
 * @brief Structure for packets to include data and sequence number
 * 
 * 
 *
 *
 * @author Jayden Sahl (jaydensahl)
 * @author Yousaf Nazari (yousafnazari)
 *
 * @bug No known Bugs.
 *
 *
 */


#ifndef PACKET_STRUCT
#define PACKET_STRUCT

#include <stdint.h>

struct packetUDP {
    uint32_t source_port;
    uint32_t destination_port;
    uint32_t sequence_number;
    char payload[512];
};



#endif