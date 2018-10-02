#include "mariohero.h"
#include <stdio.h>
#include "mariolevel.h"
mariohero::mariohero(mariolevel* lvl){
	pos_x = lvl->getMarioStartX();
	pos_y = lvl->getMarioStartY();
	state = falling_down;
	run = 1;
	coins = 0;
	lifesLeft = 3;
	hero = NULL;

	initHero();

}


mariohero::~mariohero()
{
	//deleteHero();
}

void mariohero::deleteHero() {
	SDL_FreeSurface(hero);
	SDL_FreeSurface(hero_r);
	SDL_FreeSurface(hero_r2);
	SDL_FreeSurface(hero_l);
	SDL_FreeSurface(hero_l2);
	SDL_FreeSurface(hero_up);
}
void mariohero::draw(SDL_Surface* screen) {
	DrawSurface(screen, getHero(), getMarioPosX(), getMarioPosY());
}
int mariohero::initHero() {
	hero = SDL_LoadBMP("eti.bmp");
	SDL_SetColorKey(hero, SDL_TRUE, 0x00000);
	hero_r = SDL_LoadBMP("mario_r.bmp");
	hero_r2 = SDL_LoadBMP("mario_r2.bmp");
	hero_l = SDL_LoadBMP("mario_l.bmp");
	hero_l2 = SDL_LoadBMP("mario_l2.bmp");
	hero_up = SDL_LoadBMP("mario_up.bmp");
	if (hero == NULL || hero_r == NULL || hero_r2 == NULL || hero_l == NULL || hero_l2 == NULL || hero_up == NULL) {
	printf("Nie zaladowano pliku: %s", SDL_GetError);
	return 0;
	}

	else {
		SDL_SetColorKey(hero, SDL_TRUE, 0x00000);
		SDL_SetColorKey(hero_r, SDL_TRUE, 0x00000);
		SDL_SetColorKey(hero_r2, SDL_TRUE, 0x00000);
		SDL_SetColorKey(hero_l, SDL_TRUE, 0x00000);
		SDL_SetColorKey(hero_l2, SDL_TRUE, 0x00000);
		SDL_SetColorKey(hero_up, SDL_TRUE, 0x00000);
		bounds_x = hero->w;
		bounds_y = hero->h;
		return 1;
	}
}

SDL_Surface* mariohero::getHero() {
	switch (direction) {
		case right:
			switch ((int)run) {
			default:
				run = 1;
			case 1:
				run += animationFreq;
				return hero_r;
				break;
			case 2:
				run += animationFreq;
				return hero_r2;
				break;	
			}	
		case left:
			switch ((int)run) {
			default:
				run = 1;
			case 1:
				run += animationFreq;
				return hero_l;
				break;
			case 2:
				run += animationFreq;
				return hero_l2;
				break;
			}
		case up:
			return hero_up;
			break;
	}
	return hero;
}

void mariohero::setMarioDirection(mariodirection_t dir){
	direction = dir;
}



void mariohero::setMarioPosX(int pos){
	pos_x = pos;
}

void mariohero::setMarioPosY(int pos){
	pos_y = pos;
}

void mariohero::addMarioPosX(double wek,mariolevel* level){
	if (pos_x + wek < 10 || pos_x + wek > SCREEN_WIDTH/2 || level->getSumPrzes()>0) {
		level->moveBlocks(wek);
		return;
	}
	else {
		pos_x += wek;
	}
}

void mariohero::addMarioPosY(double wek) {
	pos_y += wek;
	if (pos_y +bounds_x > SCREEN_HEIGHT*(1-BORDERHEIGHT)) {
		state = dead;
	}
}
int mariohero::getMarioPosX()
{
	return pos_x;
}

int mariohero::getMarioPosY()
{
	return pos_y;
}

mariostates_t mariohero::getMarioState()
{
	return state;
}

void mariohero::setMarioState(mariostates_t m_state){
	state = m_state;
}


