#define delayTime      200
#define kbdDelay       60
#define dataLength     18
#define incrementSize  3

#define R              P3_0_bit
#define G              P3_1_bit
#define B              P3_2_bit
#define Y              P3_3_bit


sbit Sound_Play_Pin at P1_7_bit;


unsigned char start = 0;
unsigned char Length = incrementSize;
unsigned char track[dataLength];
const unsigned char LEDs[4] = {0x01, 0x02, 0x04, 0x08};


void setup();
unsigned char readSwitches();
void playMelody();
void getMelody();
void playDemo();
void playSimons();
void playTone(unsigned char tone);


void main()
{
     setup();
     while(1)
     {
             playSimons();
     };
}


void setup()
{
  start = 0;
  P1 = 0x00;
  P3 = 0xFF;
  sound_init();
}


unsigned char readSwitches()
{
  if(!R)
  {
    delay_ms(kbdDelay);
    if(!R)
    {
      P1_0_bit = 0;
      Sound_Play(1200, delayTime);
      P1_0_bit = 1;
      delay_ms(delayTime);
      while(!R);
      return 0;
    }
  }
  else if(!G)
  {
    delay_ms(kbdDelay);
    if(!G)
    {
      P1_1_bit = 0;
      Sound_Play(1600, delayTime);
      P1_1_bit = 1;
      delay_ms(delayTime);
      while(!G);
      return 1;
    }
  }
  else if(!B)
  {
    delay_ms(kbdDelay);
    if(!B)
    {
      P1_2_bit = 0;
      Sound_Play(2200, delayTime);
      P1_2_bit = 1;
      delay_ms(delayTime);
      while(!B);
      return 2;
    }
  }
  else if(!Y)
  {
    delay_ms(kbdDelay);
    if(!Y)
    {
      P1_3_bit = 0;
      Sound_Play(2600, delayTime);
      P1_3_bit = 1;
      delay_ms(delayTime);
      while(!Y);
      return 3;
    }
  }
}


void playMelody()
{
    unsigned char s = 0;
    unsigned char index = 0;
    memset(track, 0x00, dataLength);
    for(s = 0; s < Length; s += 1)
    {
      index = (unsigned char)(rand() / 8100);
      index = (index & 0x03);
      track[s] = index;
      P1 = (~(LEDs[index]));
      playTone(index);
      P1 = 0x0F;
      delay_ms(delayTime);
    }
    delay_ms(delayTime);
}


void getMelody()
{
    unsigned char s = 0;
    unsigned char bt = 0;
    unsigned char winLoss = 0;
    while(s < Length)
    {
      while((readSwitches() != 0) && (readSwitches() != 1) && (readSwitches() != 2) && (readSwitches() != 3));
      bt = readSwitches();
      while((readSwitches() == 0) && (readSwitches() == 1) && (readSwitches() == 2) && (readSwitches() == 3));
      delay_ms(delayTime);
      if(bt == (track[s]))
      {
          ++winLoss;
      }
      else
      {
          winLoss = 0;
          break;
      }
      s += 1;
    }
    delay_ms(delayTime);
    delay_ms(delayTime);
    if(winLoss == Length)
    {
        for(s = 0; s < 3; s += 1)
        {
            P1 = 0x00;
            sound_play(3000, 90);
            P1 = 0x0F;
            sound_play(2000, 90);
        }
        Length += incrementSize;
    }
    else if((winLoss == Length) && (Length == dataLength))
    {
        for(s = 0; s < 6; s += 1)
        {
            P1 = (~(0x06));
            sound_play(2000, 60);
            sound_play(1300, 60);
            P1 = 0x0F;
            sound_play(2900, 60);
        }
        Length = incrementSize;
        start = 0;
    }
    else
    {
        for(s = 0; s < 3; s += 1)
        {
            P1 = (~(0x09));
            sound_play(1000, 40);
            sound_play(3300, 40);
            P1 = 0x0F;
            sound_play(2400, 40);
        }
        Length = incrementSize;
        start = 0;
    }
    delay_ms(400);
}


void playDemo()
{
    unsigned char s = 0;
    for(s = 0; s < 4; s++)
    {
        P1 = (~(LEDs[s]));
        delay_ms(delayTime);
        P1 = 0xFF;
        delay_ms(delayTime);
        if(!R || !B || !G || !Y)
        {
          while(!R || !B || !G || !Y)
          {
                P1 = 0x00;
          }
          P1 = 0xFF;
          start = 1;
          delay_ms(600);
          break;
        }
    }
}

void playSimons()
{
    switch(start)
    {
      case 1:
      {
        playMelody();
        getMelody();
        break;
      }
      default:
      {
        playDemo();
        break;
      }
    }
}


void playTone(unsigned char tone)
{
   switch(tone)
   {
               case 1:
               {
                    Sound_Play(1600, delayTime);
                    break;
               }
               case 2:
               {
                    Sound_Play(2200, delayTime);
                    break;
               }
               case 3:
               {
                    Sound_Play(2600, delayTime);
                    break;
               }
               default:
               {
                    Sound_Play(1200, delayTime);
                    break;
               }
   }
}
