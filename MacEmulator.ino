#include <M5Unified.h>
#include <M5AtomS3.h>
#include "images.h"

//Extended from https://github.com/tomorrow56/M5Stack_Stand/

M5Canvas canvas(&AtomS3.Display);
uint16_t FORE_COLOR = TFT_BLACK;
uint16_t BACK_COLOR = TFT_WHITE;

#define MAX_TILE_VALUE 15
int tiles[MAX_TILE_VALUE+1];
int current = 0;//random(0,MAX_TILES+1);

#define CHANGEAFTER 60 * 1000
unsigned long lastChange = 0;

void setup()
{
  auto cfg = M5.config();
  cfg.internal_mic = false;
  M5.begin(cfg);

  Serial.begin(115200);
  Serial.println("Startup");

  uint8_t display_rotation = 0; // ディスプレイの向き(0〜3)
  M5.Display.setRotation(display_rotation);

  for(int i =0 ; i<MAX_TILE_VALUE+1;i++)
    tiles[i]=i;
  shuffle();
  for(int i =0 ; i<MAX_TILE_VALUE;i++)
  {
    Serial.print(tiles[i]);
    Serial.print(" - ");
  }
  Serial.println(tiles[MAX_TILE_VALUE]);

  //for(int i =0 ; i<MAX_TILE_VALUE+1;i++)
  //  tiles[i]=15;

  drawRandom();
}

void shuffle()
{
  int i, j, tmp;

  for (i = MAX_TILE_VALUE ; i > 0; i--) {
    j = random(i + 1);
    tmp = tiles[j];
    tiles[j] = tiles[i];
    tiles[i] = tmp;
  }
}

void myDelay(int ms) //This makes it harder to program, have to press the button. but cuts the power usage from roughly 30ma to 10ma
{
  //delay(ms);
  esp_sleep_enable_timer_wakeup(ms * 1000); // Time in microseconds
  esp_light_sleep_start();// 2. Enter Light Sleep
}

void loop()
{
  M5.update();
  if (M5.BtnA.wasPressed() || (millis() - lastChange > CHANGEAFTER)) {
    drawRandom();
  }
  
  if(tiles[current] == 11)
    drawToaster();
  else if(tiles[current] == 12)
    drawStars();

  myDelay(100);
}

