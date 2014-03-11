//
//  SceneGraph.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#ifndef __Karaoke_Example__SceneGraph__
#define __Karaoke_Example__SceneGraph__

#include <iostream>
#include "y-entity.h"
#include "mo_gfx.h"


class SceneGraph
{
public:
    SceneGraph();
    virtual ~SceneGraph();
    
public:
    // cascade timestep simulation through system (as connected to this)
    void systemCascade();
    
public:
    // pause the simulation
    void pause();
    // resume the simulation
    void resume();
    // get is paused
    bool isPaused() const;
    
public:
    // set desired frame rate
    void setDesiredFrameRate( double frate );
    // get it
    double getDesiredFrameRate() const;
    // get the timestep in effect (fixed or dynamic)
    YTimeInterval delta() const;
    
public:
    // get the root
    YEntity & root() { return m_gfxRoot; }
    
protected:
    YEntity m_gfxRoot;
    
public:
    double m_desiredFrameRate;
    bool m_useFixedTimeStep;
    
public:
    YTimeInterval m_timeLeftOver;
    YTimeInterval m_simTime;
    YTimeInterval m_lastDelta;
    bool m_first;
    bool m_isPaused;
};

#endif /* defined(__Karaoke_Example__SceneGraph__) */
