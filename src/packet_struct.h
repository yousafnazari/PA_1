#ifndef PACKET_STRUCT
#define PACKET_STRUCT

#include <stdint.h>

struct packetUDP {
    uint32_t sequence_number;
    char payload[512]; // Your actual data here
};



#endif