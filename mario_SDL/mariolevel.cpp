#include "mariolevel.h"

mariolevel::mariolevel(int lvl){
	initBlockSurface();
	initBlocksBounds();
	loadLevel(lvl);
	sumPrzes = 0;
}


mariolevel::~mariolevel(){
	currentLevel = 1;
	//free(monster)
	//SDL_FreeSurface(monster_sur);
	/*SDL_FreeSurface(floor_b);
	SDL_FreeSurface(coin_b);
	SDL_FreeSurface(breakable_b);
	SDL_FreeSurface(solid_b);
	SDL_FreeSurface(castle);*/
}

void mariolevel::initBlockSurface() {
	//monster_sur = SDL_LoadBMP("monster.bmp");
	floor_b = SDL_LoadBMP("floor.bmp");
	coin_b = SDL_LoadBMP("coin.bmp");
	breakable_b = SDL_LoadBMP("breakable_block.bmp");
	solid_b = SDL_LoadBMP("solid.bmp");
	castle = SDL_LoadBMP("castle.bmp");
	if (floor_b == NULL || coin_b == NULL || breakable_b == NULL || solid_b == NULL || castle == NULL) {
		printf("Error: %s", SDL_GetError());
		SDL_Quit();
	}
	SDL_SetColorKey(castle, SDL_TRUE, 0x00ff00);
}
void mariolevel::initBlocksBounds() {
	block_sx = floor_b->w;
	block_sy = floor_b->h;
	castle_sx = castle->w;
}
int mariolevel::getPos_x(int i)
{
	return positions[i].pos_x;
}
int mariolevel::getPos_y(int i)
{
	return positions[i].pos_y;
}
int mariolevel::getBType(int i)
{
	return positions[i].type;
}
void mariolevel::initBlockPositions(int n_of_blocks){
	num_of_blocks = n_of_blocks;
	positions = (blocks_pos*)malloc(n_of_blocks * sizeof(blocks_pos));
}

bool mariolevel::deleteBlock(int i) {
	if (positions[i].type == breakable) {
		positions[i].pos_x = 0;
		positions[i].pos_y = SCREEN_HEIGHT + block_sy;
	}
	else if (positions[i].type == coin) {
		positions[i].type = solid;
		return true;
	}
	return false;
}
int mariolevel::getNumOfMonsters()
{
	return num_of_monsters;
}
int mariolevel::getMonsterPosX(int index)
{
	return monster.getMonsterPositionX(index);
}
int mariolevel::getMonsterPosY(int index)
{
	return monster.getMonsterPositionY(index);
}
int mariolevel::getMonsterBoundsX()
{
	return monster.getMonsterBoundsX();
}
int mariolevel::getMonsterBoundsY()
{
	return monster.getMonsterBoundsY();
}
int mariolevel::getMonsterMove(int index)
{
	return monster.getMonsterMove(index);
}
int mariolevel::getMonsterDirection(int index)
{
	return monster.getMonsterDirection(index);
}
void mariolevel::drawBlocks(SDL_Surface* screen){
	for (int i = 0; i < num_of_blocks; i++) {
		switch (positions[i].type) {
		case solid:
			DrawSurface(screen, solid_b, positions[i].pos_x, positions[i].pos_y);
			break;
		case floor_block:
			DrawSurface(screen, floor_b, positions[i].pos_x, positions[i].pos_y);
			break;
		case breakable:
			DrawSurface(screen, breakable_b, positions[i].pos_x, positions[i].pos_y);
			break;
		case coin:
			DrawSurface(screen, coin_b, positions[i].pos_x, positions[i].pos_y);
			break;
		case end_level:
			DrawSurface(screen, castle, positions[i].pos_x , positions[i].pos_y - castle->h/2 - floor_b->h/2);
		}

	}
		
}



int mariolevel::getNumOfBlocks(){
	return num_of_blocks;
}

int mariolevel::getBSizeY()
{
	return block_sy;
}

int mariolevel::getBSizeX()
{
	return block_sx;
}

void mariolevel::moveBlocks(int wekt){
	if(sumPrzes + wekt>=0){
	for (int i = 0; i < num_of_blocks; i++) {
		positions[i].pos_x -= wekt;
	}
		monster.moveMonsters(wekt);

	sumPrzes += wekt;
	}
	else sumPrzes = 0;
	
}

int mariolevel::getSumPrzes()
{
	return sumPrzes;
}
void mariolevel::setSumPrzes(int przes) {
	sumPrzes = przes;
}

int mariolevel::getCurrentLevel() {
	return currentLevel;
}

void mariolevel::setCurrentLevel(int i){	
	currentLevel = i;
}

void mariolevel::resetSumPrzes()
{
	moveBlocks(-sumPrzes);
	sumPrzes = 0;
}

void mariolevel::minusTimeLeft(){
	time_left--;
}

int mariolevel::getTimeLeft()
{
	return time_left;
}

int mariolevel::getMonsterMaxMove(int index)
{
	return monster.getMonsterMaxMove(index);
}

void mariolevel::draw(SDL_Surface * screen)
{
	drawBlocks(screen);
	drawMonsters(screen);
}

