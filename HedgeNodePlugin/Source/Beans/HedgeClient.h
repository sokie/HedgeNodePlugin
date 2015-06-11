#pragma once

class HedgeClient : public PersistentEntity
{

public:

	//xuid for now
	uint32_t ClientID;
	hAddress addr;
	uint32_t LastSeen;
	uint16_t LastPacketSequenceID;
	uint32_t SessionID;
	bool isAuthenticated = false;
	bool isAnonymous = false;
	std::string username;

	HedgeClient();
	//HedgeClient(uint16_t appId, uint16_t seqId, EventType evType, uint32_t tmstmp, uint8_t len, void *buffer);
	HedgeClient(ByteBuffer *InBuffer);
	~HedgeClient(void);
	bool Serialize(ByteBuffer *OutBuffer);
	//static bool Deserialize(ByteBuffer *InBuffer);

	//persistence stuff
	bool Save();
	bool Update();
	bool Remove();
};