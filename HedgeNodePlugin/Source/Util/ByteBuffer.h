/*
	This project is released under the GPL 2.0 license.
	Some parts are based on research by Bas Timmer and the OpenSteamworks project.
	Please do no evil.

	Initial author: (https://github.com/)Convery
	Started: 2014-11-06
	Notes:
		Based off DemonWares bdBytebuffer class.
		Fairly useful in any environment.
*/

#pragma once

class ByteBuffer
{
	std::basic_string<uint8_t> Buffer;
	uint32_t Position; // Next read/write will be Buffer[Position].

public:
	// Constructors taking info for the string.
	ByteBuffer();
	ByteBuffer(uint8_t *Data, uint32_t Length);
	ByteBuffer(std::basic_string<uint8_t> Data);
	ByteBuffer(std::string Data);
	~ByteBuffer(void);

	// Utility methods.
	uint32_t CurrentPosition(); // Returns Position.
	uint32_t Length(); // Length of the current buffer.
	uint8_t *GetBuffer(); // Return a pointer to the buffer.
	int32_t Find(uint8_t *Source, uint32_t Length); // Find first occurance of Source, -1 on failure.
	bool SetPosition(uint32_t Pos); // Sets the position.
	void Rewind(); // Sets the position to 0.

	// Base functions.
	bool Read(uint32_t Count, void *Output = nullptr);
	bool Write(uint32_t Count, void *Input = nullptr);
	bool ReadDataType(uint8_t Type);
	bool WriteDataType(uint8_t Type);

#pragma region BytebufferMethodsRead
	bool ReadBoolean(bool *Output);

	bool ReadInt8(int8_t *Output);
	bool ReadUInt8(uint8_t *Output);

	bool ReadInt16(int16_t *Output);
	bool ReadUInt16(uint16_t *Output);

	bool ReadInt32(int32_t *Output);
	bool ReadUInt32(uint32_t *Output);

	bool ReadInt64(int64_t *Output);
	bool ReadUInt64(uint64_t *Output);

	bool ReadFloat(float *Output);
	bool ReadString(std::string &Output);
	bool ReadBlob(std::string &Output);
	bool ReadBlob(std::basic_string<uint8_t> &Output);

	bool ReadArray(uint8_t Type, uint32_t *Count, uint32_t *ElementSize, void *Output);
#pragma endregion

#pragma region BytebufferMethodsWrite
	bool WriteBoolean(bool Input);

	bool WriteInt8(int8_t Input);
	bool WriteUInt8(uint8_t Input);

	bool WriteInt16(int16_t Input);
	bool WriteUInt16(uint16_t Input);

	bool WriteInt32(int32_t Input);
	bool WriteUInt32(uint32_t Input);

	bool WriteInt64(int64_t Input);
	bool WriteUInt64(uint64_t Input);

	bool WriteFloat(float Input);
	bool WriteString(std::string &Input);
	bool WriteBlob(std::string &Input);
	bool WriteBlob(std::basic_string<uint8_t> &Input);

	bool WriteArray(uint8_t Type, uint32_t Count, uint32_t ElementSize, void *Input);
#pragma endregion
};