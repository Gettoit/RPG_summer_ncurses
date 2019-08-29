#pragma once
#include <vector>
#include <utility>
#include "inventory.h"
#include <fstream>
#include <iostream>
using namespace std;
struct Move{
	string name;
	int damage, mana_cost, healing;
	float damage_multiplier;
	int level_req;	

};

//class for the player character gets stronger over time
class Hero{
	string name;
	int health, mana,attack,speed,experience,level;
	int max_health;
	vector<Move> move_list;
	vector<Move> move_pool;
	pair <int,int> location;
	Inventory loot;
	public:
	void add_test_moves(){
		for (int i = 0; i < 4; i++){
			move_pool.at(i)=move_list.at(i);
		}	
	}
	void move_list_fill(){
		move_pool.resize(4);
		ifstream infile;
		infile.open("moves.txt");
		while(!infile.eof()){
			string name;
			int damage, mana, healing;
			float mul;
			int req;
			if (!cin)break;
			infile>>name;
			if(infile.eof()) break;
			infile>>damage>>mana>>healing>>mul>>req;
			Move temp{name,damage,mana,healing,mul,req};
			move_list.push_back(temp);
		}
		add_test_moves();
	}
	Hero(string new_name, int new_health, int new_mana, int new_attack, int new_speed){
		name = new_name;
		health=new_health;
		max_health=new_health;
		mana=new_mana;
		attack=new_attack;
		speed = new_speed;
		level=1;
		move_list_fill();

	}
	void set_moves(int replace, int move_choice){
		;	
	

	}
	int get_max_health(){
		return max_health;
	}
	void set_loc(int x, int y){
		location.first = x;
		location.second = y;
	}
	pair<int,int> get_loc(){
		return location;
	}
	void take_damage(int damage){
		health-= damage;
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
	void health_pack(int hp){
		health+=hp;
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
	int get_level(){
		return level;
	}
	int damage_calc(int selection){
		float temp = move_list.at(selection-1).damage;
		if (temp)
			temp += level*(move_list.at(selection-1).damage_multiplier);
		if (move_list.at(selection-1).healing!=0)
			health+=move_list.at(selection-1).healing + (level*(move_list.at(selection-1).damage_multiplier));
	
		/*
		   if (health>=max_health)
			health=max_health;
		*/
		return temp;
	}
	void gain_level(){
		if ((experience%level) > 20){
			experience = experience%20;
			level++;	
		}
		health=max_health;
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
	int health, attack,speed, level, modifier;
	public:
	Enemy (int new_level){
		level = new_level;
		health= new_level * 30;
		attack = new_level *4;
		speed = new_level*5;
		modifier = rand()% speed;
	}
	int deal_damage(){
		return attack + modifier;
	}
	void set_health(int hp){
		health=hp;
	}
	void take_damage(int damage){
		health-=damage;
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

