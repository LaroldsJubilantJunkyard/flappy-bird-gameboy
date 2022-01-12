#define GAMEFIRSTLOAD 0
#define GAMEPLAYSTART 1
#define GAMEPLAYEND 2
#define COREGAMELOOP 3

extern INT16 birdY;
extern INT8 birdVelocityY;
extern UINT8 backgroundScrollCounter,highScore,floorBackgroundScroll,joypadCurrent,joypadPrevious,birdX,score,betweenTwoPipes,distance,alive,topBackgroundScroll,midBackgroundScroll,lowBackgroundScroll;

void MoveAndUpdateFlappyBird();
void UpdateScoreTextAt(UINT8 x, UINT8 y,UINT16 scoreToDisplay);
UINT8 ScrollSpritesForPipes(UINT8 speed);