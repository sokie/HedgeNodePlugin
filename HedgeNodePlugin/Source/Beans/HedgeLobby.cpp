#include "..\StdInclude.h"

std::string quotesql(const std::string& s);

HedgeLobby::HedgeLobby(){

}

HedgeLobby::HedgeLobby(ByteBuffer *InBuffer){
    
}

bool HedgeLobby::Serialize(ByteBuffer *OutBuffer){
    //we don't send this for now..
    return true;
}

bool HedgeLobby::Save(){
    //we don't persist...for now
    return true;
}

bool HedgeLobby::Update(){
    //we don't persist...for now
    return true;
}

bool HedgeLobby::Remove(){
    return true;
}

