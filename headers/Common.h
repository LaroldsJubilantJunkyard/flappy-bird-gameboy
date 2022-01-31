#define GAMEFIRSTLOAD 0
#define GAMEPLAYSTART 1
#define GAMEPLAYEND 2
#define COREGAMELOOP 3

extern int16_t birdY;
extern int8_t birdVelocityY;
extern uint8_t backgroundScrollCounter,highScore,floorBackgroundScroll,joypadCurrent,joypadPrevious,birdX,score,betweenTwoPipes,distance,alive,topBackgroundScroll,midBackgroundScroll,lowBackgroundScroll;

void MoveAndUpdateFlappyBird();
void UpdateScoreTextAt(uint8_t x, uint8_t y,uint16_t scoreToDisplay);
uint8_t ScrollSpritesForPipes(uint8_t speed);