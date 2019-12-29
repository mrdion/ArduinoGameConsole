// running on an atmega328

#include "synth.h"
#include "musictrack.h"


int samplecounter;
volatile bool tickstart;

void setup() 
{
    samplecounter = 0;
    tickstart = false;
    
    Synth::init(4);
    Synth::setNotify(countsamples);
} 

void countsamples()
{
    if (samplecounter<311)  { samplecounter++; }
    else  {    samplecounter=0;  }
    tickstart = (samplecounter<100);
}

void waittick()
{       
    while(tickstart) {}
    while (!tickstart) {}
}

void loop()
{
    Musictrack part1[4] = {
        //                                 |               |               |               |               |               |               |               |               |
        Musictrack(0, 90,  TRIANGLE, 5, F("q---l-m-o-qom-l-j---j-m-q---o-m-l---llm-o---q---m---j---j-------o---o-r-v---t-r-q---q-m-q---o-m-l---llm-o---q---m---j---j------ ")),
        Musictrack(1, 0,   TRIANGLE, 5, F("                                                                                                                                ")),
        Musictrack(2, 80,  SINE,     5, F("Q-- Q-- Q-- Q-- V-- V-- V-- V-- U-- U-- Q-- Q-- V-- V-- V-- a-- O-- O-- O-- O-- M-- M-- M-- M-- H-- H-- Q-- Q-- V-- V-- V------ ")),
        Musictrack(3, 70,  SINE,     5, F("  e-- e-- e-- e-- j-- j-- j-- j-- j-- j-- e-- e-- j-- j-- X-- c-- c-- c-- c-- c-- a-- a-- a-- a-- T-- T-- e-- e-- j-- j-- j--   "))
    };
    Musictrack part2[4] = {
        //                                 |               |               |               |               |               |               |               |               | 
        Musictrack(0, 70, TRIANGLE,  5, F("m-------j-------l-------i-------j-------e-------e-------i-------m-------j-------l-------i-------j---m---q---q---q-------------- ")),
        Musictrack(1, 70, TRIANGLE,  5, F("q-------m-------o-------l-------m-------j-------i-------l-------q-------m-------o-------l-------m---q---v---v---u-------------- ")),
        Musictrack(2, 60, SINE,      5, F("V-- V-- V-- V-- X-- X-- X-- X-- V-- V-- V-- V-- U-- U-- U-- U-- V-- V-- V-- V-- X-- X-- X-- X-- V-- V-- V-- V-- U-- U-- U-- U-- ")),
        Musictrack(3, 60, SINE,      5, F("  e-- e-- e-- e-- c-- c-- c-- c-- e-- e-- e-- e-- e-- e-- e-- e-- e-- e-- e-- e-- c-- c-- c-- c-- e-- e-- e-- e-- e-- e--       "))
    };
    Song song(4, part1, part1, part2, part1);

    waittick();
    song.start();
    while (song.tick()) { waittick(); }

    for (;;) {}
}