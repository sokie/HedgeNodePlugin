#include "..\StdInclude.h"

std::string quotesql(const std::string& s);

HedgeClient::HedgeClient(){
	ClientID = 0;
	addr = hAddress::hAddress();
	LastSeen = 0;
	LastPacketSequenceID = -1;
	SessionID = 0;
	isAuthenticated = false;
	isAnonymous = false;
	std::string username = "";
}

HedgeClient::HedgeClient(ByteBuffer *InBuffer){

	InBuffer->ReadUInt32(&ClientID);
	InBuffer->ReadUInt32(&LastSeen);
	InBuffer->ReadUInt16(&LastPacketSequenceID);
	InBuffer->ReadUInt32(&SessionID);
	
	//uint8_t *nameLength = (uint8_t *)malloc(sizeof(uint8_t));
	//InBuffer->ReadUInt8(nameLength);
	
	InBuffer->ReadString(username);
}

HedgeClient::~HedgeClient(void){

}

bool HedgeClient::Serialize(ByteBuffer *OutBuffer){
	//we don't send this for now..
	return true;
}

bool HedgeClient::Save(){
	//we don't persist...for now
	return true;
}

bool HedgeClient::Update(){
	//we don't persist...for now
	return true;
}

bool HedgeClient::Remove(){
	
	char str[10];
	sprintf(str, "%u", ClientID);

	std::string sqlstatement =
		"DELETE FROM Clients WHERE xuid = "
		+ quotesql(str) + ");";

	//std::vector<std::vector<std::string> > result = Database::DatabaseManager::query((char *)sqlstatement.c_str());
	//if (result.size > 0){
		return true;
	//}
	//else{
		//return false;
	//}
	/* Random query
	std::vector<std::vector<std::string> > result = Database::DatabaseManager::query("SELECT a,b FROM Clients;");
	for (std::vector<std::vector<std::string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		std::vector<std::string> row = *it;
		DBGPrint("Value %s %s", row.at(0), row.at(1));
	}*/
}

std::string quotesql(const std::string& s) {
	return std::string("'") + s + std::string("'");
}