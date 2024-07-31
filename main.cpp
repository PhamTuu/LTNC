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

    void loadFromRenderedText( std::string textureText, SDL_Color textColor );

    void loadText(string text, int size);
};

class Score: public Texture
{
public:

    void loadFromRenderedText( std::string textureText, SDL_Color textColor );

    void loadText(string text, int size);
};

class BlueCar: public Texture
{
    public:

    BlueCar();

    void handle_Event( SDL_Event& e );

    void turnL();

    void turnR();

    void move();

    void render();

    int bVel;
    int bdegree;
};

class RedCar: public Texture
{
    public:

    RedCar();

    void handle_Event( SDL_Event& e );

    void turnL();

    void turnR();

    void move();

    void render();

    int rVel;
    int rdegree;
};

class Obstacle : public Texture
{
    public:
    Obstacle();

    void create(int line);

    void update_Pos();

    void show();

    int obVel, obLine, obType;
};

bool init();

bool loadDot();

bool loadBackground();

bool loadBlueCar();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;

Mix_Music *mOpen = NULL;

Mix_Chunk *mPause = NULL;
Mix_Chunk *mGameOver1 = NULL;
Mix_Chunk *mGameOver2 = NULL;
Mix_Chunk *mGameOver3 = NULL;
Mix_Chunk *mGameOver4 = NULL;
Mix_Chunk *mClick = NULL;
Mix_Chunk *mHighScore = NULL;
Mix_Chunk *mScore = NULL;

Texture gBackground;
LButton gPlay, gDarkBackground, gMusicOn, gMusicOff, gPause, gHighScore, gReplay, gHome;
Texture gBlueCar, gRedCar;
Texture bDot, bSquare, rDot, rSquare;
Text gText;
Score scoreText, highScoreText, gscoreText, ghighScoreText;

Texture::Texture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture()
{
    free();
}

bool Texture::loadFromFile( std::string path )
{
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface( loadedSurface );
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void Texture::free()
{
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void Texture::render( int x, int y, int w, int h, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    SDL_Rect renderQuad = { x, y, w, h };

    SDL_RenderCopyEx( gRenderer, mTexture, NULL, &renderQuad, angle, center, flip );
}

int Texture::getWidth()
{
    return mWidth;
}

int Texture::getHeight()
{
    return mHeight;
}

void Texture::update_sprite()
{
    sprite.x = x;
    sprite.y = y;
    sprite.h = H;
    sprite.w = W;
}

//class Text
void Text::loadFromRenderedText( string textureText, SDL_Color textColor )
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
}

void Text::loadText( string text, int size)
{
	gFont = TTF_OpenFont( "ttf/gasalt.thin.ttf", size );
	if( gFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		SDL_Color textColor = { 220, 220, 220 };
		loadFromRenderedText( text, textColor );
	}
}

void Score::loadFromRenderedText( string textureText, SDL_Color textColor )
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
}

void Score::loadText( string text, int size)
{
	gFont = TTF_OpenFont( "ttf/alienleaguebold.ttf", size );
	if( gFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		SDL_Color textColor = { 220, 220, 220 };
		loadFromRenderedText( text, textColor );
	}
}

// Class LButton
LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
}

void LButton::setPosition( int x, int y, int w, int h )
{
	mPosition.x = x;
	mPosition.y = y;
	W = w;
    H = h;
}

bool LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		if( x < mPosition.x )
		{
			inside = false;
		}
		else if( x > mPosition.x + W )
		{
			inside = false;
		}
		else if( y < mPosition.y )
		{
			inside = false;
		}
		else if( y > mPosition.y + H )
		{
			inside = false;
		}

		if( inside && e->type == SDL_MOUSEBUTTONDOWN)
			return true;
	}
	return false;
}

//class Obstacle
Obstacle::Obstacle()
{
    mTexture = NULL;
    W = OBSTACLE_WIDTH;
    H = OBSTACLE_HEIGHT;
}

void Obstacle::create(int preLine)
{
    if (y <= OBJ_START || y >= SCREEN_HEIGHT + obVel)
    {
        y = OBJ_START;

        obLine = rand() % 2;
        if (preLine > 180 && preLine < 360) switch (obLine)
        {
        case 0:
            x = line1;
            break;
        case 1:
            x = line2;
            break;
        }

        if (preLine < 180 && preLine > 0) switch (obLine)
        {
        case 0:
            x = line3;
            break;
        case 1:
            x = line4;
        break;
        }
        obType = rand() % 2;
    }
}

void Obstacle::update_Pos()
{
    obVel = OBSTACLE_VEL + lvl/2*OBSTACLE_VEL_INC;
    y += obVel;
    if (x == line1 || x == line2 || x == line3 || x == line4)
        update_sprite();
}

void Obstacle::show()
{
    switch (x)
        {
        case line1:
            if (obType == 1) bDot.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            else if (obType == 0) bSquare.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            break;
        case line2:
            if (obType == 1) bDot.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            else if (obType == 0) bSquare.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            break;
        case line3:
            if (obType == 1) rDot.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            else if (obType == 0) rSquare.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            break;
        case line4:
            if (obType == 1) rDot.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            else if (obType == 0) rSquare.render( x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
            break;
        }
}

BlueCar::BlueCar()
{
    x = line1;
    y = START;

    bVel = 0;
    bdegree = 0;
    W = CAR_WIDTH;
    H = CAR_HEIGHT;
}

void BlueCar::handle_Event( SDL_Event& e )
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        if ( e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a)
        {
            if (x == line1) turnL();
            else if (x == line2) turnR();
        }
    }
}

void BlueCar::turnL()
{
    bVel = - CARS_VEL;
}

void BlueCar::turnR()
{
    bVel = CARS_VEL;
}

void BlueCar::move()
{
    if( ( x >= line1 ) && ( x <= line2 ) )
    {
        x -= bVel;
    }

    if( ( x > line1 ) && ( x < line2 ) )
        if (x > 68) bdegree -= degreeVel;
        else if (x <= 68) bdegree += degreeVel;

    if (x > line2) x = line2;
    else if (x < line1) x = line1;
    update_sprite();
}

void BlueCar::render()
{
    gBlueCar.render( x, y, CAR_WIDTH, CAR_HEIGHT, bdegree);

}

RedCar::RedCar()
{
    x = line4;
    y = START;

    rVel = 0;
    rdegree = 0;
    W = CAR_WIDTH;
    H = CAR_HEIGHT;
}

void RedCar::handle_Event( SDL_Event& e )
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        if ( e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d)
        {
            if (x == line3) turnR();
            else if (x == line4) turnL();
        }
    }
}

void RedCar::turnL()
{
    rVel = CARS_VEL;
}

void RedCar::turnR()
{
    rVel = - CARS_VEL;
}

void RedCar::move()
{
    if( ( x >= line3 ) && ( x <= line4 ) )
    {
        x -= rVel;
    }

    if( ( x > line3 ) && ( x < line4 ) )
        if (x > 252) rdegree -= degreeVel;
        else if (x <= 252) rdegree += degreeVel;

    if (x > line4) x = line4;
    else if (x < line3) x = line3;
    update_sprite();
}

void RedCar::render()
{
    gRedCar.render( x, y, CAR_WIDTH, CAR_HEIGHT, rdegree);
}

int main (){
    return 0;
}