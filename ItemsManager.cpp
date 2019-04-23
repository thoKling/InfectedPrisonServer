#include "ItemsManager.h"
#include "SocketManager.h"

std::vector<Item*> ItemsManager::_items;

void ItemsManager::addItem(Item * item, bool replicate)
{
	_items.push_back(item);
	if (replicate) {
		sf::Packet packet;
		packet << SocketManager::PacketType::CreateItem << item;
		SocketManager::broadcastPacket(packet);
	}
}

void ItemsManager::deleteItem(Item * item)
{
	auto toDelete = _items.end();
	for (auto it = _items.begin(); it != _items.end(); it++)
	{
		if ((*it)->getStack() == item->getStack()
			&& (*it)->getPosition() == item->getPosition()
			&& (*it)->getType() == item->getType()) {
			toDelete = it;
		}
	}
	if (toDelete != _items.end())
		_items.erase(toDelete);
}

std::vector<Item*>& ItemsManager::getItems()
{
	return _items;
}
