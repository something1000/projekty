#include "mariogame.h"



mariogame::mariogame(){
	initBorders();
	initialized = false;
	lastSec = 0;
}

mariogame::~mariogame(){
}

//Initialize SDL
bool mariogame::initWindow() {
	int err = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 0;
	}
	else {
		window = NULL;
		err = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
		SDL_SetWindowTitle(window,"Bartlomiej Gawrych s165619");
		if (err != 0) {
			printf("SDL_Init error: %s\n", SDL_GetError());
			return 0;
		}
		else {
			screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
				0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
			scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_ShowCursor(SDL_DISABLE);
			SDL_SetColorKey(charset, true, 0x000000);
		}
	}
}

bool mariogame::counterFPS() {
	tick2 = SDL_GetTicks();
	delta = (tick2 - tick1) *0.001;

	if (delta < 0.033) return false;

	tick1 = tick2;
	worldtime += delta;
	fpstimer += delta;

	if (fpstimer > 0.5) {
		fps = frames * 2;
		frames = 0;
		fpstimer -= 0.5;
	};
	frames++;
	return true;
}


void mariogame::endGame(mariolevel* level, mariohero* hero) {
	hero->~mariohero();
	start();
}
void mariogame::newGame(mariolevel* level, mariohero* hero) {
	hero->~mariohero();
	level->~mariolevel();
	start();
}
void mariogame::drawCommunicat(SDL_Surface* screen, const char* txttodraw) {
	SDL_Rect communicat;
	communicat.x = SCREEN_WIDTH*0.20;
	communicat.y = SCREEN_HEIGHT*0.4;
	communicat.w = SCREEN_WIDTH*0.60;
	communicat.h = SCREEN_HEIGHT*0.2;
	SDL_FillRect(screen, &communicat,0xff0000);
	sprintf(text, txttodraw);
	DrawString(screen, SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.5, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	SDL_Delay(3000);

}

void mariogame::initBorders() {
	borders[0].x = 0;								borders[0].y = SCREEN_HEIGHT*(1-BORDERHEIGHT);	borders[0].h = SCREEN_HEIGHT;		borders[0].w = SCREEN_WIDTH;
	borders[1].x = 0;								borders[1].y = 0;					borders[1].h = SCREEN_HEIGHT*BORDERHEIGHT;	borders[1].w = SCREEN_WIDTH;
	borders[2].x = 0;								borders[2].y = 0;					borders[2].h = SCREEN_HEIGHT;		borders[2].w = SCREEN_WIDTH*BORDERWIDTHVERT;
	borders[3].x = SCREEN_WIDTH*(1-BORDERWIDTHVERT);	borders[3].y = 0;					borders[3].h = SCREEN_HEIGHT;		borders[3].w = SCREEN_WIDTH;
}

bool mariogame::gamemenu() {
	SDL_Rect menurect;
	menurect.x = SCREEN_WIDTH*0.33;
	menurect.y = SCREEN_HEIGHT*0.4;
	menurect.w = SCREEN_WIDTH*0.33;
	menurect.h = SCREEN_HEIGHT*0.2;
	bool decision = false;
	while(!decision){
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return false;
				break;
			case SDLK_n:
				return true;
				break;

			}
			break;
		case SDL_QUIT:
			return false;
			break;
		}
	}

	SDL_FillRect(screen, &menurect, 0xff0000);
	sprintf(text, "Menu: ");
	DrawString(screen, SCREEN_WIDTH*0.35, SCREEN_HEIGHT*0.42, text, charset);
	sprintf(text, "n - nowa gra");
	DrawString(screen, SCREEN_WIDTH*0.37, SCREEN_HEIGHT*0.44, text, charset);
	sprintf(text, "esc - wyjdz");
	DrawString(screen, SCREEN_WIDTH*0.37, SCREEN_HEIGHT*0.46, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	}

	
}

void mariogame::clearApplication(mariolevel* level, mariohero* hero) {
	level->~mariolevel();
	hero->~mariohero();
}


