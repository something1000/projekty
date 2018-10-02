#pragma once
#include "SDL_inc.h"

#include "mariolevel.h"

enum mariostates_t {
	in_jump,
	falling_down,
	standing,
	dead,
	winner
};
enum mariodirection_t {
	right,
	left,
	up,
	def
};

#ifdef __cplusplus
extern "C"
#endif

#define animationFreq 0.2

class mariohero
{

public:
	mariohero(mariolevel* lvl);
	~mariohero();
	SDL_Surface *getHero();
	void setMarioDirection(mariodirection_t dir);// 
	void setMarioPosX(int pos);
	void setMarioPosY(int pos);
	void addMarioPosX(double wek, mariolevel* lvl);
	void addMarioPosY(double wek);
	int getMarioPosX();
	int getMarioPosY();
	mariostates_t getMarioState();
	void deleteHero();
	void draw(SDL_Surface* hero);
	void setMarioState(mariostates_t state);
	bool collisionX(mariolevel* level, int position);
	bool collisionY(mariolevel* level, int position);
	void saveGame(mariolevel * lvl, int timeleft);
	bool checkCollisionY(double,mariolevel*);
	bool checkCollisionX(double, mariolevel*);
	bool checkMonsterCollision(double move, mariolevel * level);

	int getCoins();
	void addCoin();
	void addLife(int i);
	int getLifes();
	void reborn(mariolevel* level);
	void loadLevel(mariolevel* lvl);
private:
	int coins;
	int lifesLeft;
	double run;
	int pos_y;
	int pos_x;
	int bounds_x;
	int bounds_y;
	int initHero();
	SDL_Surface *hero = NULL;
	SDL_Surface *hero_r2 = NULL;
	SDL_Surface *hero_r = NULL;
	SDL_Surface *hero_l2 = NULL;
	SDL_Surface *hero_l = NULL;
	SDL_Surface *hero_up = NULL;
	mariostates_t state;
	mariodirection_t direction;
};

