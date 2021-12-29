#define GAMEFIRSTLOAD 0
#define GAMEPLAYSTART 1
#define GAMEPLAYEND 2
#define COREGAMELOOP 3

extern INT16 birdY;
extern INT8 birdVelocityY;
extern UINT8 backgroundScrollCounter,joypadCurrent,joypadPrevious,birdX,score,betweenTwoPipes,distance,alive;

void ScrollTheBackgroundAndApplyVelocity();

UINT8 ScrollSpritesForPipes(UINT8 speed);