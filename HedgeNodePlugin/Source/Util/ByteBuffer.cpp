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

#include "..\STDInclude.h"

#pragma region Enum
enum class DataTypes : uint8_t
{
	BD_BB_NO_TYPE = 0,
	BD_BB_BOOL_TYPE = 1,
	BD_BB_SIGNED_CHAR8_TYPE = 2,
	BD_BB_UNSIGNED_CHAR8_TYPE = 3,
	BD_BB_WCHAR16_TYPE = 4,
	BD_BB_SIGNED_INTEGER16_TYPE = 5,
	BD_BB_UNSIGNED_INTEGER16_TYPE = 6,
	BD_BB_SIGNED_INTEGER32_TYPE = 7,
	BD_BB_UNSIGNED_INTEGER32_TYPE = 8,
	BD_BB_SIGNED_INTEGER64_TYPE = 9,
	BD_BB_UNSIGNED_INTEGER64_TYPE = 10,
	BD_BB_RANGED_SIGNED_INTEGER32_TYPE = 11,
	BD_BB_RANGED_UNSIGNED_INTEGER32_TYPE = 12,
	BD_BB_FLOAT32_TYPE = 13,
	BD_BB_FLOAT64_TYPE = 14,
	BD_BB_RANGED_FLOAT32_TYPE = 15,
	BD_BB_SIGNED_CHAR8_STRING_TYPE = 16,
	BD_BB_UNSIGNED_CHAR8_STRING_TYPE = 17,
	BD_BB_MBSTRING_TYPE = 18,
	BD_BB_BLOB_TYPE = 19,
	BD_BB_NAN_TYPE = 20,
	BD_BB_FULL_TYPE = 21,
	BD_BB_MAX_TYPE = 32
};
#pragma endregion

#pragma region Constructors

// Constructors taking info for the string.
ByteBuffer::ByteBuffer()
{
	Buffer.clear();
	Position = 0;
}
ByteBuffer::ByteBuffer(uint8_t *Data, uint32_t Length)
{
	Buffer.clear();
	Buffer.append(Data, Length);
	Position = 0;
}
ByteBuffer::ByteBuffer(std::basic_string<uint8_t> Data)
{
	Buffer.clear();
	Buffer.append(Data.data(), Data.size());
	Position = 0;
}
ByteBuffer::ByteBuffer(std::string Data)
{
	Buffer.clear();
	Buffer.append((uint8_t *)Data.c_str(), Data.length());
	Position = 0;
}
ByteBuffer::~ByteBuffer(void)
{
	Buffer.clear();
}

#pragma endregion

#pragma region Utility

// Utility methods.
uint32_t ByteBuffer::CurrentPosition()
{ 
	return Position;
}
uint32_t ByteBuffer::Length()
{ 
	return Buffer.length();
}
uint8_t *ByteBuffer::GetBuffer()
{
	return (uint8_t *)Buffer.data();
}
int32_t ByteBuffer::Find(uint8_t *Source, uint32_t Length)
{ 
	for(uint32_t i = 0; i < Buffer.length() - Length; i++)
		if(memcmp((void *)(Buffer.data() + i), Source, Length) == 0)
			return i;
	return -1;
}
bool ByteBuffer::SetPosition(uint32_t Pos)
{ 
	if(Pos > Buffer.length())
		return false;

	Position = Pos;
	return true;
}
void ByteBuffer::Rewind()
{ 
	Position = 0;
}

#pragma endregion

#pragma region Base

// Base functions.
bool ByteBuffer::Read(uint32_t Count, void *Output)
{ 
	if(Position + Count > Buffer.length())
		return false;

	if(Output != nullptr)
		memcpy(Output, (void *)(Buffer.data() + Position), Count);

	Position += Count;
	return true;
}
bool ByteBuffer::Write(uint32_t Count, void *Input)
{ 
	if (Input != nullptr)
	{
		if (Position == Buffer.length())
		{
			Buffer.append((uint8_t *)Input, Count);
		}
		else
		{
			if (Position < Buffer.length())
			{
				if (Position + Count < Buffer.length())
				{
					for (uint32_t i = 0; i < Count; i++)
					{
						Buffer[Position] = ((uint8_t *)Input)[i];
						++Position;
					}
					return true;
				}
				else
				{
					uint32_t FreeBytes = Buffer.length() - Position;

					Write(FreeBytes, Input);
					Buffer.append((uint8_t *)Input + FreeBytes, Count - FreeBytes);

					Position += Count - FreeBytes;
					return true;
				}
			}
			else
			{
				DBGPrint("%s - Position > Buffer.length", __FUNCTION__);
			}
		}
	}

	Position += Count;
	return true;
}
bool ByteBuffer::ReadDataType(uint8_t Type)
{ 
	uint8_t Storedtype = 0;

	Read(1, &Storedtype);
	if(Storedtype != Type)
	{
		Position--;
		return false;
	}

	return true;
}
bool ByteBuffer::WriteDataType(uint8_t Type)
{ 
	return Write(1, &Type);
}

#pragma endregion

#pragma region Read

// Read 
bool ByteBuffer::ReadBoolean(bool *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_BOOL_TYPE))
		return Read(1, Output);
	else
		return false;
}

bool ByteBuffer::ReadInt8(int8_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_SIGNED_CHAR8_TYPE))
		return Read(1, Output);
	else
		return false;
}
bool ByteBuffer::ReadUInt8(uint8_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_CHAR8_TYPE))
		return Read(1, Output);
	else
		return false;
}

bool ByteBuffer::ReadInt16(int16_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_SIGNED_INTEGER16_TYPE))
		return Read(2, Output);
	else
		return false;
}
bool ByteBuffer::ReadUInt16(uint16_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_INTEGER16_TYPE))
		return Read(2, Output);
	else
		return false;
}

