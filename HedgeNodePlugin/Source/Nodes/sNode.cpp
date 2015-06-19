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
	std::vector<uint64_t> SNode::ClientIDS;
	std::unordered_map<uint64_t, std::shared_ptr<HedgeClient>> SNode::Clients;
	std::unordered_map<uint64_t, std::shared_ptr<HedgeLobby>> SNode::Lobbies;

	std::mutex SNode::ClientsMutex;
	std::mutex SNode::LobbiesMutex;

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
				case HNCreateSessionRequest: {
					HandleCreateSessionRequest(recv, sender);
					break;
				}
				case HNUpdateSessionRequest: {
					HandleUpdateSessionRequest(recv, sender);
					break;
				}
				case HNFindSessionsRequest: {
					HandleFindSessionsRequest(recv, sender);
					break;
				}
				case HNDeleteSessionRequest: {
					HandleDeleteSessionRequest(recv, sender);
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

		ClientsMutex.lock();
		std::unordered_map<uint64_t, std::shared_ptr<HedgeClient>>::const_iterator find = Clients.find(pingPacket.ClientID);
		ClientsMutex.unlock();

		if (find == Clients.end()){
			HedgeClient client;
			client.ClientID = pingPacket.ClientID;
			client.isAnonymous = pingPacket.isAnonymous;
			client.isAuthenticated = pingPacket.isAuthenticated;
			client.LastPacketSequenceID = pingPacket.SequenceID;
			client.LastSeen = time(NULL);
			client.addr = sender;
			ClientsMutex.lock();
			Clients[pingPacket.ClientID] = std::shared_ptr<HedgeClient>(&client);
			ClientsMutex.unlock();
			ClientIDS.push_back(pingPacket.ClientID);
		}
		else{
			//Updates are made while locked, we should probably benchmark this
			ClientsMutex.lock();
			std::shared_ptr<HedgeClient> client = find->second;

			HedgeClient *hc = client.get();
			hc->ClientID = pingPacket.ClientID;
			hc->isAnonymous = pingPacket.isAnonymous;
			hc->isAuthenticated = pingPacket.isAuthenticated;
			hc->LastPacketSequenceID = pingPacket.SequenceID;
			hc->LastSeen = time(NULL);
			hc->addr = sender;
			ClientsMutex.unlock();
		}

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
			friendId = ClientIDS.at(index);
		}
		friendAtIndexPacket.friendSteamID = friendId;

		ByteBuffer outBuffer;
		friendAtIndexPacket.Serialize(&outBuffer);
		sender.Port = 20000;
		Network::SocketManager::Send_UDP(&sender, outBuffer.GetLength(), outBuffer.GetBuffer<void>());
	}

	void SNode::HandleCreateSessionRequest(ByteBuffer &Buffer, hAddress sender){

		HedgePrint("Received a CreateSessionRequest");
		Network::NetworkPacket inPacket;
		inPacket.Deserialize(&Buffer);

		HedgeLobby lobby;
		lobby.HostAddress = Buffer.ReadString();
		lobby.Gametype = Buffer.ReadUInt32();
		lobby.MaxPlayers = Buffer.ReadUInt32();
		lobby.PlayerCount = 0;
		lobby.SessionID = rand();

		LobbiesMutex.lock();
		Lobbies[lobby.SessionID] = std::shared_ptr<HedgeLobby>(&lobby);
		LobbiesMutex.unlock();

		sender.Port = 20000;
		ByteBuffer outBuffer;
		inPacket.eventType = HNCreateSessionResponse;
		inPacket.TimeStamp = time(NULL);
		inPacket.Serialize(&outBuffer);

		outBuffer.WriteUInt64(lobby.SessionID);
		Network::SocketManager::Send_UDP(&sender, outBuffer.GetLength(), outBuffer.GetBuffer<void>());
	}

	void SNode::HandleUpdateSessionRequest(ByteBuffer &Buffer, hAddress sender){
		HedgePrint("Received a UpdateSessionRequest");
		Network::NetworkPacket inPacket;
		inPacket.Deserialize(&Buffer);
		uint64_t SessionID = Buffer.ReadUInt64();
		ByteString blobData = Buffer.ReadBlob();

		LobbiesMutex.lock();
		std::unordered_map<uint64_t, std::shared_ptr<HedgeLobby>>::const_iterator find = Lobbies.find(SessionID);
		LobbiesMutex.unlock();

		if (find == Lobbies.end()){
			HedgePrint("UpdateSession, Lobby not found!");
		}
		else
		{
			//Updates are made while locked, we should probably benchmark this
			LobbiesMutex.lock();
			std::shared_ptr<HedgeLobby> lobby = find->second;

			HedgeLobby *hl = lobby.get();
			hl->LobbyBlobData = blobData;
			LobbiesMutex.unlock();
		}
	}

	void SNode::HandleFindSessionsRequest(ByteBuffer &Buffer, hAddress sender){
		HedgePrint("Received a FindSessionsRequest");
		Network::NetworkPacket inPacket;
		inPacket.Deserialize(&Buffer);

		sender.Port = 20000;
		ByteBuffer outBuffer;
		inPacket.eventType = HNFindSessionsResponse;
		inPacket.TimeStamp = time(NULL);
		inPacket.Serialize(&outBuffer);

		outBuffer.WriteUInt32(Lobbies.size());
		for (auto it = Lobbies.begin(); it != Lobbies.end(); ++it){
			std::shared_ptr<HedgeLobby> lobby = it->second;
			HedgeLobby *hl = lobby.get();
			outBuffer.WriteBlob(&hl->LobbyBlobData);
		}
		Network::SocketManager::Send_UDP(&sender, outBuffer.GetLength(), outBuffer.GetBuffer<void>());
	}

	void SNode::HandleDeleteSessionRequest(ByteBuffer &Buffer, hAddress sender){
		HedgePrint("Received a DeleteSessionRequest");
		Network::NetworkPacket inPacket;
		inPacket.Deserialize(&Buffer);
		uint64_t SessionID = Buffer.ReadUInt64();
		LobbiesMutex.lock();
		std::unordered_map<uint64_t, std::shared_ptr<HedgeLobby>>::const_iterator find = Lobbies.find(SessionID);
		if (find != Lobbies.end()){
			Lobbies.erase(find);
		}
		LobbiesMutex.unlock();
	}

	bool SNode::InitializeNode(){
		Database::DatabaseManager::InitializeDatabase();
		HANDLE td = CreateThread(NULL, NULL, ServerNodeCommandReceiver, NULL, NULL, NULL);
		return true;
	}
}