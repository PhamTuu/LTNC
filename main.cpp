#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>

using namespace std;

int score = 0, lvl = 1, prevScore;
int highscore = 0;
bool music = true;

const int SCREEN_WIDTH = 360;
const int SCREEN_HEIGHT = 640;
const int START = 520;
const int line1 = 24, line2 = 114, line3 = 208, line4 = 298;
const int maxLvl = 7;

static const int CAR_WIDTH = 40;
static const int CAR_HEIGHT = 75;
static const int CARS_VEL = 4;
static const int degreeVel = 4;

static const int OBSTACLE_WIDTH = 40;
static const int OBSTACLE_HEIGHT = 40;

static const int OBSTACLE_VEL = 5;
static const int OBJ_START = -100;
static const int OBJ_DISTANCE = 75;
static const int reduceObjDistance = 5;
static const int OBSTACLE_VEL_INC = 1;

class Texture
{
    public:
        Texture();

        ~Texture();

        bool loadFromFile( std::string path );

        void free();

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        void render( int x, int y, int w, int h, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        int getWidth();
        int getHeight();

        void update_sprite();
        SDL_Texture* mTexture;
        SDL_Rect sprite;
        int x;
        int y;
        int W;
        int H;

        int mWidth;
        int mHeight;
};

//The mouse button
class LButton : public Texture
{
	public:
		LButton();

		void setPosition( int x, int y, int w, int h);

		bool handleEvent( SDL_Event* e );

	private:
		SDL_Point mPosition;
};

class Text: public Texture
{
public:

    //Creates image from font string
    void loadFromRenderedText( std::string textureText, SDL_Color textColor );

    //input text and size
    void loadText(string text, int size);
};