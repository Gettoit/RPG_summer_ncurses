#pragma once
#include <vector>
#include <string>
#include <random>
#include <ncurses.h>
using namespace std; //Boo hiss

class Map {
	vector<vector<char>> map;
	vector <pair<int,int>> m;
	bool exit = false;
	public:
	static const char EXIT 	= 'E';
	static const char HERO     = 'H';
	static const char MONSTER  = 'M';
	static const char VWALL     = '|';
	static const char HWALL    = '_';
	static const char WATER    = '~';
	static const char OPEN     = '.';
	static const char TREASURE = '$';
	static const char HEALTH_UP = '^';
	static const char TERRAIN = '@';
	static const char BOSS = 'B';
	static const size_t SIZE = 30; //World is a 100x100 map
	static const size_t DISPLAY = 20; //Show a 20x20 area at a time
	//Randomly generate map
	void init_map() {
		random_device rd;     // only used once to initialise (seed) engine
		mt19937 gen(rd());    // random-number engine used (Mersenne-Twister in this case)
		uniform_int_distribution<int> d100(1,100);
		map.clear();
		map.resize(SIZE); //100 rows tall
		for (auto &v : map) v.resize(SIZE,'.'); //100 columns wide
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				//Line the map with walls
				if ((i ==0 and j==0) or (i==0 and j==SIZE-1))
					map.at(i).at(j) =HWALL;
				else if (j == 0  or j == SIZE-1) 
					map.at(i).at(j) = VWALL;
				else if (i == 0  or i == SIZE-1) 
					map.at(i).at(j) = HWALL;
				else if ( i==SIZE/2 and j==SIZE/2)
					map.at(i).at(j)=BOSS;
				else if (exit == false  && (d100(gen)==1 or (i==28 and j==28))){
						exit=true;
						map.at(i).at(j)=EXIT;
					}
				else {
					//5% chance of monster
					if (d100(gen) <=2){
						map.at(i).at(j) = HEALTH_UP;
					}
					else if (d100(gen) <= 5) {
						map.at(i).at(j) = MONSTER;
						m.push_back(make_pair(i,j));
					}
					else if (d100(gen) <= 3) {
						map.at(i).at(j) = TREASURE;
					}
					/*	else if (d100(gen) <=2)
						map.at(i).at(j)= TERRAIN;
						*/	else if (d100(gen) <= 3) { //3% each spot is water
							map.at(i).at(j) = WATER;
						}
					else if (d100(gen) <= 50) { //20% chance of water near other water
						if (map.at(i-1).at(j) == WATER or map.at(i+1).at(j) == WATER or map.at(i).at(j-1) == WATER or map.at(i).at(j+1) == WATER)
							map.at(i).at(j) = WATER;
					}
				}
			}
		}
	}
	//Draw the DISPLAY tiles around coordinate (x,y)
	void draw(int x, int y) {
		int start_x = x - DISPLAY/2;
		int end_x = x + DISPLAY/2;
		int start_y = y - DISPLAY/2;
		int end_y = y + DISPLAY/2;

		//Bounds check to handle the edges
		if (start_x < 0) {
			end_x = end_x - start_x;
			start_x = 0;
		}
		if (end_x > int(SIZE-1)) {
			start_x = start_x - (end_x - (SIZE-1));
			end_x = SIZE-1;
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
				//	attron(A_UNDERLINE | A_BOLD);
				int color = 8;
				if (map.at(i).at(j) == VWALL or map.at(i).at(j) ==HWALL)
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
				else if (map.at(i).at(j) == EXIT)
					color = 1;
				attron(COLOR_PAIR(color));
				mvaddch(i-start_y,j-start_x,map.at(i).at(j));
				attroff(COLOR_PAIR(color));
				//attroff(A_UNDERLINE | A_BOLD);
			}
		}
	}
	//dont need this function if doing JRPG game DONT DELETE JUST DONT USE
	void monster_move(){
		for (size_t i = 0; i < m.size(); i++){
			int x = rand() %4;
			if (x==0){
				if (m.at(i).second<SIZE-2){
					if (map.at(m.at(i).first).at(m.at(i).second) == VWALL);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == HERO);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == BOSS);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == EXIT);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == MONSTER);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == HWALL);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == WATER);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == TREASURE);
					else if (map.at(m.at(i).first).at(m.at(i).second+1) == HEALTH_UP);

					else {
						map.at(m.at(i).first).at(m.at(i).second)= OPEN;
						m.at(i).second++;
						map.at(m.at(i).first).at(m.at(i).second)= MONSTER;
					}	
				}
			}
			else if(x==1){
				if (m.at(i).first<SIZE-2){
					if (map.at(m.at(i).first).at(m.at(i).second) == VWALL);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == HERO);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == BOSS);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == EXIT);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == MONSTER);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == HWALL);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == WATER);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == TREASURE);
					else if (map.at(m.at(i).first+1).at(m.at(i).second) == HEALTH_UP);
					else {
						map.at(m.at(i).first).at(m.at(i).second)= OPEN;
						m.at(i).first++;
						map.at(m.at(i).first).at(m.at(i).second)= MONSTER;
					}	
				}
			}
			else if (x==2){
				if (m.at(i).second>1){
					if (map.at(m.at(i).first).at(m.at(i).second) == VWALL);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == HERO);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == BOSS);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == EXIT);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == MONSTER);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == HWALL);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == WATER);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == TREASURE);
					else if (map.at(m.at(i).first).at(m.at(i).second-1) == HEALTH_UP);
					else {
						map.at(m.at(i).first).at(m.at(i).second)= OPEN;
						m.at(i).second--;
						map.at(m.at(i).first).at(m.at(i).second)= MONSTER;
					}	
				}
			}
			else if (x==3){
				if (m.at(i).first>1){
					if (map.at(m.at(i).first-1).at(m.at(i).second) == VWALL);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == HERO);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == BOSS);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == EXIT);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == MONSTER);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == HWALL);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == WATER);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == TREASURE);
					else if (map.at(m.at(i).first-1).at(m.at(i).second) == HEALTH_UP);
					else {
						map.at(m.at(i).first).at(m.at(i).second)= OPEN;
						m.at(i).first--;
						map.at(m.at(i).first).at(m.at(i).second)= MONSTER;

					}	
				}
			}
		}


	}
	void remove_monster(int x , int y){
	 pair <int,int> k = make_pair(x,y);
	 for (size_t i=0; i <m.size();i++){
		if (m.at(i).first==y && m.at(i).second==x){
	 		m.at(i).first=m.at(m.size()-1).first;
			m.at(i).second=m.at(m.size()-1).second;
			m.pop_back();
		}
			
	 }

	}
	void set(int x, int y, char c) {
		for (unsigned int i =0; i < SIZE;i++){
			for (unsigned int j =0; j < SIZE;j++){
				if (map.at(i).at(j)=='H')
					map.at(i).at(j)='.';
			}
		}

		map.at(y).at(x) = c;
	}
	char get(int x, int y) {
		return map.at(y).at(x);
	}
	Map() {
		init_map();
	}
};
