namespace Nodes
{
	class SNode
	{
		//we use this for faster acces from friend calls.
		static std::vector<uint64_t> ClientIDS;

		//all other calls will be made with a clientID
		static std::unordered_map<uint64_t, std::shared_ptr<HedgeClient>> Clients;
		static std::unordered_map<uint64_t, std::shared_ptr<HedgeLobby>> Lobbies;

		static std::mutex ClientsMutex;
		static std::mutex LobbiesMutex;

		static DWORD _stdcall ServerNodeCommandReceiver(void  *lparam);

		static void HandlePingRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleFriendCountRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleFriendAtIndexRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleCreateSessionRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleUpdateSessionRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleFindSessionsRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleDeleteSessionRequest(ByteBuffer &Buffer, hAddress sender);
	public:
		static bool InitializeNode();
	
	};
}