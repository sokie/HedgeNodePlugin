namespace Nodes
{
	class CNode
	{
		static hAddress serverNode;
		static int sequenceID;
		static HANDLE sNodeDiscoveryThread;

		static DWORD _stdcall NodeDiscoverySender(void  *lparam);
	public:
		static bool InitializeNode();

	};
}