bool mariohero::checkCollisionY(double move_y, mariolevel* level) {
	for (int i = 0; i < level->getNumOfBlocks(); i++) {
		if ((pos_x+(bounds_x/2) > level->getPos_x(i) - (level->getBSizeX() / 2)) && (pos_x- (bounds_x / 2) < level->getPos_x(i) + (level->getBSizeX() / 2))) {

			if(pos_y < level->getPos_y(i)){
			if (pos_y+(bounds_y/2) + move_y >= level->getPos_y(i) - (level->getBSizeY()/2)) {
				pos_y = level->getPos_y(i) - (level->getBSizeY() / 2) - bounds_y/2;
				return true;
				}

			} else {
				if (pos_y - (bounds_y / 2) + move_y <= level->getPos_y(i) + (level->getBSizeY() / 2)) {
					if (level->deleteBlock(i)) coins++;
					return true;
				}
			}
		}
	
	}
	return false;
}
bool mariohero::checkCollisionX(double move_x, mariolevel* level) {
	for (int i = 0; i < level->getNumOfBlocks(); i++) {
		if (level->getBType(i) == end_level) {
			if (pos_x > level->getPos_x(i) - level->getCastleBound() / 2) {
				state = winner;
			}
		}
		if ( pos_y -bounds_y/2 < level->getPos_y(i) + (level->getBSizeY()/2 ) &&
			 pos_y + bounds_y/2 > level->getPos_y(i) - (level->getBSizeY() / 2)) {
			
			if(move_x>=0){
				if (pos_x+(bounds_x/2) + move_x >= level->getPos_x(i) - (level->getBSizeX() / 2) && pos_x -bounds_x/2 < level->getPos_x(i)+(level->getBSizeX()/2)) {
					
					return true;
				}
			}
			else {
				if (pos_x - (bounds_x / 2) + move_x <= level->getPos_x(i) + (level->getBSizeX() / 2) && pos_x + bounds_x/2 > level->getPos_x(i) - (level->getBSizeX() / 2)) {
					return true;
				}
			}

		}
	}

	return false;
}

bool mariohero::checkMonsterCollision(double move, mariolevel* level) {
	for (int i = 0; i < level->getNumOfMonsters(); i++) {
		if ((pos_y + bounds_y / 2) > level->getMonsterPosY(i)-level->getMonsterBoundsY()/2 && (pos_y - bounds_y / 2) < level->getMonsterPosY(i)+ level->getMonsterBoundsY()/2) {

			/*if (pos_x+(bounds_x/2)  >= level->getMonsterPosX(i) - level->getMonsterBoundsX()/2 &&
				pos_x-(bounds_x/2)  <= level->getMonsterPosX(i) - level->getMonsterBoundsX() / 2) {
				return true;
			}*/

			if (pos_x + (bounds_x/2) >= level->getMonsterPosX(i) - level->getMonsterBoundsX()/2 && 
				pos_x - (bounds_x/2) < level->getMonsterPosX(i) + level->getMonsterBoundsX()/2 ) {
				return true;
			}

		}
	}
	return false;
}

int mariohero::getCoins()
{
	return coins;
}

void mariohero::addCoin()
{
	coins++;
}

void mariohero::addLife(int i){
	lifesLeft += i;
}

int mariohero::getLifes()
{
	return lifesLeft;
}

void mariohero::reborn(mariolevel* level){
	pos_x = level->getMarioStartX();
	pos_y = level->getMarioStartY();
	level->resetSumPrzes();
}


bool mariohero::collisionX(mariolevel* level, int position){

	if (pos_x < level->getPos_x(position+1)) {
		if (pos_y < level->getPos_y(position + 1)-level->getBSizeY()) {
			if (pos_y > level->getPos_y(position + 1) + level->getBSizeY()) {
				return true;
			}
			
		}
				
	}

	return false;
}

bool mariohero::collisionY(mariolevel* level, int position) {
	
	for (int i = 0; i < level->getNumOfBlocks(); i++) {
		//bounds_x/2
		if (pos_x > level->getPos_x(i) - (level->getBSizeX()/2)) {

			if (pos_x < level->getPos_x(i) + (level->getBSizeX()/2)) {

				if (pos_y >= level->getPos_y(i) - level->getBSizeY()) return false;
			}
		}

	}

	/*
	if (((pos_y == (level->getPos_y(position) - (level->getBSizeY() / 2)))) && (pos_x > level->getPos_x(position) - level->getBSizeX() && pos_x < level->getPos_x(position) + level->getBSizeX() )) {
			return true;
		}*/
	return true;
}

