//RPG Made by Derek Lee
#include "map.h"
#include <bits/stdc++.h> 
#include <unistd.h>
#include <chrono>
using namespace std::chrono;
using hrc = std::chrono::high_resolution_clock;

const unsigned int TIMEOUT = 10;
const int UP = 65; //Key code for up arrow (also use WASD)
const int DOWN = 66; //code for down 
const int LEFT = 68; //code for left
const int RIGHT = 67; //code for right



void ncurses_on(){
	srand(time(0));
	initscr();//starts ncurses
	start_color();//setting colors
	init_pair(1,COLOR_GREEN, COLOR_BLACK);//setting color pairs
	init_pair(2,COLOR_CYAN, COLOR_BLUE);
	init_pair(3,COLOR_GREEN, COLOR_BLACK);
	init_pair(4,COLOR_YELLOW, COLOR_BLACK);
	init_pair(5,COLOR_RED, COLOR_BLACK);
	init_pair(6,COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7,COLOR_RED, COLOR_BLUE);
	init_pair(8,COLOR_BLACK, COLOR_BLACK);
	init_pair(9,COLOR_GREEN, COLOR_BLACK);
	init_pair(10,COLOR_WHITE,COLOR_BLACK);
	clear();
	noecho();
	cbreak();
	timeout(TIMEOUT); //delay for a keystroke

}


void ncurses_off(){
	clear();
	endwin();//ends ncruses;
	system("clear");
}
//TODO: make combat good it will require some stuff to happen


