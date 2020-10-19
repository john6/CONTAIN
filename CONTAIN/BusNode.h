#pragma once
#include <iostream>
#include <functional>
#include "MessageBus.h"
#include "Message.h"

class BusNode
{
public:
	BusNode();

	virtual void UpdateMessages();

protected:

	std::function<void(Message)> getNotifyFunc();

	void send(Message message);

	virtual void onNotify(Message message);
};