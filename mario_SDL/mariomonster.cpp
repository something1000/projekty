#include "mariomonster.h"



mariomonster::mariomonster(){
	monster = SDL_LoadBMP("monster_r.bmp");
	monster_l = SDL_LoadBMP("monster_l.bmp");
	if (monster == NULL) {
		printf("error: %s", SDL_GetError());
		SDL_Quit();
	}
	SDL_SetColorKey(monster, SDL_TRUE, 0xED1C24);
	SDL_SetColorKey(monster_l, SDL_TRUE, 0xED1C24);
	monster_bounds_x = monster->w;
	monster_bounds_y = monster->h;
	keyframe = 0;
}



mariomonster::~mariomonster(){

}

SDL_Surface* mariomonster::animate() {
	if (keyframe > 10) return monster_l;
	else return monster;
}

void mariomonster::drawMonsters(SDL_Surface* screen){
	for (int i = 0; i < num_of_monsters; i++) {
		DrawSurface(screen, animate(), monsters_list[i].pos_x, monsters_list[i].pos_y);
	}
	monsterWalking();
}

void mariomonster::setMonsterPositions(int index, int x, int y)
{
	monsters_list[index].pos_x = x;
	monsters_list[index].pos_y = y;
}

void mariomonster::deleteMonster() {
	SDL_FreeSurface(monster);
	SDL_FreeSurface(monster_l);
}
void mariomonster::setMonsterMaxMove(int index,int mv)
{
	monsters_list[index].max_move = mv;
	monsters_list[index].move = 0;
	monsters_list[index].move_right = 1;
}

void mariomonster::moveMonsters(int wek)
{
	for (int i = 0; i < num_of_monsters; i++) {
		monsters_list[i].pos_x -= wek;
	}
}

void mariomonster::setNumOfMonsters(int num)
{
	num_of_monsters = num;
}

void mariomonster::initMonsters(int n) {
	monsters_list = (monster_inf_t*)malloc(sizeof(monster_inf_t)*n);
	setNumOfMonsters(n);
}

int mariomonster::getMonsterPositionY(int index)
{
	return monsters_list[index].pos_y;
}
int mariomonster::getMonsterPositionX(int index)
{
	return monsters_list[index].pos_x;
}

int mariomonster::getMonsterBoundsX()
{
	return monster_bounds_x;
}

int mariomonster::getMonsterBoundsY()
{
	return monster_bounds_y;
}

int mariomonster::getMonsterDirection(int index)
{
	return monsters_list[index].move_right;
}

int mariomonster::getMonsterMove(int index)
{
	return monsters_list[index].move;
}

int mariomonster::getMonsterMaxMove(int index)
{
	return monsters_list[index].max_move;
}

void mariomonster::setMonsterMove(int index, int move)
{
	monsters_list[index].move = move;
}

void mariomonster::setMonsterDirection(int index, int dir)
{
	monsters_list[index].move_right = dir;
}

void mariomonster::clearList()
{
	free(monsters_list);
}

void mariomonster::monsterWalking()
{
	for (int i = 0; i < num_of_monsters; i++) {


		if (monsters_list[i].move_right == 1) {
			monsters_list[i].pos_x += MONSTER_SPEED;
			monsters_list[i].move += MONSTER_SPEED;
			if (monsters_list[i].move >= monsters_list[i].max_move) monsters_list[i].move_right = 0;
		}
		else {
			monsters_list[i].pos_x -= MONSTER_SPEED;
			monsters_list[i].move -= MONSTER_SPEED;
			if (monsters_list[i].move <=  0) monsters_list[i].move_right = 1;
		}

		
	}
	keyframe += 1;
	if (keyframe > 20) keyframe = 0;
}
