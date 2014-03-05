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

class Audio
{
public:
    // init audio
    static bool init( );
    // start audio
    static bool start();
};

#endif /* defined(__Karaoke_Example__Audio__) */
