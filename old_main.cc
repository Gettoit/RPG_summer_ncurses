//Game made by Derek Lee
//Testers: Matt Bilello, Emmy Issa
//Special Thanks: Whoever the hell would be interesting in seeing this.
#include "map.h"
#include "game.h"
#include <unistd.h>
#include <chrono>
//Don't use cout or cin, that's why this is commented out
#include <iostream>
using namespace std::chrono;
using hrc = std::chrono::high_resolution_clock;



const unsigned int TIMEOUT = 10; //Milliseconds to wait for a getch to finish
const int UP = 65; //Key code for up arrow
const int DOWN = 66;
const int LEFT = 68;
const int RIGHT = 67;
//modify these 4 values to determine starting stats maybe give players the option to make their own stats?
const int STARTING_HEALTH = 0;
const int STARTING_MANA= 0;
const int STARTING_ATTACK= 0;
const int STARTING_SPEED= 0;
//remove these when hero class gets fully implimented
int player_health = 100;
int score = 0;
//remove to here
void turn_on_ncurses() {
	srand(time(0));
	initscr();//Start curses mode
	start_color(); //Enable Colors if possible
	init_pair(1, COLOR_GREEN, COLOR_BLACK); //Set up some color pairs
	init_pair(2, COLOR_CYAN, COLOR_BLUE);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_RED,COLOR_BLUE);
	init_pair(8,COLOR_BLACK,COLOR_BLACK);
	clear();
	noecho();
	cbreak();
	timeout(TIMEOUT); //Set a max delay for key entry
}
void exit_ncurses(){
	clear();
	endwin(); // End curses mode
	system("clear");
}
void move_floor();
void combat(int &x){
	player_health-= rand()%10 +1;
	if (x>0)
		x--;
}
void boss_combat(bool &b){
	player_health-= rand() %100 +1;
	b=true;	

}

//switch the combat to these when game is getting close to finishing
/*
void combat(Hero &player, int number_of enemies){
	exit_ncurses();
	turn_on_ncurses();
	

}
void boss_combat(Hero &player,int stage_level);
*/

void collect(){
	score += rand()%10+1;
}
void game_start (Map map,int x,int y, int floor_number, Hero &player);