void game_start(){
	vector<vector<vector<Game_map>>> board;
	int ladder_down_x = 0;
	int ladder_down_y = 0;
	const int FLOOR_COUNT = 4;
	const int BOARD_SIZE = 10;
	board.resize(FLOOR_COUNT);
	for (int k = 0; k< 4; k++){
		vector<vector<Game_map>> temp;
		
		for(int i = 0; i < BOARD_SIZE; i++){
			vector<Game_map> temp2;
			for(int j = 0; j < BOARD_SIZE; j++){
				Game_map k;
				temp2.push_back(k);
			}
			temp.push_back(temp2);
		}
		board.at(k)=temp;
	}
	for (int i = 0; i < FLOOR_COUNT; i++){
		//temp variables to see if boss is in location
		int temph = (rand()%(Game_map::SIZEH -1))+1;
		int temp = (rand()%(Game_map::SIZE -1))+1;
		//spawns boss at a random location on the floor
		board.at(i).at(rand()%(BOARD_SIZE -1)).at(rand()%(BOARD_SIZE -1)).spawn_boss();
		
		//ladder up
		if (i!=0){
			board.at(i).at(rand()%(BOARD_SIZE -1)).at(rand()%(BOARD_SIZE -1)).spawn_ladder_up(ladder_down_x,ladder_down_y);
			
		}
		//spawns a ladder down at a random spot on the floor (handles colision with boss)
		if (i!=FLOOR_COUNT-1){
			if (board.at(i).at(rand()%(BOARD_SIZE-1)).at(rand()%(BOARD_SIZE-1)).get_current_loc(temph,temp)==Game_map::BOSS){

			}
			else{
				board.at(i).at(rand()%(BOARD_SIZE -1)).at(rand()%(BOARD_SIZE -1)).spawn_ladder_down(temph,temp);
				ladder_down_x = temph;
				ladder_down_y = temp;
			}
		}

	}
	string test;
	hrc::time_point old_time = hrc::now();
	while (true){
		mvprintw(0,0,"Please Enter Your Name\n");
		//can use getstr() but not sure how big the names should be use vector for being safe
		int ch = getch();
		if (ch == 13 or ch == 10)
			break;
		//do nothing
		else if(ch==ERR){;}
		else
			test.push_back(char (ch));
	}
	system("clear");
	
	const int n = test.length();
	char name[n+1];
	strcpy(name,test.c_str());
	for (size_t i = 0; i < test.size(); i ++){
		name[i] = test.at(i);
	}
	Hero hero(test,100,10,10,10);
	int x = 1, y = 1;
	int level = 0, board_x=0,board_y=0;
	hero.set_loc(x,y);
	char c = 'H';
	while (true){
		int ch= getch();
		if (ch == 'q' or ch == 'Q'){
			ncurses_off();
			exit(1);
		}
		else if (ch==RIGHT){
			x++;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WALL) x--;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WATER) x--;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::GRASS){
				if (board.at(level).at(board_y).at(board_x).check_monster(x,y)){
					board.at(level).at(board_y).at(board_x).combat(hero);
					ncurses_off();
					ncurses_on();
			}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::EXIT){
				if (x==0){
					board_x--;
					if (board_x<0)
						board_x=BOARD_SIZE-2;
					x=Game_map::SIZEH-2;

				}
				else if (x==Game_map::SIZEH-1){
					board_x++;
					if (board_x>BOARD_SIZE-1)
						board_x=0;
					x=1;
				}
				else if (y==0){
					board_y--;
					if (board_y<0)
						board_y=BOARD_SIZE-2;
					y=Game_map::SIZE-2;
				}
				else if (y==Game_map::SIZE-1){
					board_y++;
					if (board_y>BOARD_SIZE-1)
						board_y=0;
					y=1;
				}
			}
			
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::BOSS)
				;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_DOWN){
				level++;
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_UP){
				level--;
			}
			
			hero.set_loc(x,y);

		}
		else if (ch==LEFT){
			x--;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WALL) x++;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WATER) x++;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::GRASS){
			//	if (board.check_grass(x,y) == true)
				//do combat in map.h
				if (board.at(level).at(board_y).at(board_x).check_monster(x,y)){
					board.at(level).at(board_y).at(board_x).combat(hero);
					ncurses_off();
					ncurses_on();
			}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_DOWN){
				level++;
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_UP){
				level--;
			}
			
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::EXIT){
				if (x==0){
					board_x--;
					if (board_x<0)
						board_x=BOARD_SIZE-2;
					x=Game_map::SIZEH-2;
				}
				if (x==Game_map::SIZEH-1){
					board_x++;
					if (board_x>BOARD_SIZE-1)
						board_x=0;
					x=1;
				}
				if (y==0){
					board_y--;
					if (board_y<0)
						board_y=BOARD_SIZE-2;
					y=Game_map::SIZE-2;
				}
				if (y==Game_map::SIZE-1){
					board_y++;
					if (board_y>BOARD_SIZE-1)
						board_y=0;
					y=1;
				}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::BOSS)
				;
			
			hero.set_loc(x,y);

		}
		else if (ch==UP){
			y--;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WALL) y++;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WATER) y++;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::GRASS){
			//	if (board.check_grass(x,y) == true)
				//do combat in map.h
//					combat(hero);
				if (board.at(level).at(board_y).at(board_x).check_monster(x,y)){
					board.at(level).at(board_y).at(board_x).combat(hero);
					ncurses_off();
					ncurses_on();
				}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_DOWN){
				level++;
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_UP){
				level--;
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::EXIT){
				if (x==0){
					board_x--;
					if (board_x<0)
						board_x=BOARD_SIZE-2;
					x=Game_map::SIZEH-2;
				}
				if (x==Game_map::SIZEH-1){
					board_x++;
					if (board_x>BOARD_SIZE-1)
						board_x=0;
					x=1;
				}
				if (y==0){
					board_y--;
					if (board_y<0)
						board_y=BOARD_SIZE-2;
					y=Game_map::SIZE-2;
				}
				if (y==Game_map::SIZE-1){
					board_y++;
					if (board_y>BOARD_SIZE-1)
						board_y=0;
					y=1;
				}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::BOSS)
				;
			
			hero.set_loc(x,y);

		}
		else if (ch==DOWN){
			y++;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WALL) y--;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::WATER) y--;
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::GRASS){
			//	if (board.check_grass(x,y) == true)
				//do combat in map.h
//					combat(hero);
				if (board.at(level).at(board_y).at(board_x).check_monster(x,y)){
					board.at(level).at(board_y).at(board_x).combat(hero);
					ncurses_off();
					ncurses_on();
				}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_DOWN){
				level++;
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::LADDER_UP){
				level--;
			}
			
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::EXIT){
				if (x==0){
					board_x--;
					if (board_x<0)
						board_x=BOARD_SIZE-2;
					x=Game_map::SIZEH-2;
				}
				if (x==Game_map::SIZEH-1){
					board_x++;
					if (board_x>BOARD_SIZE-1)
						board_x=0;
					x=1;
				}
				if (y==0){
					board_y--;
					if (board_y<0)
						board_y=BOARD_SIZE-2;
					y=Game_map::SIZE-2;
				}
				if (y==Game_map::SIZE-1){
					board_y++;
					if (board_y>BOARD_SIZE-1)
						board_y=0;
					y=1;
				}
			}
			if (board.at(level).at(board_y).at(board_x).get_current_loc(x,y) == Game_map::BOSS)
				;
			
			hero.set_loc(x,y);

		}
		
		mvprintw(Game_map::SIZE,0,"Character name: ");
		mvprintw(Game_map::SIZE,16,name);
		mvprintw(Game_map::SIZE +1, 0, "Player level: ");
		mvprintw(Game_map::SIZE +1, 14,"%d",hero.get_level());
		mvprintw(Game_map::SIZE +2,0, "Current Health: ");
		mvprintw(Game_map::SIZE +2,16, "%d", hero.get_health());
		mvprintw(Game_map::SIZE +2,20, "/ ");
		mvprintw(Game_map::SIZE +2,22, "%d",hero.get_max_health());

		board.at(level).at(board_y).at(board_x).set_player_loc(x,y,c);
		board.at(level).at(board_y).at(board_x).draw(x,y);
	}


}

int main(){
	ncurses_on();
	game_start();
}