void drawRandom()
{
  lastChange = millis();

  current++;
  if(current>MAX_TILE_VALUE)
    current = 0;
  int t = tiles[current];

  switch(t)
    {
      case 1: 
          drawHappyMac(FORE_COLOR, BACK_COLOR);
        break;
      case 2:
          drawImage(mac, sizeof(mac), MAC_WIDTH, MAC_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 3:
          drawImage(bloop, sizeof(bloop), BLOOP_WIDTH, BLOOP_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 4:
          drawImage(floppy, sizeof(floppy), FLOPPY_WIDTH, FLOPPY_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 5:
          drawImage(bomb, sizeof(bomb), BOMB_WIDTH, BOMB_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 6:
          drawImage(bin, sizeof(bin), BIN_WIDTH, BIN_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 7:
          drawImage(misc1, sizeof(misc1), MISC1_WIDTH, MISC1_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 8:
          drawImage(misc2, sizeof(misc2), MISC2_WIDTH, MISC2_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 9:
          drawImage(macwrite, sizeof(macwrite), MACWRITE_WIDTH, MACWRITE_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 10:
          drawImage(dogcow, sizeof(dogcow), DOGCOW_WIDTH, DOGCOW_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      case 11:
          initToasters();
        break;
      case 12:
          initStars();
        break;
      case 13:
          drawPng(hello, sizeof(hello), FORE_COLOR);
        break;
      case 14:
          drawPng(apple, sizeof(apple), BACK_COLOR);
        break;
      case 15:
          drawImage(ok, sizeof(ok), OK_WIDTH, OK_HEIGHT, 0, FORE_COLOR, BACK_COLOR);
        break;
      default: //0
          drawSadMac(FORE_COLOR, BACK_COLOR);
    }
}

/*void drawImage(const byte *image, int sz, int width, int height, int pixelsize, uint16_t FORE_COLOR, uint16_t BACK_COLOR)
{
  uint32_t COLOR_PRIMARY = M5.Display.color16to24(FORE_COLOR);
  uint32_t COLOR_BACKGROUND = M5.Display.color16to24(BACK_COLOR);
  M5.Display.fillScreen(COLOR_BACKGROUND);

  int screenwidth;
  int screenheight;

  if(pixelsize==0) // autosize, will give borders as can only be an integer
  {
    pixelsize = min( 128/width, 128/height);
  }
  screenwidth = width * pixelsize;
  screenheight = height * pixelsize;

  int xoffset = (128-screenwidth)/2; // centre on screen
  int yoffset = (128-screenheight)/2;

  for(int y=0;y<height;y++)
  {
    for(int x=0;x<width;x++)
    {
      int pos = (y*width) + x;
      int bytepos = pos / 8;
      if(bytepos>sz-1)
        continue;
      byte by = image[ bytepos];
      int  n = 7 - (pos % 8); //flip endian
      if( bitRead(by, n)==0 )
        M5.Display.fillRect(xoffset + (x*pixelsize), yoffset + (y*pixelsize), pixelsize, pixelsize, COLOR_PRIMARY);
    }
  }
}*/
void drawImage(const byte *image, int sz, int width, int height, int pixelsize, uint16_t FORE_COLOR, uint16_t BACK_COLOR)
{
  uint32_t COLOR_PRIMARY = M5.Display.color16to24(FORE_COLOR);
  uint32_t COLOR_BACKGROUND = M5.Display.color16to24(BACK_COLOR);
  M5.Display.fillScreen(COLOR_BACKGROUND);

  int screenwidth;
  int screenheight;

  if(pixelsize==0) // autosize, will give borders as can only be an integer
  {
    pixelsize = min( 128/width, 128/height);
  }
  screenwidth = width * pixelsize;
  screenheight = height * pixelsize;

  int xoffset = (128-screenwidth)/2; // centre on screen
  int yoffset = (128-screenheight)/2;

  for(int y=0;y<height;y++)
  {
    for(int x=0;x<width;x++)
    {
      int pos = (y*width) + x;
      int bytepos = pos / 8;
      if(bytepos>sz-1)
        continue;
      byte by = image[ bytepos];
      int  n = 7 - (pos % 8); //flip endian
      if( bitRead(by, n)==0 )
        M5.Display.fillRect(xoffset + (x*pixelsize), yoffset + (y*pixelsize), pixelsize, pixelsize, COLOR_PRIMARY);
    }
  }
}

void drawPng(const byte *image, int sz, uint16_t BACK_COLOR)
{
  
  canvas.createSprite(128, 128);
  canvas.fillRect(0, 0, 128, 128,  BACK_COLOR);
  canvas.drawPng(image, sz, 0, 0, 0, 0, 0, 0, 1.0f, 0.0f, middle_center) ;
  canvas.pushSprite(0,0);//"&M5.Lcd" is not necessary here
  canvas.deleteSprite();
}

void drawSadMac(uint16_t FORE_COLOR, uint16_t BACK_COLOR){
  int x_offset;
  uint32_t COLOR_PRIMARY = M5.Display.color16to24(FORE_COLOR);
  uint32_t COLOR_BACKGROUND = M5.Display.color16to24(BACK_COLOR);

  M5.Display.fillScreen(COLOR_BACKGROUND);
  // Right Eye
  x_offset = 0;
  M5.Display.fillRect(24 + x_offset, 31, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(39 + x_offset, 31, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(31 + x_offset, 39, 8, 7, COLOR_PRIMARY);
  M5.Display.fillRect(24 + x_offset, 46, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(39 + x_offset, 46, 7, 8, COLOR_PRIMARY);

  // Left Eye
  x_offset = 60;
  M5.Display.fillRect(24 + x_offset, 31, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(39 + x_offset, 31, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(31 + x_offset, 39, 8, 7, COLOR_PRIMARY);
  M5.Display.fillRect(24 + x_offset, 46, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(39 + x_offset, 46, 7, 8, COLOR_PRIMARY);

  // Nose
  M5.Display.fillRect(46, 61, 8, 8, COLOR_PRIMARY);
  M5.Display.fillRect(54, 69, 15, 7, COLOR_PRIMARY);
  M5.Display.fillRect(69, 61, 7, 8, COLOR_PRIMARY);

  // Mouse
  M5.Display.fillRect(39, 91, 7, 8, COLOR_PRIMARY);
  M5.Display.fillRect(46, 84, 38, 7, COLOR_PRIMARY);
  M5.Display.fillRect(84, 91, 15, 8, COLOR_PRIMARY);
  M5.Display.fillRect(99, 99, 7, 7, COLOR_PRIMARY);
}

void drawHappyMac(uint16_t FORE_COLOR, uint16_t BACK_COLOR){
  int x_offset;
  uint32_t COLOR_PRIMARY = M5.Display.color16to24(FORE_COLOR);
  uint32_t COLOR_BACKGROUND = M5.Display.color16to24(BACK_COLOR);

  M5.Display.fillScreen(COLOR_BACKGROUND);
  // Right Eye
  x_offset = 0;
  M5.Display.fillRect(31 + x_offset, 34, 8, 16, COLOR_PRIMARY);

  // Left Eye
  x_offset = 53;
  M5.Display.fillRect(31 + x_offset, 34, 8, 16, COLOR_PRIMARY);

  // Nose
  M5.Display.fillRect(61, 34, 8, 46, COLOR_PRIMARY);
  M5.Display.fillRect(53, 72, 8, 8, COLOR_PRIMARY);

  // Mouse
  M5.Display.fillRect(38, 86, 8, 8, COLOR_PRIMARY);
  M5.Display.fillRect(46, 94, 31, 8, COLOR_PRIMARY);
  M5.Display.fillRect(77, 86, 8, 8, COLOR_PRIMARY);
}