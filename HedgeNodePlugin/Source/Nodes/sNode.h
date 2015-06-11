namespace Nodes
{
	class SNode
	{
		static std::vector<HedgeClient> Clients;

		static std::mutex ThreadSafe;

		static DWORD _stdcall ServerNodeCommandReceiver(void  *lparam);

	public:
		static bool InitializeNode();
	
	};
}