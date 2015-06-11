/*
This project is licensed under the GNU GPL 2.0 license. Please respect that.

Initial author: (https://github.com/)Sokie
Started: 11-06-2015
Notes
This handles connection to a server node
*/

#include "..\STDInclude.h"
namespace Nodes
{
	hAddress CNode::serverNode;
	int CNode::sequenceID = 0;
	HANDLE CNode::sNodeDiscoveryThread;

	DWORD _stdcall CNode::NodeDiscoverySender(void  *lparam)
	{
		uint16_t port = 20000;
		Network::SocketManager::Create_UDP(&port, false);

		char *recvBuffer = (char *)malloc(sizeof(char) * 1024);
		hAddress sender = hAddress::hAddress();
		while (true){
			HedgePrint("Searching for sNode\n");
			ByteBuffer packetBuffer = ByteBuffer::ByteBuffer();

			//Dummy data for now
			Network::PingPacket *pingPacket = new Network::PingPacket();
			pingPacket->ClientID = 76561197960287930;
			pingPacket->isAnonymous = true;
			pingPacket->isAuthenticated = true;
			pingPacket->SessionID = rand();
			pingPacket->username = "sokie";
			ByteBuffer pingBuffer = ByteBuffer::ByteBuffer();
			pingPacket->Serialize(&pingBuffer);

			Network::NetworkPacket *packet = new Network::NetworkPacket();
			packet->ApplicationID = 202990;
			packet->eventType = HNPingRequest;
			packet->SequenceID = sequenceID++;
			packet->TimeStamp = time(NULL);
			packet->DataLength = pingBuffer.Length();
			packet->DataBuffer = pingBuffer.GetBuffer();
			packet->Serialize(&packetBuffer);

			Network::SocketManager::Send_UDPBroadcast(Global::Listen_Port, packetBuffer.Length(), packetBuffer.GetBuffer());
			int len = Network::SocketManager::Receive_UDP(port, 1024, recvBuffer, &sender);
			if (len > 0){
				serverNode = sender;
				HedgePrint("Found sNode, stopping...\n");
				break;
			}
			Sleep(100);
		}

		return 0;
	}

	bool CNode::InitializeNode(){
		sequenceID = 0;
		sNodeDiscoveryThread = CreateThread(NULL, NULL, NodeDiscoverySender, NULL, NULL, NULL);
		return true;
	}
}