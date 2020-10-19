#include "BusNode.h"



BusNode::BusNode()
{
	//GLBVRS::MBUSPTR->addReceiver(this->getNotifyFunc());
}

void BusNode::UpdateMessages()
{
}

std::function<void(Message)> BusNode::getNotifyFunc()
{
	auto messageListener = [=](Message message) -> void {
		this->onNotify(message);
	};
	return messageListener;
}

void BusNode::send(Message message)
{
	//GLBVRS::MBUSPTR->sendMessage(message);
}

void BusNode::onNotify(Message message)
{
	// Do something here. Your choice. You could do something like this.
	// std::cout << "Siopao! Siopao! Siopao! (Someone forgot to implement onNotify().)" << std::endl;
}