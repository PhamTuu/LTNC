# LTNC
CarCar

Phạm Anh Tú - 22029055

Intruction:
- Moving 2 cars in different line to dogde SQUARE and eat CIRCLE to earn point
- How to play:
 - ->/a :Switch blue car 's line 
 - <-/d :Switch red car 's line
 - Enter/Space :Play/Replay/Pause/Continue
 - Esc :Quit

Function:
- Music On/Off
- Velocity and Number of Obstacle increase through time

GAME                                                                                               
![alt text](image-1.png)

Source code game:
- Core Structure:
 - Classes:
    - Texture: A base class for handling textures (images) in the game. It loads images, renders them on the screen, and manages their dimensions.
    - LButton: Inherits from Texture and represents clickable buttons in the game.
    - Text: Inherits from Texture and specializes in rendering text on the screen using SDL_ttf.
    - Score: Inherits from Texture and renders the score and high score values.
    - BlueCar and RedCar: Represent the two player-controlled cars. They handle input (left/right movement), update their positions, and render themselves on the screen.
    - Obstacle: Represents the obstacles (dots and squares) that the cars must avoid or collect. It handles obstacle creation, movement, and rendering.

- Global variables:
    - gWindow and gRenderer: The main SDL window and renderer for displaying graphics.
    - gFont: The font used for rendering text.
    - mOpen, mPause, mGameOver*, mClick, mHighScore, mScore: Sound effects and music for the game.
    - gBackground, gPlay, gDarkBackground, etc.: Textures used in the game.
    - blueCar, redCar: Instances of the car classes.

- main functions:
    - init(): Initializes SDL, creates the window and renderer, and sets up SDL_image, SDL_ttf, and SDL_mixer.
    - loadAudio(), loadMedia(), loadBackground(), loadBlueCar(), loadRedCar(), loadDot(): Load various game assets (sounds, images).
    - close(): Frees resources and shuts down SDL.
    - hitASquare(): Checks if a car has collided with a square obstacle, ending the game, or a dot obstacle, increasing the score.
    - missAPoint(): Checks if a dot obstacle has passed the screen without being collected, ending the game.
    - main():
        - Initializes the game and loads media.
        - Implements the main game loop.
        - Handles different game states:
         - home: Displays the main menu with Play and High Score buttons.
         - pause: Displays the pause screen with Continue and Home buttons.
         - play: Runs the actual gameplay:
          - Handles car movement based on user input.
          - Creates and updates obstacle positions.
          - Checks for game over conditions.
          - Renders the game scene.
          - Updates the score and level.
         - replay: Displays the game over screen with the score, high score, and Replay and Home buttons.
