#include "..\..\StdInclude.h"

namespace Network
{
        
    NetworkPacket::NetworkPacket(){
        eventType = HNUnknownEvent;
        DataLength = 0;
        ApplicationID = 0;
        SequenceID = 0;
        DataBuffer = nullptr;
    }
    
    NetworkPacket::NetworkPacket(uint32_t appId, uint16_t seqId, EventType evType, uint32_t	tmstmp, uint8_t	len, void *buffer){
        ApplicationID = appId;
        SequenceID = seqId;
        eventType = evType;
        TimeStamp = tmstmp;
        DataLength = len;
        DataBuffer = buffer;
    }
    
    NetworkPacket::NetworkPacket(ByteBuffer *InBuffer){
        int16_t *eventType = (int16_t *)malloc(sizeof(int16_t));
        InBuffer->ReadInt16(eventType);
        //eventType = static_cast<EventType>(eventType);
        InBuffer->ReadUInt32(&ApplicationID);
        InBuffer->ReadUInt16(&SequenceID);
        InBuffer->ReadUInt32(&TimeStamp);
        InBuffer->ReadUInt8(&DataLength);
        int8_t *len = (int8_t *)malloc(sizeof(int8_t));
        InBuffer->ReadInt8(len);
        InBuffer->Read((uint32_t)*len, DataBuffer);
    }
    
    NetworkPacket::~NetworkPacket(void){
        
    }
    
    bool NetworkPacket::Serialize(ByteBuffer *OutBuffer){
        //EventType first so we can peak at packets
        OutBuffer->WriteInt16(eventType);
        OutBuffer->WriteUInt32(ApplicationID);
        OutBuffer->WriteUInt16(SequenceID);
        OutBuffer->WriteUInt32(TimeStamp);
        OutBuffer->WriteUInt8(DataLength);
        OutBuffer->Write(DataLength, DataBuffer);
        return true;
    }
}