void mariohero::saveGame(mariolevel* lvl,int timeleft) {
	FILE* plik = NULL;
	plik = fopen("saved.txt", "w");
	if (plik == NULL) return ;
	char znak = ' ';
	int pozycjay = 0;
	int pozycjax = 0;

	pozycjay = ((SCREEN_HEIGHT*(1 - BORDERHEIGHT) - lvl->getBSizeY() / 2) - lvl->getMarioStartY()) / lvl->getBSizeY();
	pozycjax = lvl->getMarioStartX() / lvl->getBSizeX();
	fprintf(plik, "H %d %d\n", pozycjay,pozycjax);
	fprintf(plik, "T %d\n", lvl->getLevelTime());
	fprintf(plik, "%d\n", lvl->getNumOfBlocks());

	char block_type = NULL;
	for (int i = 0; i < lvl->getNumOfBlocks(); i++) {
		lvl->getBType(i);
		switch (lvl->getBType(i)) {
		case floor_block:
			block_type = 'F';
			break;
		case breakable:
			block_type = 'B';
			break;
		case solid:
			block_type = 'S';
			break;
		case coin:
			block_type = 'C';
			break;
		case end_level:
			block_type = 'E';
			break;
		}
		//positions[i].pos_y = (SCREEN_HEIGHT*(1 - BORDERWIDTHHOR) - block_sy / 2) - num*block_sy;
		pozycjay = ((SCREEN_HEIGHT*(1 - BORDERHEIGHT) - lvl->getBSizeY() / 2) - lvl->getPos_y(i)) / lvl->getBSizeY();
		pozycjax = lvl->getPos_x(i) / lvl->getBSizeX();
		fprintf(plik, "%c %d %d\n", block_type,pozycjay, pozycjax);
	}

	fprintf(plik, "%d\n", lvl->getNumOfMonsters());
	for (int i = 0; i < lvl->getNumOfMonsters(); i++) {

		pozycjax = lvl->getMonsterPosX(i)/lvl->getBSizeX();
		pozycjay = ((SCREEN_HEIGHT*(1 - BORDERHEIGHT) - lvl->getBSizeY() / 2) - lvl->getMonsterPosY(i))/lvl->getBSizeY();
		fprintf(plik, "%d %d %d %d %d\n", pozycjay, pozycjax, lvl->getMonsterMaxMove(i),lvl->getMonsterMove(i),lvl->getMonsterDirection(i));
	}
	fprintf(plik, "saveInfo: \n");

	pozycjay = ((SCREEN_HEIGHT*(1 - BORDERHEIGHT) - lvl->getBSizeY() / 2) - pos_y) / lvl->getBSizeY();
	pozycjax = pos_x / lvl->getBSizeX();
	fprintf(plik, "Poz: %d %d\n", pozycjay, pozycjax);
	fprintf(plik, "Coins: %d\n",coins);
	fprintf(plik, "Przes: %d\n",lvl->getSumPrzes());
	fprintf(plik, "Level: %d\n",lvl->getCurrentLevel());
	fprintf(plik, "Lifes: %d\n", lifesLeft);
	fprintf(plik, "TLeft: %d\n", lvl->getTimeLeft());
	fclose(plik);
}

void mariohero::loadLevel(mariolevel* lvl) {
	if(lvl->loadLevel(-1)){
	pos_x = lvl->getPosXLoad();
	pos_y = lvl->getPosYLoad();
	coins = lvl->getLoadedCoins();
	lifesLeft = lvl->getLoadedLifes();}
	/*FILE* plik = NULL;
	plik = fopen("saved.txt", "r");
	if (plik == NULL) {
		printf("sprwadzam");
	}
	int buf = 0;
	fscanf(plik, "S: %d ", &buf);
	fscanf(plik, "Coins: %d", &coins);
	fscanf(plik, "Przes: %d", &buf);
	lvl->setSumPrzes(buf);
	fscanf(plik, "Level: %d", &buf);
	lvl->setCurrentLevel(buf);
	fclose(plik);*/
}