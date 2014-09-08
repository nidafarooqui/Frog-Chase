#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <queue>
#include <vector>
#if defined(_MSC_VER)
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#elif defined(__clang__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

/*
* The Chase Frog Game
*/
//Screen attributes
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 728;

// Scaling the tiles to be 80x80
const int TILE_SIZE = 80;

std::multimap<int, int> rocks;
bool startSprite = false;
//generate a random number between minimum and maximum
int roll(int min, int max)
{
	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX);

	// [0,1[ * (max - min) + min is in [min,max[
	int that = min + static_cast<int>(x * (max - min));

	return that;
}
class Card
{
public:
	//initialize variables
	Card();

	//deallocate memory
	//~Card();

	//get X Position of the red chance tile
	int getCardXPos();

	//get Y position of the red chance tile
	int getCardYPos();

	//set a and y positions of the red tile
	void setPositions(int x, int y);

	//randomly allocate positions to the red tiles
	void setRandomPositions();

private:
	int posX;
	int posY;

};

Card cardHome, cardBird, cardDown1, cardDown2;
Card::Card()
{
	setRandomPositions();
}
int Card::getCardXPos()
{
	return posX;
}

int Card::getCardYPos()
{
	return posY;
}
void Card::setPositions(int x, int y)
{
	posX = x;
	posY = y;
}
//randomly set positions of the tiles
void Card::setRandomPositions()
{
	int x, y;

	int minX = 1;
	int maxX = 3;
	x = roll(minX, maxX);
	posX = x;

	int minY = 2;
	int maxY = 8;
	y = roll(minY, maxY);
	posY = y;
}


class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);


	//Deallocates texture
	void free();


	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Set image dimensions
	void setWidth(int w);
	void setHeight(int h);

	//Texture's collision box
	SDL_Rect iCollider;

	void setCollisionDimensions(int w, int h);

	void setCollisionDimensionsPos(int x, int y);

	int getCollisionX();
	int getCollisionY();
	int getCollisionW();
	int getCollisionH();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	//Set collision box dimension
	iCollider.w = mWidth;
	iCollider.h = mHeight;
	iCollider.x = 0;
	iCollider.y = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}


void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}


int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::setWidth(int w)
{
	mWidth = w;
	iCollider.w = mWidth;
}

void LTexture::setHeight(int h)
{
	mHeight = h;
	iCollider.h = mHeight;
}

void LTexture::setCollisionDimensions(int w, int h)
{
	iCollider.w = mWidth;
	iCollider.h = mHeight;
}
void LTexture::setCollisionDimensionsPos(int x, int y)
{
	iCollider.x = x;
	iCollider.y = y;
}
int LTexture::getCollisionX()
{
	return iCollider.x;
}
int LTexture::getCollisionY()
{
	return iCollider.y;
}
int LTexture::getCollisionW()
{
	return iCollider.w;
}
int LTexture::getCollisionH()
{
	return iCollider.h;
}
/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* LTexture::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)
	{
		logSDLError(std::cout, "LoadTexture");
	}

	//set collision box dimensions of the image
	SDL_Surface* loadedSurface = IMG_Load(file.c_str());
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	setCollisionDimensions(mWidth, mHeight);

	return texture;
}
//texture for the house
LTexture t4, menu, rock;

//randomly set positions of the rocks
void setRandomRockPositions()
{
	for (int i = 0; i<40; i++)
	{
		int x, y;

		int minX = 3;
		int maxX = 8;
		x = roll(minX, maxX);


		int minY = 1;
		int maxY = 9;
		y = roll(minY, maxY);


		if ((x != 9 && y != 5))
		{
			rock.setCollisionDimensionsPos(x, y);
			rocks.insert(std::pair<int, int>(x, y));

			//std::cout<<rock.getCollisionX()<<"  "<<rock.getCollisionY()<<std::endl;
		}
	}


}
//The sprite that will move around on the screen
class Sprite
{
public:
	//The dimensions of the sprite
	static const int SPRITE_WIDTH = 104;
	static const int SPRITE_HEIGHT = 96;

