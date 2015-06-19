#pragma once

using ByteString	= std::basic_string < uint8_t > ;

class HedgeLobby : public PersistentEntity
{
    
public:
    
    uint64_t SessionID;
    std::string HostAddress; // Cap at 255.
    uint32_t MaxPlayers;
    uint32_t PlayerCount;
    uint32_t Gametype;
    ByteString LobbyBlobData;
    
    HedgeLobby();
    //HedgeClient(uint16_t appId, uint16_t seqId, EventType evType, uint32_t tmstmp, uint8_t len, void *buffer);
    HedgeLobby(ByteBuffer *InBuffer);
    ~HedgeLobby(void);
    bool Serialize(ByteBuffer *OutBuffer);
    //static bool Deserialize(ByteBuffer *InBuffer);
    
    //persistence stuff
    bool Save();
    bool Update();
    bool Remove();
};