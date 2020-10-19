#include "Message.h"

Message::Message(std::shared_ptr<Entity> i_entPtr)
{
	entSpawnPtr = i_entPtr;
}

std::shared_ptr<Entity> Message::GetEntSpawnEvnt()
{
	return entSpawnPtr;
}

//std::string Message::getEvent()
//{
//	return messageEvent;
//}