	//Maximum axis velocity of the sprite
	static const int SPRITE_VEL = 80;

	//Initializes the variables
	Sprite();

	//Set the positions of the sprite
	void setPositions(int x, int y);

	//Get the position X of the sprite
	int getPositionX();

	//Get the position Y of the sprite
	int getPositionY();

	//Takes key presses and adjusts the sprite's velocity
	void handleEvent(SDL_Event& e);

	//Moves the sprite
	void move();

	//checks all possible collisions
	bool checkCollisions();

	//checks collision between two bounded boxes
	bool collisionExists(int x, int y, SDL_Rect b);


private:
	//The X and Y offsets of the sprite
	int mPosX, mPosY;

	//The velocity of the sprite
	int mVelX, mVelY;

	//Sprite's collision box
	SDL_Rect mCollider;
};
Sprite::Sprite()
{
	//Initialize the offsets
	mPosX = 0;
	mPosY = 0;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Set collision box dimension
	mCollider.w = SPRITE_WIDTH;
	mCollider.h = SPRITE_HEIGHT;
}

void Sprite::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) //&& e.key.repeat == 0
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= SPRITE_VEL; break;
		case SDLK_DOWN: mVelY += SPRITE_VEL; break;
		case SDLK_LEFT: mVelX -= SPRITE_VEL; break;
		case SDLK_RIGHT: mVelX += SPRITE_VEL; break;
			//case SDLK_RETURN: startSprite=true;; break;
		}

	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) //&& e.key.repeat == 0
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += SPRITE_VEL; break;
		case SDLK_DOWN: mVelY -= SPRITE_VEL; break;
		case SDLK_LEFT: mVelX += SPRITE_VEL; break;
		case SDLK_RIGHT: mVelX -= SPRITE_VEL; break;
			//case SDLK_RETURN: startSprite=true; break;
		}
	}
}
bool Sprite::collisionExists(int x, int y, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = x;
	rightA = x + b.w;
	topA = y;
	bottomA = y + b.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB + 67)
	{
		return false;
	}

	if (topA >= bottomB - 67)
	{
		return false;
	}

	if (rightA <= leftB - 50)
	{
		return false;
	}

	if (leftA >= rightB - 50)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Sprite::checkCollisions()
{
	SDL_Rect sprite;
	sprite.x = mPosX;
	sprite.y = mPosY;
	sprite.w = SPRITE_WIDTH;
	sprite.h = SPRITE_HEIGHT;

	std::vector<int> x(rocks.size());
	std::vector<int> y(rocks.size());
	std::multimap<int, int>::iterator it = rocks.begin();
	while (it != rocks.end())
	{
		for (int i = 0; i<x.size(); i++)
		{
			x[i] = ((*it).first)*TILE_SIZE;
			++it;
		}
	}
	std::multimap<int, int>::iterator it2 = rocks.begin();
	while (it2 != rocks.end())
	{
		for (int j = 0; j<y.size(); j++)
		{
			y[j] = ((*it2).second)*TILE_SIZE;
			++it2;
		}
	}

	for (int k = 0; k<rocks.size(); k++)
	{
		for (int l = 0; l<rocks.size(); l++)
		{
			if (collisionExists(y[l], x[k], sprite))
			{
				std::cout << "Collision Detected!" << std::endl;
				return true;

			}
		}
	}

	return false;
}