SDL_Surface* mariolevel::getMonsterSur()
{
	return monster_sur;
}

void mariolevel::drawMonsters(SDL_Surface* screen)
{
		monster.drawMonsters(screen);
}

int mariolevel::getCastleBound()
{
	return castle_sx;
}

void mariolevel::clearLevel() {
	monster.clearList();
	free(positions);
	num_of_blocks = 0;
	num_of_monsters = 0;
	sumPrzes = 0;
}

bool mariolevel::setLevel()
{
	clearLevel();
	currentLevel++;
	return loadLevel(currentLevel);
}

void mariolevel::resetTimeLeft()
{
	time_left = level_time;
}

void mariolevel::deleteLevel(){
	//monster.clearList();
	monster.deleteMonster();
	//free(positions);
	SDL_FreeSurface(floor_b);
	SDL_FreeSurface(coin_b);
	SDL_FreeSurface(breakable_b);
	SDL_FreeSurface(solid_b);
	SDL_FreeSurface(castle);

}



void mariolevel::loadMonsters(FILE ** plik){
	int num1 =0;
	int num2 = 0; 
	int num3 = 0;
	int num4 = 0;
	int num5 = 0;

	fscanf(*plik, " %d", &num1);
	if (num1 == EOF) {
		num_of_monsters = 0;
	}
	else {
		num_of_monsters = num1;
	}
	monster.initMonsters(num_of_monsters);
	
	for (int i = 0; i < num_of_monsters; i++) {
		fscanf(*plik, " %d %d %d %d %d ", &num1, &num2,&num3,&num4,&num5);
		//mnstr[i].~mariomonster();
		monster.setMonsterPositions(i, block_sx*num2, (SCREEN_HEIGHT*(1 - BORDERHEIGHT) - block_sy/2) - num1*block_sy);
		monster.setMonsterMaxMove(i, num3);
		monster.setMonsterMove(i, num4);
		monster.setMonsterDirection(i, num5);
	}

}

bool mariolevel::loadLevel(int lvl){
	char nazwa[10] = "etap";
	FILE* level = NULL;

	if (lvl == -1) {
		strcpy(nazwa, "saved");
		level = fopen(strcat(nazwa, ".txt"), "r");
		if (level == NULL) {
			printf("jestem tutaj");
			return false;
		}
		clearLevel();

	} else {
		char lvlBuf[3];
		currentLevel = lvl;
		sprintf(lvlBuf, "%d", lvl);
		strcat(nazwa, lvlBuf);
		level = fopen(strcat(nazwa,".txt"), "r");}

		if (level == NULL) {
			printf("jestem tutaj");
			return false;
		}
	char buf;
	int num = 0;
	fscanf(level, "H %d ", &num);
	hero_startY = (SCREEN_HEIGHT*(1 - BORDERHEIGHT) - block_sy / 2) - num*block_sy;
	fscanf(level, " %d ", &num);
	hero_startX = block_sx*num;
	//fscanf(level, " %d ", &hero_startY);
	//fscanf(level, " %d", &hero_startX);

	fscanf(level, "T %d ", &level_time);
	//fscanf(level, " %d", &num);
	time_left = level_time;
	fscanf(level, " %d", &num);
	initBlockPositions(num);
	for (int i = 0; i < num_of_blocks; i++) {
		fscanf(level, " %c ", &buf);
		switch (buf) {
		case 'F':
			positions[i].type = floor_block;
			break;
		case 'B':
			positions[i].type = breakable;
			break;
		case 'C':
			positions[i].type = coin;
			break;
		case 'S':
			positions[i].type = solid;
			break;
		case 'E':
			positions[i].type = end_level;
			break;
		}
		fscanf(level, " %d", &num);
		positions[i].pos_y = (SCREEN_HEIGHT*(1- BORDERHEIGHT) - block_sy/2) - num*block_sy;
		fscanf(level, " %d", &num);
		positions[i].pos_x = block_sx*num;
	}
	loadMonsters(&level);

	if(lvl==-1){
	fscanf(level, " saveInfo: ");

	fscanf(level, " Poz: %d ", &num);
	posYLoad = (SCREEN_HEIGHT*(1 - BORDERHEIGHT) - block_sy / 2) - num*block_sy;
	fscanf(level, " %d ", &num);
	posXLoad = block_sx*num;

	fscanf(level, " Coins: %d", &coinsLoad);
	fscanf(level, " Przes: %d", &num);
	setSumPrzes(num);
	fscanf(level, " Level: %d", &num);
	setCurrentLevel(num);
	fscanf(level, " Lifes: %d", &lifesLoad);
	fscanf(level, " TLeft: %d", &num);
	time_left = num;
}

	fclose(level);
	return true;
}

int mariolevel::getMarioStartX()
{
	return hero_startX;
}

int mariolevel::getMarioStartY()
{
	return hero_startY;
}

int mariolevel::getLoadedCoins()
{
	return coinsLoad;
}

int mariolevel::getLoadedLifes()
{
	return lifesLoad;
}

int mariolevel::getLevelTime()
{
	return level_time;
}

int mariolevel::getPosXLoad()
{
	return posXLoad;
}

int mariolevel::getPosYLoad()
{
	return posYLoad;
}

