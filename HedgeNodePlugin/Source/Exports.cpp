/*
	This project is licensed under the GNU GPL 2.0 license. Please respect that.

	Initial author: (https://github.com/)Sokie
	Started: 2014-10-01
	Notes
		A plugin that handles networking
*/

#include "STDInclude.h"

uint32_t PreInit()
{
	Global::Node_is_sNode = GetPrivateProfileIntA("Node", "IsServerNode", 0, "./HedgeNode.ini") == TRUE;
	Global::Listen_Port = GetPrivateProfileIntA("Node", "ListenUDPPort", 31337, "./HedgeNode.ini");
	GetPrivateProfileStringA("Node", "DatabaseName", "./Node.db", Global::Database_name, 16, "./HedgeNode.ini");

	Network::SocketManager::InitializeManager();

	if (Global::Node_is_sNode){

		Nodes::SNode::InitializeNode();

	}
	else{
		
		Nodes::SNode::InitializeNode();

	}

	return 1;
}

uint32_t PostInit()
{
	// Memory changes.
	return 1;
}

uint32_t AuthorInfo()
{
	printf("https://github.com/Sokie\n");
	return 1;
}

uint32_t ExtendedInfo()
{
	printf("A plugin to handle networking.\n");
	return 1;
}

uint32_t OfficialMod()
{
	return 1;
}

 uint32_t DependencyCount()
{
	return 0;
}

const char *GetDependency(uint32_t Index)
{
	switch(Index)
	{
	default:
		return "InvalidPlugin";
	}
}

uint64_t* SendMessageB(const char* Message, void* Callback){
	static uint64_t ret = 1;
	return &ret;
}