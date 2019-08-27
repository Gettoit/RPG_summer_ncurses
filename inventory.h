#pragma once
#include <string>
#include "item.h"

class Inventory{
	std::vector <Item> items;
	public:
	void add_item(Item temp){
		items.push_back(temp);
	}
	void use_item(int x){
		items.at(x-1).use_item();
	}
	bool check_item(int x){
		return items.at(x-1).check_if_used();
	}
	std::string give_name(int x){
		return items.at(x-1).get_name();
	}
};

