//
//  SceneGraph.cpp
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#include "SceneGraph.h"
#include "Simulation.h"

using namespace std;




// up to max fps steps per second, e.g., 1./60
#define STEPTIME (1.0 / getDesiredFrameRate())
// max sim step size in seconds
#define SIM_SKIP_TIME (.25)




//-------------------------------------------------------------------------------
// name: SceneGraph()
// desc: constructor
//-------------------------------------------------------------------------------
SceneGraph::SceneGraph()
{
    m_desiredFrameRate = 60;
    m_useFixedTimeStep = false;
    m_timeLeftOver = 0;
    m_simTime = 0;
    m_lastDelta = 0;
    m_first = true;
    m_isPaused = false;
}




//-------------------------------------------------------------------------------
// name: ~SceneGraph()
// desc: ...
//-------------------------------------------------------------------------------
SceneGraph::~SceneGraph()
{
    // nothing to do
}




//-------------------------------------------------------------------------------
// name: triggerUpdates()
// desc: trigger system wide update with time steps
//-------------------------------------------------------------------------------
void SceneGraph::systemCascade()
{
    // get current time (once per frame)
    MoGfx::getCurrentTime( true );
    
    // Timing loop
    // TODO THIS MIGHT NEED TO CHANGE
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    
    YTimeInterval timeElapsed = MoGfx::getCurrentTime(true) - m_simTime;
    
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    
    m_simTime += timeElapsed;
    
    // special case: first update
    if( m_first )
    {
        // set time just enough for one update
        timeElapsed = STEPTIME;
        // set flag
        m_first = false;
    }
    
    // clamp it
    if( timeElapsed > SIM_SKIP_TIME )
        timeElapsed = SIM_SKIP_TIME;
    
    // update it
    // check paused
    if( !m_isPaused )
    {
        Simulation::systemCascade( timeElapsed );
        // update the world with a fixed timestep
        m_gfxRoot.updateAll( timeElapsed );
    }
    
    // redraw
    m_gfxRoot.drawAll();
    
    // set
    m_lastDelta = timeElapsed;
}





//-------------------------------------------------------------------------------
// pause the simulation
//-------------------------------------------------------------------------------
void SceneGraph::pause() { m_isPaused = true; }
//-------------------------------------------------------------------------------
// resume the simulation
//-------------------------------------------------------------------------------
void SceneGraph::resume() { m_isPaused = false; }
//-------------------------------------------------------------------------------
// get is paused
//-------------------------------------------------------------------------------
bool SceneGraph::isPaused() const { return m_isPaused; }
//-------------------------------------------------------------------------------
// set desired frame rate
//-------------------------------------------------------------------------------
void SceneGraph::setDesiredFrameRate( double frate ) { m_desiredFrameRate = frate; }
//-------------------------------------------------------------------------------
// get it
//-------------------------------------------------------------------------------
double SceneGraph::getDesiredFrameRate() const { return m_desiredFrameRate; }
//-------------------------------------------------------------------------------
// get the timestep in effect (fixed or dynamic)
//-------------------------------------------------------------------------------
YTimeInterval SceneGraph::delta() const
{ return m_lastDelta; }
