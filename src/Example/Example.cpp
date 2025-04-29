#include <iostream>
#include <MemoryPackets/FPacket.h>

int main(int argc, char** argv){
    printf("Example Hello World\n");
    FPacket packet;
    packet.WriteUInt32(15);
    packet.WriteInt16(-1235);
    packet.SetReadPos(0);
    uint32_t t1 = packet.ReadUInt32();
    int16_t t2 = packet.ReadInt16();
}