bool ByteBuffer::ReadInt32(int32_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_SIGNED_INTEGER32_TYPE))
		return Read(4, Output);
	else
		return false;
}
bool ByteBuffer::ReadUInt32(uint32_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_INTEGER32_TYPE))
		return Read(4, Output);
	else
		return false;
}

bool ByteBuffer::ReadInt64(int64_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_SIGNED_INTEGER64_TYPE))
		return Read(8, Output);
	else
		return false;
}
bool ByteBuffer::ReadUInt64(uint64_t *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_INTEGER64_TYPE))
		return Read(8, Output);
	else
		return false;
}

bool ByteBuffer::ReadFloat(float *Output)
{
	if(ReadDataType((uint8_t)DataTypes::BD_BB_FLOAT32_TYPE))
		return Read(4, Output);
	else
		return false;
}
bool ByteBuffer::ReadString(std::string &Output)
{
	uint32_t Length = 0;

	if(!ReadDataType((uint8_t)DataTypes::BD_BB_SIGNED_CHAR8_STRING_TYPE))
		return false;

	Length = strlen((char *)(Buffer.data() + Position)) + 1;

	Output.append((char *)(Buffer.data() + Position), Length);
	Position += Length;
	return true;
}
bool ByteBuffer::ReadBlob(std::string &Output)
{
	uint32_t Length = 0;

	if(!ReadDataType((uint8_t)DataTypes::BD_BB_BLOB_TYPE) || !ReadUInt32(&Length))
		return false;

	Output.append((char *)(Buffer.data() + Position), Length);
	Position += Length;
	return true;
}
bool ByteBuffer::ReadBlob(std::basic_string<uint8_t> &Output)
{
	uint32_t Length = 0;

	if(!ReadDataType((uint8_t)DataTypes::BD_BB_BLOB_TYPE) || !ReadUInt32(&Length))
		return false;

	for(uint32_t i = Position; i < Position + Length; i++)
		Output.push_back(Buffer[i]);

	Position += Length;
	return true;
}

bool ByteBuffer::ReadArray(uint8_t Type, uint32_t *Count, uint32_t *ElementSize, void *Output)
{
	uint8_t StoredType = 0;
	uint32_t arraySize = 0;
	uint32_t arrayCount = 0;

	if(!Read(1, &StoredType))
		return false;

	if(StoredType != (Type + 100))
		return false;

	if(!ReadUInt32(&arraySize))
		return false;

	if(!Read(4, &arrayCount))
		return false;

	*Count = arrayCount;

	if(ElementSize != nullptr)
	{
		if(arrayCount > 0)
			*ElementSize = arraySize / arrayCount;
		else
			*ElementSize = 0;
	}

	return Read(arraySize, Output);
}

#pragma endregion

#pragma region Write

// Write
bool ByteBuffer::WriteBoolean(bool Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_BOOL_TYPE))
		return Write(1, &Input);
	else
		return false;
}

bool ByteBuffer::WriteInt8(int8_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_SIGNED_CHAR8_TYPE))
		return Write(1, &Input);
	else
		return false;
}
bool ByteBuffer::WriteUInt8(uint8_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_CHAR8_TYPE))
		return Write(1, &Input);
	else
		return false;
}

bool ByteBuffer::WriteInt16(int16_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_SIGNED_INTEGER16_TYPE))
		return Write(2, &Input);
	else
		return false;
}
bool ByteBuffer::WriteUInt16(uint16_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_INTEGER16_TYPE))
		return Write(2, &Input);
	else
		return false;
}

bool ByteBuffer::WriteInt32(int32_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_SIGNED_INTEGER32_TYPE))
		return Write(4, &Input);
	else
		return false;
}
bool ByteBuffer::WriteUInt32(uint32_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_INTEGER32_TYPE))
		return Write(4, &Input);
	else
		return false;
}

bool ByteBuffer::WriteInt64(int64_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_SIGNED_INTEGER64_TYPE))
		return Write(8, &Input);
	else
		return false;
}
bool ByteBuffer::WriteUInt64(uint64_t Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_UNSIGNED_INTEGER64_TYPE))
		return Write(8, &Input);
	else
		return false;
}

bool ByteBuffer::WriteFloat(float Input)
{
	if(WriteDataType((uint8_t)DataTypes::BD_BB_FLOAT32_TYPE))
		return Write(4, &Input);
	else
		return false;
}
bool ByteBuffer::WriteString(std::string &Input)
{
	if(!WriteDataType((uint8_t)DataTypes::BD_BB_SIGNED_CHAR8_STRING_TYPE))
		return false;
	else
		return Write(strlen(Input.c_str()) + 1, (void *)Input.c_str());
}
bool ByteBuffer::WriteBlob(std::string &Input)
{
	if(!WriteDataType((uint8_t)DataTypes::BD_BB_BLOB_TYPE) || !WriteUInt32(Input.length()))
		return false;
	else
		return Write(Input.length(), (void *)Input.data());
}
bool ByteBuffer::WriteBlob(std::basic_string<uint8_t> &Input)
{
	if(!WriteDataType((uint8_t)DataTypes::BD_BB_BLOB_TYPE) || !WriteUInt32(Input.size()))
		return false;
	else
		return Write(Input.size(), (void *)Input.data());
}

bool ByteBuffer::WriteArray(uint8_t Type, uint32_t Count, uint32_t ElementSize, void *Input)
{
	char StoredType = Type + 100;

	if(!Write(1, &StoredType))
		return false;

	if(!WriteUInt32(Count * ElementSize))
		return false;

	if(!WriteUInt32(Count))
		return false;

	return Write(Count * ElementSize, Input);
}

#pragma endregion