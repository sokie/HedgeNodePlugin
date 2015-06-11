/*
	This project is released under the GPL 2.0 license.
	Please do no evil.

	Initial author: (https://github.com/)Convery
	Started: 2015-04-30
	Notes:
		Structs and classes used for the database.
*/

#pragma once

// Stores information about an event.
class Event
{
	uint8_t		EventType;
	uint32_t	TimeStamp;
	uint8_t		DataLength;
	std::string DataBuffer;

	virtual bool Serialize(char *Outdata, uint8_t Length);
	virtual bool Deserialize(char *Indata, uint8_t Length);
};
inline bool Event::Serialize(char *Outdata, uint8_t Length)
{
	if (Length < 6)
		return false;

	// Convert the data to big endian.
	uint32_t NetTimeStamp = htonl(this->TimeStamp);

	// Copy to the outdata buffer and increment the pointer.
	memcpy(Outdata, &this->EventType, 1);
	Outdata += 1;
	memcpy(Outdata, &NetTimeStamp, 4);
	Outdata += 4;
	memcpy(Outdata, &this->DataLength, 1);
	Outdata += 1;

	// Copy as much data as we can fit.
	for (uint8_t i = 0; i < min(Length - 6, this->DataBuffer.size()); i++)
	{
		memcpy(Outdata, &this->DataBuffer[i], 1);
		Outdata += 1;
	}

	return true;
}
inline bool Event::Deserialize(char *Indata, uint8_t Length)
{
	if (Length < 6)
		return false;

	// Copy from the packetstream and increment the pointer.
	memcpy(&this->EventType, Indata, 1);
	DataBuffer += 1;
	memcpy(&this->TimeStamp, Indata, 4);
	DataBuffer += 4;
	memcpy(&this->DataLength, Indata, 1);
	DataBuffer += 1;

	// Convert the data to host endian.
	this->TimeStamp = ntohl(this->TimeStamp);

	// Read as much data as we can fit.
	DataBuffer.clear();
	for (uint8_t i = 0; i < min(Length - 6, this->DataLength); i++)
	{
		this->DataBuffer.push_back(0);
		memcpy(&this->DataBuffer[i], Indata, 1);
		Indata += 1;
	}

	return true;
}

// Event storage file format.
struct EventBlock
{
	uint64_t CreatorID;
	uint32_t CreatedTimestamp;
	uint32_t LogID;
	std::vector<Event> Events;
	uint32_t EndToken;
};
