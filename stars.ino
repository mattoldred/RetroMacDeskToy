//https://forum.arduino.cc/t/starsimulation-sh1106-i2c-oled-animation/393311

int hCenter = 64;                         // horizontal center of animation
int vCenter = 64;                         // vertical center of animation
int Hstar, Vstar;                         // star location currently
#define Quantity 50                        // number of stars
int StarProgress[Quantity];                     // array that tracks progress of each star
int StarAngle[Quantity];                        // array that tracks angle of each star
int interval[Quantity];                         // array used to track speed of each star

static const unsigned char PROGMEM stievenart[40] = {0x37, 0x5D, 0x5D, 0x24, 0xCE, 0x42, 0x51, 0x51, 0xAA, 0xA4, 0x22, 0x59, 0x59, 0xEE, 0xC4, 0x12, 
                                                     0x51, 0x51, 0x6A, 0xA4, 0x62, 0x5C, 0x9D, 0x2A, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void initStars()
{
  for (int i = 0; i < Quantity; i++) {    // loop to setup initial stars
    StarAngle[i] = random(360);           // generates random angle for each star
    interval[i] = (random(5))+1;          // generates random speed for each star +1 keeps stars from being zero
  }
}

void drawStars()
{
  unsigned long currentMillis = millis();

  canvas.createSprite(128, 128);
  canvas.fillRect(0, 0, 128, 128,  FORE_COLOR);

  //canvas.drawBitmap(44, 58, stievenart, 40, 8, WHITE);

  for (int i = 0; i < Quantity; i++)                                              // loop to draw each star
  {
    if (StarProgress[i] > 100)                                                    // if statement to reset stars after going off screen
    {
      StarProgress[i] = StarProgress[i] - 100;                                    // resets star progress
      StarAngle[i] = random(360);                                                 // resets star angle
      interval[i] = (random(5))+1;                                                // resets star speed
    }
    else
    {
      StarProgress[i] = StarProgress[i] + interval[i];                            // distance traveled for star
    }

    if (currentMillis - StarProgress[i] >= interval[i] && StarProgress[i] > 0)    // if statement to exclude some star paths
    {
      Hstar = (hCenter + (sin(StarAngle[i] / 57.296) * StarProgress[i] ));        // calculates star horizontal coordinate
      Vstar = (vCenter + (cos(StarAngle[i] / 57.296) * StarProgress[i] ));        // calculates star horizontal coordinate
      canvas.drawPixel(Hstar, Vstar, WHITE);                              // draws each star
    }
  }
  canvas.drawPng(colony, sizeof(colony), 0, 0, 0, 0, 0, 0, 1.0f, 0.0f, middle_center) ;
  canvas.pushSprite(0,0);//"&M5.Lcd" is not necessary here
  canvas.deleteSprite();
}