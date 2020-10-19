#pragma once
#include <string>

class Entity;

class Message
{
private:
	std::shared_ptr<Entity> entSpawnPtr;
	//std::string messageEvent;

public:
	Message(std::shared_ptr<Entity> i_entPtr);

	//std::string getEvent();

	std::shared_ptr<Entity> GetEntSpawnEvnt();
};

