// running on an atmega328

#include "av.h"
#include "musictrack.h"

static const uint8_t PROGMEM waveSine[] = 
{
    127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,
    176,179,182,184,187,190,193,195,198,200,203,205,208,210,213,215,
    217,219,221,224,226,228,229,231,233,235,236,238,239,241,242,244,
    245,246,247,248,249,250,251,251,252,253,253,254,254,254,254,254,
    255,254,254,254,254,254,253,253,252,251,251,250,249,248,247,246,
    245,244,242,241,239,238,236,235,233,231,229,228,226,224,221,219,
    217,215,213,210,208,205,203,200,198,195,193,190,187,184,182,179,
    176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,
    127,124,121,118,115,111,108,105,102, 99, 96, 93, 90, 87, 84, 81,
     78, 75, 72, 70, 67, 64, 61, 59, 56, 54, 51, 49, 46, 44, 41, 39,
     37, 35, 33, 30, 28, 26, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10,
      9,  8,  7,  6,  5,  4,  3,  3,  2,  1,  1,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,  8,
      9, 10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 26, 28, 30, 33, 35,
     37, 39, 41, 44, 46, 49, 51, 54, 56, 59, 61, 64, 67, 70, 72, 75,
     78, 81, 84, 87, 90, 93, 96, 99,102,105,108,111,115,118,121,124
};
static const uint8_t PROGMEM waveTriangle[] = 
{
      0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 
     32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
     64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
     96, 98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,
    128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,
    160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,
    192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,
    224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,
    255,253,251,249,247,245,243,241,239,237,235,233,231,229,227,225,
    223,221,219,217,215,213,211,209,207,205,203,201,199,197,195,193,
    191,189,187,185,183,181,179,177,175,173,171,169,167,165,163,161,
    159,157,155,153,151,149,147,145,143,141,139,137,135,133,131,129,
    127,125,123,121,119,117,115,113,111,109,107,105,103,101, 99, 97,
     95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65,
     63, 61, 59, 57, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33,
     31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11,  9,  7,  5,  3,  1
};

static const uint8_t PROGMEM screenTiles[] = 
{
#include "tetris_tiles.h"
};

static const uint8_t PROGMEM pieceShapes[10*16] = 
{
    // The J piece
    0, 0, 0, 0,
    0, 0, TILE_J, 0,
    0, 0, TILE_J, 0,
    0, TILE_J, TILE_J, 0, 
    // The L piece
    0, 0, 0, 0,
    0, TILE_L, 0, 0,
    0, TILE_L, 0, 0,
    0, TILE_L, TILE_L, 0,
    // The O piece
    0, 0, 0, 0,
    0, TILE_O, TILE_O, 0,
    0, TILE_O, TILE_O, 0,
    0, 0, 0, 0,
    // The S piece
    0, 0, 0, 0,
    0, TILE_S, TILE_S, 0,
    TILE_S, TILE_S, 0, 0,
    0, 0, 0, 0,
    // The T piece
    0, 0, 0, 0,
    TILE_T, TILE_T, TILE_T, 0,
    0, TILE_T, 0, 0,
    0, 0, 0, 0,
    // The Z piece
    0, 0, 0, 0,
    TILE_Z, TILE_Z, 0, 0,
    0, TILE_Z, TILE_Z, 0,
    0, 0, 0, 0,
    // The I piece
    0, TILE_I0, 0, 0,
    0, TILE_I1, 0, 0,
    0, TILE_I2, 0, 0,
    0, TILE_I3, 0, 0,
    // 2. appearance of the I piece
    0, TILE_H3, 0, 0,
    0, TILE_H2, 0, 0,
    0, TILE_H1, 0, 0,
    0, TILE_H0, 0, 0,
    // 3. appearance of the I piece
    0, TILE_I3, 0, 0,
    0, TILE_I2, 0, 0,
    0, TILE_I1, 0, 0,
    0, TILE_I0, 0, 0,
    // 4. appearance of the I piece
    0, TILE_H0, 0, 0,
    0, TILE_H1, 0, 0,
    0, TILE_H2, 0, 0,
    0, TILE_H3, 0, 0,
};



#define PFWIDTH 10
#define PFHEIGHT 16
uint8_t playfield[PFWIDTH][PFHEIGHT];


