#include "StdInclude.h"

std::string quotesql(const std::string& s);

HedgeLobby::HedgeLobby(ByteBuffer *InBuffer){
    
    
    /*     uint64_t bdSessionID;
     std::string HostAddress; // Cap at 255.
     uint32_t MaxPlayers;
     uint32_t PlayerCount;
     uint32_t Gametype;
     ByteString LobbyBlobData;*/
    
    
  /*  InBuffer->ReadUInt64(&ClientID);
    InBuffer->ReadUInt32(&LastSeen);
    InBuffer->ReadUInt16(&LastPacketSequenceID);
    InBuffer->ReadUInt64(&SessionID);
    
    //uint8_t *nameLength = (uint8_t *)malloc(sizeof(uint8_t));
    //InBuffer->ReadUInt8(nameLength);
    
    InBuffer->ReadString(username); */
}

bool HedgeLobby::Serialize(ByteBuffer *OutBuffer){
    //we don't send this for now..
    return true;
}

bool HedgeLobby::Save(){
    //we don't persist...for now
    return true;
}

bool HedgeLobby::Update(){
    //we don't persist...for now
    return true;
}

bool HedgeLobby::Remove(){
    return true;
}