void Sprite::move()
{
	//Move the sprite left or right
	mPosX += mVelX;
	mCollider.x = mPosX;

	//If the sprite went too far to the left or right
	if ((mPosX < 0) || (mPosX + SPRITE_WIDTH > SCREEN_WIDTH - 100) || checkCollisions())
	{
		//Move back
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}

	//Move the sprite up or down
	mPosY += mVelY;
	mCollider.y = mPosY;

	//If the sprite went too far up or down
	if ((mPosY < 0) || (mPosY + SPRITE_HEIGHT > SCREEN_HEIGHT + 50) || checkCollisions())
	{
		//Move back
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}

void Sprite::setPositions(int x, int y)
{
	mPosX = x;
	mPosY = y;
}

int Sprite::getPositionX()
{
	return mPosX;
}

int Sprite::getPositionY()
{
	return mPosY;
}





/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}
void drawTiles(int xTiles, int yTiles, SDL_Texture *background, SDL_Texture *redTileImage, SDL_Texture *rockImage, SDL_Renderer *renderer)
{
	//y is row and x is colummn
	//Draw the tiles by calculating their positions
	for (int i = 0; i < xTiles * yTiles; ++i)
	{
		int x = i % xTiles;
		int y = i / xTiles;
		renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	}

	renderTexture(redTileImage, renderer, 7 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	renderTexture(redTileImage, renderer, cardBird.getCardYPos() * TILE_SIZE, cardBird.getCardXPos() * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	renderTexture(redTileImage, renderer, cardDown1.getCardYPos() * TILE_SIZE, cardDown1.getCardXPos() * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	renderTexture(redTileImage, renderer, cardDown2.getCardYPos() * TILE_SIZE, cardDown2.getCardXPos() * TILE_SIZE, TILE_SIZE, TILE_SIZE);

	for (int i = 0; i<11; i++)
	{
		int x = i;
		int y = 4;
		renderTexture(rockImage, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	}
	//int homeX=0;
	//int homeY=0;
	//for(std::multimap<int,int>::iterator it=rocks.begin(); it!=rocks.end(); ++it)
	//{
	//if((it->first)*TILE_SIZE == 400 && (it->second)*TILE_SIZE == 720)
	//{
	//homeX=(it->first);
	//homeY=(it->second);
	//rocks.erase(it);
	//}
	//}
	if (rock.getCollisionX() == 20 && rock.getCollisionY() == 510 || rock.getCollisionX() == 20 && rock.getCollisionY() == 430 || rock.getCollisionX() == 400 && rock.getCollisionY() == 720)//|| homeX*TILE_SIZE==400 && homeY*TILE_SIZE==720)
	{
		rock.setCollisionDimensionsPos(9 * TILE_SIZE, 7 * TILE_SIZE);
		renderTexture(rockImage, renderer, 9 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, TILE_SIZE);

	}


	else
	{
		for (std::multimap<int, int>::iterator it = rocks.begin(); it != rocks.end(); ++it)
		{
			if ((it->first)*TILE_SIZE != 400 && (it->second)*TILE_SIZE != 720)
			{
				renderTexture(rockImage, renderer, (it->second)*TILE_SIZE, (it->first)*TILE_SIZE, TILE_SIZE, TILE_SIZE);
			}
			else
			{
				rocks.erase(it);
			}

		}
	}

}

void drawImages(SDL_Texture *image, SDL_Texture *image2, SDL_Texture *image3, SDL_Texture *menuImage, Sprite sprite1, Sprite sprite2, SDL_Renderer *renderer)
{
	//Draw our image in the center of the window
	//We need the foreground image's width to properly compute the position
	//of it's top left corner so that the image will be centered
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = sprite1.getPositionX();//SCREEN_WIDTH / 2 - iW / 2;
	int y = sprite1.getPositionY();//SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, renderer, x, y);

	int iW2, iH2;
	SDL_QueryTexture(image2, NULL, NULL, &iW2, &iH2);
	int x2 = sprite2.getPositionX();  //805;
	int y2 = sprite2.getPositionY();//10;
	renderTexture(image2, renderer, x2, y2);

	int iW3, iH3;
	SDL_QueryTexture(image3, NULL, NULL, &iW3, &iH3);
	int x3 = 400;
	int y3 = 640;
	renderTexture(image3, renderer, x3, y3);

	int iW4, iH4;
	SDL_QueryTexture(menuImage, NULL, NULL, &iW4, &iH4);
	int x4 = 880;//815
	int y4 = 0;//10
	renderTexture(menuImage, renderer, x4, y4);
}




int main(int argc, char** argv)
{
	//The music that will be played
	Mix_Music *gMusicIntro = NULL;
	Mix_Music *gMusicMid = NULL;
	Mix_Music *gMusicEnd = NULL;

	//The sound effects that will be used
	Mix_Chunk *gCroak = NULL;
	Mix_Chunk *gWhoosh = NULL;
	Mix_Chunk *gLaugh = NULL;

	bool success = true;
	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}


	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}//Setup our window and renderer

	SDL_Window *window = SDL_CreateWindow("Frog Chase", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		logSDLError(std::cout, "CreateWindow");
		return 2;
	}


	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		logSDLError(std::cout, "CreateRenderer");
		return 3;
	}
	else
	{

		//Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
	}



	//The textures we'll be using
	LTexture t1, t2, t3, intro, end, redTile;

	SDL_Texture *background = t1.loadTexture("white_tile_small.png", renderer);
	SDL_Texture *image = t2.loadTexture("frog_tiny.png", renderer);
	SDL_Texture *image2 = t3.loadTexture("pelican.png", renderer);
	SDL_Texture *image3 = t4.loadTexture("frog_house.png", renderer);
	SDL_Texture *menuImage = menu.loadTexture("command_screen.png", renderer);
	SDL_Texture *introImage = intro.loadTexture("peeking_frog2.png", renderer);
	SDL_Texture *gameOverImage = end.loadTexture("game_over_screen.png", renderer);
	SDL_Texture *redTileImage = redTile.loadTexture("mysterydoor_tiny.png", renderer);
	SDL_Texture *rockImage = rock.loadTexture("small_rock.png", renderer);

	//Load music
	gMusicIntro = Mix_LoadMUS("GameIntro.mp3");
	if (gMusicIntro == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	//Load music
	gMusicMid = Mix_LoadMUS("GameMid.mp3");
	if (gMusicMid == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	//Load music
	gMusicEnd = Mix_LoadMUS("Undefeated.mp3");
	if (gMusicEnd == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gCroak = Mix_LoadWAV("Frog Croaking.wav");
	if (gCroak == NULL)
	{
		printf("Failed to load croaking sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gWhoosh = Mix_LoadWAV("Whoosh.wav");
	if (gWhoosh == NULL)
	{
		printf("Failed to load whoosh sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gLaugh = Mix_LoadWAV("Evil_laugh.wav");
	if (gLaugh == NULL)
	{
		printf("Failed to load evil laugh sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	bool allGame = false;

	while (!allGame)
	{
		//Load music
		gMusicIntro = Mix_LoadMUS("GameIntro.mp3");
		if (gMusicIntro == NULL)
		{
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		//Load music
		gMusicMid = Mix_LoadMUS("GameMid.mp3");
		if (gMusicMid == NULL)
		{
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		//Load music
		gMusicEnd = Mix_LoadMUS("Undefeated.mp3");
		if (gMusicEnd == NULL)
		{
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		setRandomRockPositions();
		cardHome.setPositions(3 * TILE_SIZE, 7 * TILE_SIZE);
		cardBird.setRandomPositions();
		cardDown1.setRandomPositions();
		cardDown2.setRandomPositions();
		bool gameEnd = false;
		int x = 415;
		int y = SCREEN_HEIGHT - 80;
		t4.setCollisionDimensionsPos(x, y);

		//Make sure they both loaded ok
		if (background == nullptr || image == nullptr || image2 == nullptr || image3 == nullptr || menuImage == nullptr || introImage == nullptr || gameOverImage == nullptr || redTileImage == nullptr || rockImage == nullptr){
			return 4;
		}
		SDL_Event e;
		bool quit = false;
		//bool startSprite = false;
		Sprite sprite1;
		Sprite sprite2;

		sprite1.setPositions(20, 30);
		sprite2.setPositions(815, 10);

		//queue of events
		std::queue<SDL_Event> events;

		//Determine how many tiles we'll need to fill the screen
		int xTiles = SCREEN_WIDTH / TILE_SIZE;
		int yTiles = SCREEN_HEIGHT / TILE_SIZE;

		bool exit = false;
		int channel = -1;
		Mix_PlayMusic(gMusicIntro, -1);
		//welcome screen
		while (!exit)
		{


			int iW, iH;
			SDL_QueryTexture(introImage, NULL, NULL, &iW, &iH);
			int x = SCREEN_WIDTH / 2 - iW / 2;
			int y = SCREEN_HEIGHT / 2 - iH / 2;

			renderTexture(introImage, renderer, x, y);


			//Update the screen
			SDL_RenderPresent(renderer);

			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					exit = true;
					quit = true;
					gameEnd = true;
					allGame = true;
				}
				if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
				{

					events.push(e);
					if (e.key.keysym.sym == SDLK_RETURN)
					{
						channel = Mix_PlayChannel(-1, gCroak, 0);
						SDL_Delay(200);
						Mix_HaltChannel(channel);
						exit = true;
					}
					else if (e.key.keysym.sym == SDLK_m)
					{
						Mix_PlayMusic(gMusicIntro, -1);
					}
					else if (e.key.keysym.sym == SDLK_0)
					{
						Mix_HaltMusic();
					}
				}
			}

		}

		//Stop the music
		Mix_HaltMusic();
		Mix_FreeMusic(gMusicIntro);
		gMusicIntro = NULL;
		SDL_Delay(50);
		Mix_PlayMusic(gMusicMid, -1);
		//main game loop
		while (!quit)
		{

			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
					gameEnd = true;
					allGame = true;
				}
				if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
				{

					events.push(e);
					if (e.key.keysym.sym == SDLK_RETURN)
					{
						while (!events.empty())
						{
							SDL_Event b = events.front();
							sprite1.handleEvent(b);
							SDL_RenderClear(renderer);
							drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
							drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);
							sprite1.move();

							Mix_PlayChannel(-1, gWhoosh, 0);

							SDL_Delay(100); // possible delay.

							SDL_RenderPresent(renderer);

							events.pop();
						}
						//startSprite = true;

					}
					else if (e.key.keysym.sym == SDLK_0)
					{
						//Stop the music
						Mix_PauseMusic();
					}
					else if (e.key.keysym.sym == SDLK_m)
					{
						//Play the music
						Mix_PlayMusic(gMusicMid, -1);
					}
				}
			}










			//Clear the window
			SDL_RenderClear(renderer);


			//Draw the tiles by calculating their positions
			drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);

			//draw all images
			drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);


			//Update the screen
			SDL_RenderPresent(renderer);

			int frogPosX = sprite1.getPositionX();
			int frogPosY = sprite1.getPositionY();

			int cHomeX = cardHome.getCardXPos() + 30;
			int cHomeY = cardHome.getCardYPos() + 20;

			int cBirdX = cardBird.getCardXPos()*TILE_SIZE + 30;
			int cBirdY = cardBird.getCardYPos()*TILE_SIZE + 20;

			int cDown1X = cardDown1.getCardXPos()*TILE_SIZE + 30;
			int cDown1Y = cardDown1.getCardYPos()*TILE_SIZE + 20;

			int cDown2X = cardDown2.getCardXPos()*TILE_SIZE + 30;
			int cDown2Y = cardDown2.getCardYPos()*TILE_SIZE + 20;
			//std::cout<<frogPosY;

			if (frogPosX <= 420 && frogPosX >= 400 && frogPosY <= 670 && frogPosY >= 650)
			{
				sprite1.setPositions(410, SCREEN_HEIGHT - 80);
				sprite2.setPositions(815, 10);

				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);

				//play music for winning
				Mix_PlayMusic(gMusicEnd, -1);

				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(400);
				quit = true;
			}
			else if (frogPosX == cHomeY && frogPosY == cHomeX)
			{
				sprite1.setPositions(420, 670);
				sprite2.setPositions(815, 10);
				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);
				//play music for winning
				Mix_PlayMusic(gMusicEnd, -1);
				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(400);

				quit = true;
			}
			else if (frogPosX == cBirdY && frogPosY == cBirdX)
			{
				sprite1.setPositions(815, 10);
				sprite2.setPositions(815, 10);
				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);
				//play music for losing
				Mix_PlayChannel(-1, gLaugh, 0);
				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(200);

				quit = true;
			}
			else if (frogPosX == cDown1Y && frogPosY == cDown1X)
			{
				sprite1.setPositions(20, 430);
				sprite2.setPositions(815, 10);
				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);
				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(200);

			}
			else if (frogPosX == cDown2Y && frogPosY == cDown2X)
			{
				sprite1.setPositions(20, 510);
				sprite2.setPositions(815, 10);
				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);
				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(200);
			}
			if (frogPosX == cDown2Y && frogPosY == cDown2X || frogPosX == cDown1Y && frogPosY == cDown1X || frogPosX == 20 && frogPosY == 30 || frogPosX == 20 && frogPosY == 430 || frogPosX == 20 && frogPosY == 510 || (frogPosX <= 420 && frogPosX >= 400 && frogPosY <= 670 && frogPosY >= 650))
			{
				sprite2.setPositions(815, 10);
				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);

				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(200);
			}
			else
			{
				//if(frogPosX !=20 || frogPosY != 30 || frogPosX!=20 && frogPosY != 30)
				//{
				sprite2.setPositions(frogPosX, frogPosY);
				drawTiles(xTiles, yTiles, background, redTileImage, rockImage, renderer);
				drawImages(image, image2, image3, menuImage, sprite1, sprite2, renderer);
				Mix_PlayChannel(-1, gWhoosh, 0);
				//play music for losing
				Mix_PlayChannel(-1, gLaugh, 0);
				//Update the screen
				SDL_RenderPresent(renderer);
				SDL_Delay(200);

				quit = true;


				//}
			}


		}
		//Stop the music
		//Mix_HaltMusic();
		Mix_FreeMusic(gMusicMid);
		gMusicMid = NULL;
		//game over screen

		while (!gameEnd)
		{

			//Clear the window
			SDL_RenderClear(renderer);

			int iW2, iH2;
			SDL_QueryTexture(gameOverImage, NULL, NULL, &iW2, &iH2);
			int x2 = SCREEN_WIDTH / 2 - iW2 / 2;
			int y2 = SCREEN_HEIGHT / 2 - iH2 / 2;
			renderTexture(gameOverImage, renderer, x2, y2);

			//Update the screen
			SDL_RenderPresent(renderer);
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					gameEnd = true;
					allGame = true;
				}
				if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
				{


					if (e.key.keysym.sym == SDLK_y)
					{
						gameEnd = true;
						allGame = false;
						//Stop the music
						Mix_HaltMusic();
						Mix_FreeMusic(gMusicEnd);
						gMusicEnd = NULL;

					}
					else if (e.key.keysym.sym == SDLK_n)
					{
						gameEnd = true;
						allGame = true;
						Mix_HaltMusic();
						Mix_FreeMusic(gMusicEnd);
						gMusicEnd = NULL;
					}
					else if (e.key.keysym.sym == SDLK_m)
					{
						Mix_PlayMusic(gMusicEnd, -1);
					}
					else if (e.key.keysym.sym == SDLK_0)
					{
						Mix_HaltMusic();
					}

				}
			}
		}


	}


	//Destroy the various items
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(image2);
	SDL_DestroyTexture(image3);
	SDL_DestroyTexture(menuImage);
	SDL_DestroyTexture(introImage);
	SDL_DestroyTexture(gameOverImage);
	SDL_DestroyTexture(redTileImage);
	SDL_DestroyTexture(rockImage);

	//Free the sound effects
	Mix_FreeChunk(gCroak);
	Mix_FreeChunk(gWhoosh);
	Mix_FreeChunk(gLaugh);


	gCroak = NULL;
	gWhoosh = NULL;
	gLaugh = NULL;

	//Free the music






	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;

	//system("PAUSE");
}
