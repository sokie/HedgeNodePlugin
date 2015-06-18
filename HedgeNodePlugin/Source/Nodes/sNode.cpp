/*
	This project is licensed under the GNU GPL 2.0 license. Please respect that.

	Initial author: (https://github.com/)Sokie
	Started: 11-06-2015
	Notes
		This handles most of persistence and networking
		Mostly a glorified database
*/

#include "..\STDInclude.h"

namespace Nodes
{
	std::vector<HedgeClient> SNode::Clients;
	std::mutex SNode::ThreadSafe;

	DWORD _stdcall SNode::ServerNodeCommandReceiver(void  *lparam)
	{
		uint16_t port = 31337;
		Network::SocketManager::Create_UDP(&port, false);

		char *recvBuffer = (char *)malloc(sizeof(char) * 1024);
		hAddress sender = hAddress::hAddress();
		while (true){
			int len = Network::SocketManager::Receive_UDP(port, 1024, recvBuffer, &sender);
			if (len > 0){
				ByteBuffer recv = ByteBuffer::ByteBuffer(len, (uint8_t *)recvBuffer);

				int16_t eventType;
				recv.ReadInt16(&eventType);

				switch (eventType){

				case HNPingRequest: {
					HandlePingRequest(recv, sender);
					break;
				}
				case HNFriendCountRequest: {
					HandleFriendCountRequest(recv, sender);
					break;
				}
				case HNFriendAtIndexRequest: {
					HandleFriendAtIndexRequest(recv, sender);
					break;
				}
				default:
					break;
				}
			}
			Sleep(100);
		}

		return 0;
	}

	void SNode::HandlePingRequest(ByteBuffer &Buffer, hAddress sender){
		Network::PingPacket	pingPacket;

		pingPacket.Deserialize(&Buffer);

		printf("Received packet from client from %ul with xuid %llu\n", htonl(sender.Address), pingPacket.ClientID);

		HedgeClient *client = new HedgeClient();
		client->ClientID = pingPacket.ClientID;
		client->isAnonymous = pingPacket.isAnonymous;
		client->isAuthenticated = pingPacket.isAuthenticated;
		client->LastPacketSequenceID = pingPacket.SequenceID;
		client->LastSeen = time(NULL);
		client->addr = sender;
		Clients.push_back(*client);

		ByteBuffer buf = ByteBuffer::ByteBuffer();
		buf.WriteInt16(1);
		std::string s("hello");
		buf.WriteString(s);
		sender.Port = 20000;
		Network::SocketManager::Send_UDP(&sender, buf.GetLength(), buf.GetBuffer<void>());
	}

	void SNode::HandleFriendCountRequest(ByteBuffer &Buffer, hAddress sender){

		HedgePrint("Received a FriendCountRequest");

		Network::NetworkPacket inPacket;
		inPacket.Deserialize(&Buffer);

		Network::FriendCountPacket friendCountPacket;
		friendCountPacket.ApplicationID = inPacket.ApplicationID;
		friendCountPacket.SequenceID = inPacket.SequenceID;
		friendCountPacket.eventType = HNFriendCountResponse;
		friendCountPacket.TimeStamp = time(NULL);
		friendCountPacket.friendsCount = Clients.size();

		sender.Port = 20000;
		ByteBuffer outBuffer;
		friendCountPacket.Serialize(&outBuffer);
		Network::SocketManager::Send_UDP(&sender, outBuffer.GetLength(), outBuffer.GetBuffer<void>());
	}

	void SNode::HandleFriendAtIndexRequest(ByteBuffer &Buffer, hAddress sender){

		HedgePrint("Received a FriendAtIndexRequest");

		Network::NetworkPacket inPacket;
		inPacket.Deserialize(&Buffer);
		int32_t index;
		Buffer.ReadInt32(&index);

		Network::FriendAtIndex friendAtIndexPacket;
		friendAtIndexPacket.ApplicationID = inPacket.ApplicationID;
		friendAtIndexPacket.SequenceID = inPacket.SequenceID;
		friendAtIndexPacket.eventType = HNFriendAtIndexResponse;
		friendAtIndexPacket.TimeStamp = time(NULL);
		uint64_t friendId = 0x1100001DEADC0DE;
		if (Clients.size() > index){
			friendId = Clients.at(index).ClientID;
		}
		friendAtIndexPacket.friendSteamID = friendId;

		ByteBuffer outBuffer;
		friendAtIndexPacket.Serialize(&outBuffer);
		sender.Port = 20000;
		Network::SocketManager::Send_UDP(&sender, outBuffer.GetLength(), outBuffer.GetBuffer<void>());
	}

	bool SNode::InitializeNode(){
		Database::DatabaseManager::InitializeDatabase();
		HANDLE td = CreateThread(NULL, NULL, ServerNodeCommandReceiver, NULL, NULL, NULL);
		return true;
	}
}