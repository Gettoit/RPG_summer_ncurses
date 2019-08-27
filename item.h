#pragma once

class Item{
	std::string name;
	bool used = false;
	public:
	Item (std::string new_name){
		name=new_name;
	}
	std::string get_name(){
		return name;
	}
	void use_item(){
		used = true;
	}
	bool check_if_used(){
		return used;
	}
};

