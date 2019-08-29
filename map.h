#pragma once
#include <vector> 
#include <string>
#include <random>
#include <ncurses.h>
#include <fstream>
#include "player.h"
//for testing
#include <iostream>
using namespace std;


class Game_map{
	vector <string> map;
	vector <pair<int,int>> monster_loc;
	vector<vector<bool>> is_grass;
	vector<vector<bool>> is_monster;
	vector<vector<bool>> is_exit;
	vector <Enemy> enemies;
	
	//No longer use exit to leave the floor
	//bool exit=false;
	public:
	static const char EXIT = 'E';
	static const char HERO = 'H';
	static const char MONSTER = 'M';
	static const char WALL = '#';
	static const char GRASS = '!';
	static const char WATER = '~';
	static const char OPEN = '.';
	static const char TREASURE = '$';
	static const char HEALTH_UP = '+';
	static const char TERRAIN = '@';
	static const char BOSS = 'B';
	static const char LADDER_UP = '^';
	static const char LADDER_DOWN = 'v';
	static const size_t SIZE = 45;
	static const size_t SIZEH = 100;
	
	//TEST
	//static const size_t SIZE = 20;
	//static const size_t SIZEH = 20;

	static const size_t DISPLAY = 19;
	
	
	//open world map may make a dungeon like map just comment out and make a new one if need be
	void init_map() {
		random_device rd;     // only used once to initialise (seed) engine
		mt19937 gen(rd());    // random-number engine used (Mersenne-Twister in this case)
		uniform_int_distribution<int> d100(1,100);
		map.clear();
		map.resize(SIZE); 
		is_grass.resize(SIZE);
		is_exit.resize(SIZE);
		is_monster.resize(SIZE);
		for (auto &v : map) v.resize(SIZEH,' '); 
		for (auto &v : is_grass) v.resize(SIZEH,false);
		for (auto &v : is_exit) v.resize(SIZEH,false);
		for (auto &v : is_monster) v.resize(SIZEH,false);
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZEH; j++) {
				//Line the map with walls
				if ((i==0 and (j>=(SIZEH/2 -1) and j<=(SIZEH/2+1))) or (j==0 and (i>=(SIZE/2 -1) and i<=(SIZE/2+1))) or (i==SIZE-1 and (j>=(SIZEH/2 -1) and j<=(SIZEH/2+1))) or (j==SIZEH-1 and (i>=(SIZE/2 -1) and i<=(SIZE/2+1)))) {
						
					map.at(i).at(j) = EXIT;
					is_exit.at(i).at(j)=true;

				}
				else if ((i ==0 and j==0) or (i==0 and j==SIZEH-1) or j == 0 or j == SIZEH -1 or i == 0 or i == SIZE -1)
					map.at(i).at(j) =WALL;
				

				// CHANGED HOW BOSS WORKS NO LONGER IN EVERY ROOM
				//TODO: make the boss spawn only in 1 room
				/*
				else if ( i==SIZE/2 and j==SIZEH/2)
					map.at(i).at(j)=BOSS;
					*/



				//CHANGED HOW THE EXIT WORKS
				//TODO: Make one ladder down and one up (no up on first floor and no down on last floor
				/*
				else if (exit == false  && (d100(gen)==1 or (i==28 and j==28))){
					exit=true;
					map.at(i).at(j)=EXIT;
				}
				*/

				else {
					//2% chance of health gain
					//TODO: change this maybe remove random health ups on the map or limit them to a shop???
					
					if (d100(gen) <=1){
						map.at(i).at(j) = HEALTH_UP;
					}
					
					//5% change to spawn a monster
					//TODO: maybe make the monsters hidden and only show up in grass need to determine that when game is closer to being playable
					//TODO: monsters from visible map and add them to be only spawned in grass may need to move this entirely
					
					/*
					else if (d100(gen) <= 3) {
						map.at(i).at(j) = TREASURE;
					}
					*/

					if (d100(gen) <= 1) { //3% each spot is water
						map.at(i).at(j) = WATER;
					}
					else if (d100(gen) <= 30) { //20% chance of water near other water
						if (map.at(i-1).at(j) == WATER or map.at(i+1).at(j) == WATER or map.at(i).at(j-1) == WATER or map.at(i).at(j+1) == WATER)
							map.at(i).at(j) = WATER;

					}
					else if (d100(gen)<=2){
						map.at(i).at(j)=GRASS;
						is_grass.at(i).at(j)=true;
						if (d100(gen) <= 5) {
							is_monster.at(i).at(j)=true;
							//monster_loc.push_back(make_pair(i,j));
						}
					}
					else if (d100(gen)<=95){
						if (map.at(i-1).at(j) == GRASS or map.at(i+1).at(j) == GRASS or map.at(i).at(j-1) == GRASS or map.at(i).at(j+1) == GRASS){
							map.at(i).at(j)=GRASS;
							is_grass.at(i).at(j)=true;
							if (d100(gen) <= 5) {
								is_monster.at(i).at(j)=true;
								//monster_loc.push_back(make_pair(i,j));
					}
						}

					}
				}
			}
		}
	}


	//draws the map to the screen with ncurses
	void draw(int x, int y) {
		int start_x = 0;
		int end_x = SIZEH -1;
		int start_y = 0;
		int end_y = SIZE -1;

		//Bounds check to handle the edges
		if (start_x < 0) {
			end_x = end_x - start_x;
			start_x = 0;
		}
		if (end_x > int(SIZE-1)) {
			start_x = start_x - (end_x - (SIZEH-1));
			end_x = SIZEH-1;
		}
		if (start_y < 0) {
			end_y = end_y - start_y;
			start_y = 0;
		}
		if (end_y > int(SIZE-1)) {
			start_y = start_y - (end_y - (SIZE-1));
			end_y = SIZE-1;
		}

		//Now draw the map using NCURSES
		for (int i = start_y; i <= end_y; i++) {
			for (int j = start_x; j <= end_x; j++) {
				//if (i == cursor_x && j == cursor_y)
				//  attron(A_UNDERLINE | A_BOLD);
				int color = 10;
				if (map.at(i).at(j) == WALL)
					color = 5;
				else if (map.at(i).at(j) == WATER)
					color = 2;
				else if (map.at(i).at(j) == HERO)
					color = 3;
				else if (map.at(i).at(j) == TREASURE)
					color = 4;
				else if (map.at(i).at(j) == MONSTER)
					color = 6;
				else if (map.at(i).at(j) == TERRAIN)
					color = 5;
				else if (map.at(i).at(j) == HEALTH_UP)
					color = 0;
				else if (map.at(i).at(j) == BOSS)
					color = 7;
				else if (map.at(i).at(j) == GRASS)
					color = 9;
				else if (map.at(i).at(j) == EXIT)
					color = 1;
				attron(COLOR_PAIR(color));
				mvaddch(i-start_y,j-start_x,map.at(i).at(j));
				attroff(COLOR_PAIR(color));
			}
		}
	}
	//FOR TESTING PURPOSES ONLY
	/*
	   void print_map(){
	   for (auto a: map) cout<<a<<endl;
	   }*/
	//function moves monsters randomly and updates their position both on the board and in the vector where the monsters are stored may have to change this to only have them move on grass will determine if thats needed when game is getting close to finishing
	void enemy_movement(){
		for (size_t i = 0; i < monster_loc.size(); i++){
			int x = rand() %4;
			if (x==0){
				if (monster_loc.at(i).second< int(SIZE-2)){
					if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second) == WALL);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == HERO);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == BOSS);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == EXIT);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == MONSTER);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == WALL);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == WATER);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == TREASURE);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second+1) == HEALTH_UP);

					else {
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= OPEN;
						monster_loc.at(i).second++;
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= MONSTER;
					}
				}
			}
			else if(x==1){
				if (monster_loc.at(i).first<int(SIZE-2)){
					if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second) == WALL);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == HERO);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == BOSS);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == EXIT);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == MONSTER);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == WALL);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == WATER);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == TREASURE);
					else if (map.at(monster_loc.at(i).first+1).at(monster_loc.at(i).second) == HEALTH_UP);
					else {
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= OPEN;
						monster_loc.at(i).first++;
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= MONSTER;
					}
				}
			}
			else if (x==2){
				if (monster_loc.at(i).second>1){
					if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second) == WALL);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == HERO);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == BOSS);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == EXIT);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == MONSTER);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == WALL);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == WATER);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == TREASURE);
					else if (map.at(monster_loc.at(i).first).at(monster_loc.at(i).second-1) == HEALTH_UP);
					else {
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= OPEN;
						monster_loc.at(i).second--;
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= MONSTER;
					}
				}
			}
			else if (x==3){
				if (monster_loc.at(i).first>1){
					if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == WALL);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == HERO);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == BOSS);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == EXIT);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == MONSTER);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == WALL);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == WATER);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == TREASURE);
					else if (map.at(monster_loc.at(i).first-1).at(monster_loc.at(i).second) == HEALTH_UP);
					else {
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= OPEN;
						monster_loc.at(i).first--;
						map.at(monster_loc.at(i).first).at(monster_loc.at(i).second)= MONSTER;

					}
				}
			}
		}
	}
	//function removes the monster from the map when defeated
	/*void remove_monster(int x , int y){
		pair <int,int> k = make_pair(x,y);
		for (size_t i=0; i <monster_loc.size();i++){
			if (monster_loc.at(i).first==y && monster_loc.at(i).second==x){
				monster_loc.at(i).first=monster_loc.at(monster_loc.size()-1).first;
				monster_loc.at(i).second=monster_loc.at(monster_loc.size()-1).second;
				monster_loc.pop_back();
			}

		}

	}*/
	void remove_monster(int x , int y){
		is_monster.at(y).at(x)=false;
	}
	bool check_monster(int x, int y){
		return is_monster.at(y).at(x);
	}
	//Replaces the player's old location with and empty space may need to change it to originial is like grass or something check in the future
	void set_player_loc(int x, int y, char c) {
		for (unsigned int i =0; i < SIZE;i++){
			for (unsigned int j =0; j < SIZEH;j++){
				if (map.at(i).at(j)=='H'){
					if(is_exit.at(i).at(j) == true)
						map.at(i).at(j) = EXIT;
					if(is_grass.at(i).at(j) == true)
						map.at(i).at(j) = GRASS;
					else
						map.at(i).at(j)=' ';
				}
			}
		}

		map.at(y).at(x) = c;
	}
	//function checks the location the player wants to move and will bounce off if wall or trigger an effect
	char get_current_loc(int x, int y) {
		return map.at(y).at(x);
	}
	//DO COMBAT HERE DUMMY
	void combat(Hero &hero){
		const int UP = 65; //Key code for up arrow (also use WASD)
		const int DOWN = 66; //code for down 
		const int LEFT = 68; //code for left
		const int RIGHT = 67; //code for right
		const int NUM_OPTIONS = 4;
		system("clear");
		Enemy monster(hero.get_level());
		//points is a variable that determies where the pointer for the interface should be 1-4 options for moves may add more options later	
		int points = 1;
		while (true){
			int ch = getch();
			if (ch==UP){
				points-=2;

			}
			else if (ch==DOWN){
				points+=2;
			}
			else if (ch==LEFT){
				points-=1;
				if (points<1)
					points = 2;
				else if (points == 2)
					points = 4;


			}
			else if (ch==RIGHT){
				points+=1;
				if (points>NUM_OPTIONS){
					points=3;
				}
			}
			else if (ch == 10 or ch == 13){
				if (points == 1) {
					//add moves later to this
					break;
				}
				else if (points == 2){
					break;
				}
				else if (points == 3){
					break;
				}
				else if (points == 4){
					break;
				}


						
			}
			//do nothing
			else if (ch==ERR){;}
			if (points == 1){
				mvprintw(15,28,">");
				mvprintw(20,43," ");
				mvprintw(20,28," ");
				mvprintw(15,43," ");
			}
			if (points == 2){
				mvprintw(20,43," ");
				mvprintw(15,28," ");
				mvprintw(20,28," ");
				mvprintw(15,43,">");
			}
			if (points == 3){
				mvprintw(20,28,">");
				mvprintw(15,28," ");
				mvprintw(20,43," ");
				mvprintw(15,43," ");
			}
			if (points == 4){
				mvprintw(20,43,">");
				mvprintw(15,28," ");
				mvprintw(20,28," ");
				mvprintw(15,43," ");
			}
			mvprintw(15,45,"MOVE 2");
			mvprintw(20,30,"MOVE 3");
			mvprintw(20,45,"MOVE 4");
			mvprintw(15,30,"MOVE 1");
			mvprintw(0,0,"Enemy Health: %d", monster.get_health());
			mvprintw(15,0,"Player Health: %d / %d", hero.get_health(), hero.get_max_health());


		}

		return;

	}
	void spawn_ladder_down(int x, int y){
		map.at(y).at(x)=LADDER_DOWN;
	}
	void spawn_ladder_up(int x, int y){
		map.at(y).at(x)=LADDER_UP;
	}
	void spawn_boss(){
		map.at(SIZE/2).at(SIZEH/2)=BOSS;
	}
	//easier to call init_map no other purpose but to save typing 
	Game_map() {
		init_map();
	}


};