void setup()
{
    DDRC = 0;     // use port c for inputs
    PORTC = 0x3f; // weak pull-up
    DDRB = 0;     // use port b for inputs
    PORTB = 0x18; // weak pull-ups 
      
    av_init(screenTiles, waveSine); 
}
void loop() 
{  
    paintBackground();
    
    Musictrack part1[4] = {
        //                                 |               |               |               |               |               |               |               |               |
        Musictrack(voiceA, 90, waveTriangle, 5, F("q---l-m-o-qom-l-j---j-m-q---o-m-l---llm-o---q---m---j---j-------o---o-r-v---t-r-q---q-m-q---o-m-l---llm-o---q---m---j---j------ ")),
        Musictrack(voiceB, 0,  waveTriangle, 5, F("                                                                                                                                ")),
        Musictrack(voiceC, 80, waveSine,     5, F("Q-- Q-- Q-- Q-- V-- V-- V-- V-- U-- U-- Q-- Q-- V-- V-- V-- a-- O-- O-- O-- O-- M-- M-- M-- M-- H-- H-- Q-- Q-- V-- V-- V------ ")),
        Musictrack(voiceD, 70, waveSine,     5, F("  e-- e-- e-- e-- j-- j-- j-- j-- j-- j-- e-- e-- j-- j-- X-- c-- c-- c-- c-- c-- a-- a-- a-- a-- T-- T-- e-- e-- j-- j-- j--   "))
    };
    Musictrack part2[4] = {
        //                                 |               |               |               |               |               |               |               |               | 
        Musictrack(voiceA, 70, waveTriangle, 5, F("m-------j-------l-------i-------j-------e-------e-------i-------m-------j-------l-------i-------j---m---q---q---q-------------- ")),
        Musictrack(voiceB, 70, waveTriangle, 5, F("q-------m-------o-------l-------m-------j-------i-------l-------q-------m-------o-------l-------m---q---v---v---u-------------- ")),
        Musictrack(voiceC, 60, waveSine,     5, F("V-- V-- V-- V-- X-- X-- X-- X-- V-- V-- V-- V-- U-- U-- U-- U-- V-- V-- V-- V-- X-- X-- X-- X-- V-- V-- V-- V-- U-- U-- U-- U-- ")),
        Musictrack(voiceD, 60, waveSine,     5, F("  e-- e-- e-- e-- c-- c-- c-- c-- e-- e-- e-- e-- e-- e-- e-- e-- e-- e-- e-- e-- c-- c-- c-- c-- e-- e-- e-- e-- e-- e--       "))
    };
    Song song(4, part1, part1, part2, part1);

    av_waitForBlanking();
    song.start();

    byte timer=0;
    byte rot=0;
    byte i,b;
    
    for (;;) 
    {
        if (!song.tick()) { song.start(); } 
        av_waitForBlanking(); 
        
        if (timer<50)
        {   timer++;
        }
        else
        {   byte typ;

            timer = 0;
            rot = (rot+1) % 4;

            clearPlayfield();
            for (typ=0; typ<6; typ++)
            {   setPiece(1 + (typ%2)*4, (typ/2)*4, typ, rot, false); 
            }
            setPiece(3,12, 6, rot, false);
        }
        
        paintPlayfield();
        b = 0;
        b = getButtons();
        for (i=0; i<8; i++)
        {
           videoMatrix[20*i] = ((b&0x01)==0) ? TILE_WHITE : TILE_BLACK;
            b = b >> 1;
        }
    }
}

byte getButtons()
{
    return (PINC&0x3f) | ((PINB&0x18) << 3);
}


void paintBackground()
{
    int i,j;
    for (i=0; i<320; i++) { videoMatrix[i] = TILE_BLACK; }
    for (i=0; i<16; i++) 
    {   
        videoMatrix[i*20+1] = TILE_WALL;
        for (j=0; j<10; j++) { videoMatrix[i*20+2+j] = TILE_WHITE; }
        videoMatrix[i*20+12] = TILE_WALL;       
    }

    paintBox(2*20+13,2);
    paintBox(6*20+13,2);   
    paintBox(10*20+13,4);    
}

void paintBox(int i, byte h)
{
    byte j;
  
    videoMatrix[i] = TILE_BOX_LT;
    videoMatrix[i+1] = TILE_BOX_T;
    videoMatrix[i+2] = TILE_BOX_T;
    videoMatrix[i+3] = TILE_BOX_T;
    videoMatrix[i+4] = TILE_BOX_T;
    videoMatrix[i+5] = TILE_BOX_T;
    videoMatrix[i+6] = TILE_BOX_RT;
    i+=20;
    for (j=0; j<h; j++)
    {
        videoMatrix[i] = TILE_BOX_L;
        videoMatrix[i+1] = TILE_WHITE;
        videoMatrix[i+2] = TILE_WHITE;
        videoMatrix[i+3] = TILE_WHITE;
        videoMatrix[i+4] = TILE_WHITE;
        videoMatrix[i+5] = TILE_WHITE;
        videoMatrix[i+6] = TILE_BOX_R;
        i+=20;
    }
    videoMatrix[i] = TILE_BOX_LB;
    videoMatrix[i+1] = TILE_BOX_B;
    videoMatrix[i+2] = TILE_BOX_B;
    videoMatrix[i+3] = TILE_BOX_B;
    videoMatrix[i+4] = TILE_BOX_B;
    videoMatrix[i+5] = TILE_BOX_B;
    videoMatrix[i+6] = TILE_BOX_RB;
}

void paintPlayfield()
{
    byte x=0; 
    byte y=0;
    for (x=0; x<PFWIDTH; x++)
    {
        for (y=0; y<PFHEIGHT; y++)
        {
            videoMatrix[20*y+x+2] = playfield[x][y];
        }      
    }
}


void clearPlayfield()
{
    byte x,y;
    for (x=0; x<PFWIDTH; x++)
    {
      for (y=0; y<PFHEIGHT; y++)
      {
          playfield[x][y] = 0;
      }
    }
}

bool setPiece(byte x, byte y, byte typ, byte rot, bool onlyTest)
{
    byte i;
    byte tx=0;
    byte ty=0;

    // use different appearance for I-tile
    if (typ==6) { typ+=rot; }
    
    for (i=0; i<16; i++)
    { 
        byte tile = pgm_read_byte(pieceShapes+(typ*16+i));
        switch (rot)
        {   case 0:  tx = x + i%4; ty = y + i/4; break;
            case 1:  ty = y + i%4; tx = x + 3 - i/4; break;
            case 2:  tx = x + 3 - i%4; ty = y + 3 - i/4; break;
            case 3:  ty = y + 3 - i%4; tx = x+ i/4; break;
        }
        
        if (tile!=0)     
        {   if (onlyTest)
            {   if (playfield[tx][ty]!=0) { return false; }
            }    
            else
            {   playfield[tx][ty] = tile;                
            }
        }   
    }
    return true;
}