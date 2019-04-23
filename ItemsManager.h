#pragma once

#include "Item.h"

#include <vector>

class ItemsManager
{
public:
	static void addItem(Item* item, bool replicate = true);
	static void deleteItem(Item* item);
	static std::vector<Item*>& getItems();

private:
	static std::vector<Item*> _items;
};

