//
//  Simulation.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#ifndef __Karaoke_Example__Simulation__
#define __Karaoke_Example__Simulation__

#include <iostream>
#include "Globals.h"
#import "mo_gfx.h"
#import "y-entity.h"

class Simulation : public YEntity
{
    
public:
    // get the root
    YEntity & root() { return _root; }
protected:
    YEntity _root;
    
public:
    // cascade timestep simulation through system (as connected to this)
    static void init();
    static void systemCascade(YTimeInterval dt);
    
};

#endif /* defined(__Karaoke_Example__Simulation__) */
