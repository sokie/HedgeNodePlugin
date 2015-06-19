#pragma once

using ByteString = std::basic_string < uint8_t > ;

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
    HedgeLobby(ByteBuffer *InBuffer);

    bool Serialize(ByteBuffer *OutBuffer);
    //static bool Deserialize(ByteBuffer *InBuffer);
    
    //persistence stuff
    bool Save();
    bool Update();
    bool Remove();
};