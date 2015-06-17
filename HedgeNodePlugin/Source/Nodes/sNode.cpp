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

				Network::NetworkPacket *packet = new Network::NetworkPacket(&recv);

				switch (packet->eventType){

				case HNPingRequest: {
					ByteBuffer pingBuffer = ByteBuffer::ByteBuffer(packet->DataLength, (uint8_t *)packet->DataBuffer);
					Network::PingPacket *pingPacket = new Network::PingPacket(&pingBuffer);

					HedgePrint("Received packet from client from %ul with xuid %u\n", htonl(sender.Address), pingPacket->ClientID);
					HedgeClient *client = new HedgeClient();
					client->ClientID = pingPacket->ClientID;
					client->isAnonymous = pingPacket->isAnonymous;
					client->isAuthenticated = pingPacket->isAuthenticated;
					client->LastPacketSequenceID = packet->SequenceID;
					client->LastSeen = time(NULL);
					client->addr = sender;
					Clients.push_back(*client);

					//Construct a proper ping response later on
					// whatever the client doesn't care
					ByteBuffer buf = ByteBuffer::ByteBuffer();
					buf.WriteInt16(1);
					std::string s("hello");
					buf.WriteString(s);
					sender.Port = 20000;
					Network::SocketManager::Send_UDP(&sender, buf.GetLength(), buf.GetBuffer<void>());
					break;
				}
				case HNFriendCountRequest: {
					ByteBuffer buf = ByteBuffer::ByteBuffer();
					Network::NetworkPacket *friendResponse = new Network::NetworkPacket();
					friendResponse->ApplicationID = packet->ApplicationID;
					friendResponse->SequenceID = packet->SequenceID;
					friendResponse->eventType = HNFriendCountResponse;
					friendResponse->TimeStamp = time(NULL);
					friendResponse->DataLength = sizeof(uint32_t);
					uint32_t size = Clients.size();
					friendResponse->DataBuffer = &size;
					friendResponse->Serialize(&buf);
					sender.Port = 20000;
					Network::SocketManager::Send_UDP(&sender, buf.GetLength(), buf.GetBuffer<void>());
					break;
				}
				case HNFriendAtIndexRequest: {
					ByteBuffer buf = ByteBuffer::ByteBuffer();
					Network::NetworkPacket *friendResponse = new Network::NetworkPacket();
					friendResponse->ApplicationID = packet->ApplicationID;
					friendResponse->SequenceID = packet->SequenceID;
					friendResponse->eventType = HNFriendAtIndexResponse;
					friendResponse->TimeStamp = time(NULL);
					friendResponse->DataLength = sizeof(uint64_t);
					int32_t index = *((int32_t *)packet->DataBuffer);
					uint64_t friendId = 0x1100001DEADC0DE;
					if (Clients.size > index){
						friendId = Clients.at(index).ClientID;
					}
					
					friendResponse->DataBuffer = &friendId;
					friendResponse->Serialize(&buf);
					sender.Port = 20000;
					Network::SocketManager::Send_UDP(&sender, buf.GetLength(), buf.GetBuffer<void>());
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

	bool SNode::InitializeNode(){
		Database::DatabaseManager::InitializeDatabase();
		HANDLE td = CreateThread(NULL, NULL, ServerNodeCommandReceiver, NULL, NULL, NULL);
		return true;
	}
}