int mariogame::initest() {

	charset = SDL_LoadBMP("cs8x8.bmp");
	if (initWindow()) {
		printf("Nie udalo sie zinicjalizaowac");
		return 1;
	}

	while (!quit) {
		SDL_FillRect(screen, NULL, 0x000000);
		if (gamemenu()) {
			start();
		}
		else quit = true;	
	}

	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(charset);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

int mariogame::start(){


	SDL_FillRect(screen, NULL, 0x000000);

	currentLevel = 1;
	mariolevel my_level(currentLevel);
	mariohero my_hero(&my_level);


	int tryb = BIEGANIE;
	lastSec = 0;
	double move;
	tick1 = SDL_GetTicks();
	delta = 0;
	worldtime = 0;
	fpstimer = 0;
	wektor_x = 0;
	wektor_y = 1.56;


	while(!quit){
		SDL_FillRect(screen, NULL, 0x5C94FC);

		//DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC), SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
		//DrawRectangle(screen, SCREEN_WIDTH*0.05, SCREEN_HEIGHT*0.05, SCREEN_WIDTH - SCREEN_WIDTH*0.05, SCREEN_HEIGHT - SCREEN_HEIGHT*0.05, SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC), SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

		if(!counterFPS()) continue;

		if (worldtime > lastSec + 1) {
			lastSec = worldtime;
			my_level.minusTimeLeft();
		}

		if (my_level.getTimeLeft()<0) {
			my_hero.setMarioState(dead);
		}


		//my_level.drawBlocks(screen);


		if (my_hero.getMarioState() == dead) {
			if (my_hero.getLifes() > 1) {
				my_hero.addLife(-1);
				my_hero.reborn(&my_level);
				my_hero.setMarioState(standing);
				my_level.resetTimeLeft();
			}
			else {
				drawCommunicat(screen, "To bylo ostatnie zycie. Przegrales");
				my_level.deleteLevel();
				my_hero.deleteHero();
				//clearApplication(&my_level, &my_hero);
				return true;
			}
		}else if (my_hero.getMarioState() == winner) {
			if (!my_level.setLevel()) {
				printf("asdasd");
				drawCommunicat(screen, "Gratulacje! Przeszedles wszystkie etapy");
				my_hero.deleteHero();
				my_level.deleteLevel();
				//clearApplication(&my_level, &my_hero);
				return true;
			}
			my_hero.setMarioPosX(my_level.getMarioStartX());
			my_hero.setMarioPosY(my_level.getMarioStartY());
			my_hero.setMarioState(standing);
		}

		if (wektor_y < -MARIOJUMP) my_hero.setMarioState(falling_down);


		if (tryb == HAMOWANIE && wektor_x>0) {
			wektor_x -= 0.1;
		}
		else if (tryb == HAMOWANIE && wektor_x< 0) {
			wektor_x += 0.1;
		}
		if (wektor_x< 0.2 && wektor_x > -0.2) {
			wektor_x = 0;
			my_hero.setMarioDirection(def);
		}

		move = sin(wektor_y) * MARIOJUMPALTITUDE;

		if (!my_hero.checkCollisionY(move, &my_level)) {
			my_hero.addMarioPosY(move);
			my_hero.setMarioDirection(up);
		}
		else my_hero.setMarioState(standing);

		move = sin(wektor_x) * MARIOSPEED;



		if (!my_hero.checkCollisionX(move, &my_level)) {
			my_hero.addMarioPosX(move,&my_level);

		}
		else my_hero.setMarioState(standing);

		if (my_hero.getMarioState() == standing) {
			wektor_y = MARIOGRAV;
		}

		if (my_hero.getMarioState() == falling_down) {
			if (wektor_y < MARIOGRAV) {
				wektor_y += 0.1;
			}
		}

		if (my_hero.checkMonsterCollision(move, &my_level)) {
			SDL_Delay(3000);
			my_hero.setMarioState(dead);
		}

		my_level.draw(screen);
		my_hero.draw(screen);
		DrawBorders(screen,borders);

		//sprintf(text, "FPS: %d %d %.1lf %d Coins: %d", fps, frames, worldtime,my_level.getTimeLeft(), my_hero.coins);
		//DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

		sprintf(text, "LIFES: %d", my_hero.getLifes());
		DrawString(screen, SCREEN_WIDTH*0.02, SCREEN_HEIGHT*0.04, text, charset);
		sprintf(text, "Time Left: %d", my_level.getTimeLeft());
		DrawString(screen, SCREEN_WIDTH*0.5 - strlen(text)*4, SCREEN_HEIGHT*0.04, text, charset);
		sprintf(text, "Coins: %d", my_hero.getCoins());
		DrawString(screen, SCREEN_WIDTH*0.9 - strlen(text) * 4, SCREEN_HEIGHT*0.04, text, charset);
		//mon.drawMonster(screen);



		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_UP:

					if (my_hero.getMarioState() != falling_down && wektor_y>-MARIOJUMP) {
						wektor_y = -MARIOJUMP;
						my_hero.setMarioState(in_jump);
						my_hero.setMarioDirection(up);
					}
					else {
						my_hero.setMarioState(falling_down);
					}

					break;
				case SDLK_DOWN:
					break;
				case SDLK_LEFT:
					if (wektor_x > 0) wektor_x = 0;
					tryb = BIEGANIE;
					if (wektor_x>- MARIOMOVE) {
						wektor_x += -MARIOMOVE/2;
					}
					my_hero.setMarioState(falling_down);
					my_hero.setMarioDirection(left);

					break;
				case SDLK_RIGHT:
					if (wektor_x < 0) wektor_x = 0;
					tryb = BIEGANIE;
					if (wektor_x<MARIOMOVE) {
						wektor_x += MARIOMOVE/2;
					}
					my_hero.setMarioState(falling_down);
					my_hero.setMarioDirection(right);
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					my_hero.setMarioState(falling_down);

					break;
				case SDLK_DOWN:
					break;
				case SDLK_LEFT:
					tryb = HAMOWANIE;
					break;
				case SDLK_RIGHT:
					tryb = HAMOWANIE;
					break;
				case SDLK_n:
					my_level.deleteLevel();
					my_hero.deleteHero();
					return false;
					break;
				case SDLK_s:
					my_hero.saveGame(&my_level, 10);
					break;
				case SDLK_l:
					my_hero.loadLevel(&my_level);
					break;
				case SDLK_ESCAPE:
					my_level.deleteLevel();
					my_hero.deleteHero();
					quit = true;
					return 0;
					break;
				}
				break;
			case SDL_QUIT:
				quit = true;
			}
		}

	}

return false;
}