void map_stuff(Map map,int &x,int &y, int floor_number, Hero &player){
	hrc::time_point old_time = hrc::now(); //Get the current time
	int counter =0;
	int monsters_left = 15;
	bool boss_dead = false;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q'){
			exit_ncurses();
			exit(1);
		}
		else if (ch == RIGHT) {
			x++;
			if (map.get(x,y) == Map::VWALL or map.get(x,y)==Map::HWALL) x--; 
			if (map.get(x,y) == Map::WATER) x--; 
			if (map.get(x,y) == Map::MONSTER){
				//comment out combat below when game is finished
				combat(monsters_left); 
				/*use this combat when game is done
				combat(player,(rand%3+1));
				*/
				map.remove_monster(x,y);
			}
			if (map.get(x,y) == Map::BOSS) boss_combat(boss_dead); 
			if (map.get(x,y) == Map::TREASURE) collect(); 
			if (map.get(x,y) == Map::HEALTH_UP) player_health+=10; 
			if (map.get(x,y) == Map::EXIT && !monsters_left) return; 
			if (map.get(x,y) == Map::EXIT && monsters_left) {
				x--;
			}
			if (x >= int(Map::SIZE)) x = Map::SIZE - 1; //Clamp value
		} else if (ch == LEFT) {
			x--;
			if (map.get(x,y) == Map::VWALL or map.get(x,y)==Map::HWALL) x++; 
			if (map.get(x,y) == Map::WATER) x++; 
			if (map.get(x,y) == Map::MONSTER){
				//comment out combat below when game is finished
				combat(monsters_left); 
				/*use this combat when game is done
				combat(player,(rand%3+1));
				*/
				map.remove_monster(x,y);
			}
			if (map.get(x,y) == Map::BOSS) boss_combat(boss_dead); 
			if (map.get(x,y) == Map::TREASURE) collect(); 
			if (map.get(x,y) == Map::HEALTH_UP) player_health+=10; 
			if (map.get(x,y) == Map::EXIT && !monsters_left) return; 
			if (map.get(x,y) == Map::EXIT && monsters_left) {
				x++;
			}
			if (x < 0) x = 0;
		} else if (ch == UP) {
			y--;
			if (map.get(x,y) == Map::VWALL or map.get(x,y)==Map::HWALL) y++; 
			if (map.get(x,y) == Map::WATER) y++; 
			if (map.get(x,y) == Map::MONSTER){
				//comment out combat below when game is finished
				combat(monsters_left); 
				/*use this combat when game is done
				combat(player,(rand%3+1));
				*/
				map.remove_monster(x,y);
			}
			if (map.get(x,y) == Map::BOSS) boss_combat(boss_dead); 
			if (map.get(x,y) == Map::TREASURE) collect(); //
			if (map.get(x,y) == Map::HEALTH_UP) player_health+=10; //
			if (map.get(x,y) == Map::EXIT && !monsters_left) return; 
			if (map.get(x,y) == Map::EXIT && monsters_left){
				y++;
			}
			if (y < 0) y = 0;
		} else if (ch == DOWN) {
			y++;
			if (map.get(x,y) == Map::VWALL or map.get(x,y)==Map::HWALL) y--; 
			if (map.get(x,y) == Map::WATER) y--; //
			if (map.get(x,y) == Map::MONSTER){
				//comment out combat below when game is finished
				combat(monsters_left); 
				/*use this combat when game is done
				combat(player,(rand%3+1));
				*/
				map.remove_monster(x,y);
			}
			if (map.get(x,y) == Map::BOSS) boss_combat(boss_dead);
			if (map.get(x,y) == Map::TREASURE)collect() ; //
			if (map.get(x,y) == Map::HEALTH_UP) player_health+=10; //
			if (map.get(x,y) == Map::EXIT && !monsters_left) return; 
			if (map.get(x,y) == Map::EXIT && monsters_left) {
				y--; 
			}
			if (y >= int(Map::SIZE)) y = Map::SIZE - 1; //Clamp value
		} else if (ch == ERR) { //No keystroke
			; //Do nothing
		}
		if (player_health<= 0){
			exit_ncurses();
			cout<<"You have Died...\n";
			exit(1);
		}
		//clear(); //Uncomment these lines if the code is drawing garbage
		if (counter>=50){
			map.monster_move();
			counter =0;
		}
		counter++;
		map.set(x,y,'H');
		map.draw(x, y);
		mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i\n", x, y);
		//usleep(10000); //pause for 10000 us
		//refresh(); //Uncomment these lines if the code is drawing garbage
		hrc::time_point new_time = hrc::now();
		duration<double> time_span = duration_cast<duration<double>>(new_time - old_time);
		
		mvprintw(Map::DISPLAY + 2, 0, "PLAYER HP: %i\n", player_health);
		mvprintw(Map::DISPLAY + 3, 0, "PLAYER SCORE: %i\n", score);
		mvprintw(Map::DISPLAY + 4, 0, "MONSTERS TO KILL: %i\n", monsters_left);
		if (!boss_dead)
			mvprintw(Map::DISPLAY +5,0,"There is still a tough enemy to fight");
		else
			mvprintw(Map::DISPLAY +5,0,"You have killed the Boss on this floor");

		if (monsters_left)
			mvprintw(Map::DISPLAY +6,0,"You need to slay more monsters");
		else 
			mvprintw(Map::DISPLAY +6,0,"You have slain all monsters necessary, you may now leave.");
		//mvprintw(Map::DISPLAY + 4, 0, "FPS: %5.1f\n", 1 / time_span.count());

		old_time = new_time;
	}
}
void move_floor(int floor_number, Hero &player){
	Map map;
	game_start(map,1,1, floor_number,player);
	

}
void game_start (Map map,int x,int y, int floor_number, Hero &player){
	while (true){
		if (floor_number>=3){
			map_stuff(map,x,y,floor_number,player);
			exit_ncurses();
			cout<<"You Win!\n";
			exit(0);
		}
		map_stuff(map,x,y,floor_number,player);
		floor_number++;
		move_floor(floor_number,player);
		exit_ncurses();
		turn_on_ncurses();
	}
}
int main() {
#ifndef MADE_USING_MAKEFILE
	static_assert(0, "Compile this code using 'make' not 'compile.");
#endif	
//	hrc::time_point old_time = hrc::now(); //Get the current time

	cout<<"Input your name player!\n";
	string name;
	cin>>name;
	Hero player(name,STARTING_HEALTH,STARTING_MANA,STARTING_ATTACK,STARTING_SPEED);
	turn_on_ncurses(); //Turn on full screen mode
	Map map;
	int floor_number=1;
	int x = 1, y = 1; //Start in middle of the world
	game_start(map,x,y,floor_number,player);
	exit_ncurses();
}
