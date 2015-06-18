namespace Nodes
{
	class SNode
	{
		static std::vector<HedgeClient> Clients;

		static std::mutex ThreadSafe;

		static DWORD _stdcall ServerNodeCommandReceiver(void  *lparam);

		static void HandlePingRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleFriendCountRequest(ByteBuffer &Buffer, hAddress sender);
		static void HandleFriendAtIndexRequest(ByteBuffer &Buffer, hAddress sender);
	public:
		static bool InitializeNode();
	
	};
}