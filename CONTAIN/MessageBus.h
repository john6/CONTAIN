#pragma once
#include <functional>
#include <queue>
#include <vector>
#include "Message.h"

class MessageBus
{
private:
	std::vector<std::function<void(Message)>> receivers;
	std::queue<Message> messages;

public:
	MessageBus();
	~MessageBus();

	void addReceiver(std::function<void(Message)> messageReceiver);

	void sendMessage(Message message);

	void notify();
};