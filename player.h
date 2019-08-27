#pragma once
#include <vector>
#include <utility>
#include "inventory.h"
using namespace std;
class Move{
	string name;
	int damage, mana;
	float scaling;
	public:
	Move (string new_name, int new_damage, int new_mana, float new_scale){
		name= new_name;
		damage= new_damage;
		mana= new_mana;
		scaling=new_scale;
	}
};

//class for the player character gets stronger over time
class Hero{
	string name;
	int health, mana,attack,speed,experience,level;
	vector<Move> move_pool;
	pair <int,int> location;
	Inventory loot;
	public:
	Hero(string new_name, int new_health, int new_mana, int new_attack, int new_speed){
		name = new_name;
		health=new_health;
		mana=new_mana;
		attack=new_attack;
		speed = new_speed;
		level=1;
	}
	void set_loc(int x, int y){
		location.first = x;
		location.second = y;
	}
	pair<int,int> get_loc(){
		return location;
	}
	void set_health(int hp){
		health=hp;
	}
	int get_health(){
		return health;
	}
	void set_attack(int hp){
		attack=hp;
	}
	int get_attack(){
		return attack;
	}
	void set_speed(int hp){
		speed=hp;
	}
	int get_speed(){
		return speed;
	}
	void set_mana (int mn){
		mana=mn;
	}
	int get_mana(){
		return mana;
	}
	void set_experience(int x){
		experience+=x;
	}
	int get_experience(){
		return experience;
	}
	void level_up(){
		if ((experience%level) > 20){
			experience = experience%20;
			level++;	
		}
	}
	void add_loot(Item temp){
		loot.add_item(temp);
	}
	void use_loot(int x){
		loot.use_item(x);
	}

	void set_moves(vector<Move>hi){
		move_pool=hi;
	}
	Move get_move_one(){
		return move_pool.at(0);
	}
	Move get_move_two(){
		return move_pool.at(1);
	}
	Move get_move_three(){
		return move_pool.at(2);
	}
	Move get_move_four(){
		return move_pool.at(3);
	}

};

class Enemy{
	int health, attack,speed;
	public:
	Enemy (int hp, int atk, int spd){
		health=hp;
		attack=atk;
		speed=spd;
	}
			
	void set_health(int hp){
		health=hp;
	}
	int get_health(){
		return health;
	}
	
	void set_attack(int hp){
		attack=hp;
	}
	int get_attack(){
		return attack;
	}
	void set_speed(int hp){
		speed=hp;
	}
	int get_speed(){
		return speed;
	}
};

