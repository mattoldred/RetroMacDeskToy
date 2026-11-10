
void drawImageSprite(const byte *image, int sz, int width, int height, int pixelsize, uint16_t FORE_COLOR, uint16_t BACK_COLOR, int xoffset, int yoffset)
{
  uint32_t COLOR_PRIMARY = M5.Display.color16to24(FORE_COLOR);
  uint32_t COLOR_BACKGROUND = M5.Display.color16to24(BACK_COLOR);

  int screenwidth;
  int screenheight;

  screenwidth = width * pixelsize;
  screenheight = height * pixelsize;

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
      if( bitRead(by, n)==1 )
        canvas.fillRect(xoffset + (x*pixelsize), yoffset + (y*pixelsize), pixelsize, pixelsize, COLOR_BACKGROUND);
    }
  }
}

#define numSprites 2
bool spriteType[numSprites]; //true = toast, false = toaster
int spriteX[numSprites];
int spriteY[numSprites];
int spriteSpeed[numSprites];
int spriteAni[numSprites];

void initSprite(int sprite)
{
  //Serial.println("Init Sprite");
  spriteType[sprite] = random(0,2)==1; // 0..1
  if(sprite==0)
  {
    spriteX[sprite] = 128;
    spriteY[sprite] = random(-10,44);
  }
  else
  {
    spriteX[sprite] = random(84, 128+10+1) - (spriteType[sprite] ? TOAST_WIDTH : TOASTER_WIDTH);
    spriteY[sprite] = -(spriteType[sprite] ? TOAST_HEIGHT : TOASTER_HEIGHT);
  }
  spriteSpeed[sprite] = random(1,4); // 1..3
}

void initToasters()
{
  initSprite(0);
  initSprite(1);
}

void updateSprite(int sprite)
{
  if( (spriteX[sprite]+(spriteType[sprite] ? TOAST_WIDTH : TOASTER_WIDTH))<0 || spriteY[sprite]>128)
  {
    initSprite(sprite);
  }
  else
  {
    spriteX[sprite] -= spriteSpeed[sprite];
    spriteY[sprite] += spriteSpeed[sprite];

    spriteAni[sprite]++;
    if(spriteAni[sprite]>6)
      spriteAni[sprite]=0;
  }
}

void drawSprite(int sprite)
{
  if(spriteType[sprite])
  {
    drawImageSprite(toast, sizeof(toast), TOAST_WIDTH, TOAST_HEIGHT, 1, FORE_COLOR, BACK_COLOR, spriteX[sprite], spriteY[sprite]);
  }
  else
  {
    int frame = spriteAni[sprite] >3 ? (6 - spriteAni[sprite]) : spriteAni[sprite];
    const byte *image;
    switch(frame)
    {
      case 0:
          image = toaster0;
        break;
      case 1:
          image = toaster1;
        break;
      case 2:
          image = toaster2;
        break;
      case 3:
          image = toaster3;
        break;
    }
    drawImageSprite(image, sizeof(toaster0), TOASTER_WIDTH, TOASTER_HEIGHT, 1, FORE_COLOR, BACK_COLOR, spriteX[sprite], spriteY[sprite]);
  }

}

//TOASTER
void drawToaster()
{
  canvas.createSprite(128, 128);
  canvas.fillRect(0, 0, 128, 128,  FORE_COLOR);

  for(int i=0;i<2;i++)
  {
    updateSprite(i);
    drawSprite(i);
  }

  //drawImageSprite(toast, sizeof(toast), TOAST_WIDTH, TOAST_HEIGHT, 1, FORE_COLOR, BACK_COLOR, spriteX, spriteY);

  canvas.pushSprite(0,0);//"&M5.Lcd" is not necessary here
  canvas.deleteSprite();
}
