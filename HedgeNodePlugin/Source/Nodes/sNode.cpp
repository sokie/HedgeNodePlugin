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
		Network::SocketManager::Create_UDP(&Global::Listen_Port, false);

		char *recvBuffer = (char *)malloc(sizeof(char) * 1024);
		hAddress sender = hAddress::hAddress();
		while (true){
			int len = Network::SocketManager::Receive_UDP(Global::Listen_Port, 1024, recvBuffer, &sender);
			if (len > 0){
				ByteBuffer recv = ByteBuffer::ByteBuffer((uint8_t *)recvBuffer, len);

				Network::NetworkPacket *packet = new Network::NetworkPacket(&recv);
				switch (packet->eventType){

				case HNPingRequest: {
					ByteBuffer pingBuffer = ByteBuffer::ByteBuffer((uint8_t *)packet->DataBuffer, packet->DataLength);
					Network::PingPacket *pingPacket = new Network::PingPacket(&pingBuffer);

					HedgePrint("Received packet from client from %s with xuid %u\n", inet_ntoa(*(in_addr *)sender.Address), pingPacket->ClientID);
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
					Network::SocketManager::Send_UDP(&sender, buf.Length(), buf.GetBuffer());
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