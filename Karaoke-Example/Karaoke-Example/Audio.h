//
//  Audio.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/4/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#ifndef __Karaoke_Example__Audio__
#define __Karaoke_Example__Audio__

#include <iostream>
#include "Globals.h"
#import "mo_audio.h"
#import "y-score-reader.h"
#import <vector>
#import "Mandolin.h"
#import "mo_fun.h"
class Audio
{
public:
    // init audio
    static bool init( );
    // start audio
    static bool start();
public:
    static YScoreReader * _scoreReader;
    static stk::Mandolin ** _mandolins;
};

#endif /* defined(__Karaoke_Example__Audio__) */
