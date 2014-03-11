/***************************************************/
/*! \class TwoPole
    \brief STK two-pole filter class.

    This class implements a two-pole digital filter.  A method is
    provided for creating a resonance in the frequency response while
    maintaining a nearly constant filter gain.

    by Perry R. Cook and Gary P. Scavone, 1995 - 2010.
*/
/***************************************************/

#include "TwoPole.h"
#include <cmath>

namespace stk {

TwoPole :: TwoPole( void )
{
  b_.resize( 1 );
  a_.resize( 3 );
  inputs_.resize( 1, 1, 0.0 );
  outputs_.resize( 3, 1, 0.0 );
  b_[0] = 1.0;
  a_[0] = 1.0;

  Stk::addSampleRateAlert( this );
}

TwoPole :: ~TwoPole()
{
  Stk::removeSampleRateAlert( this );
}

void TwoPole :: sampleRateChanged( StkFloat newRate, StkFloat oldRate )
{
  if ( !ignoreSampleRateChange_ ) {
    errorString_ << "TwoPole::sampleRateChanged: you may need to recompute filter coefficients!";
    handleError( StkError::WARNING );
  }
}

void TwoPole :: setResonance( StkFloat frequency, StkFloat radius, bool normalize )
{
  a_[2] = radius * radius;
  a_[1] = (StkFloat) -2.0 * radius * cos(TWO_PI * frequency / Stk::sampleRate());

  if ( normalize ) {
    // Normalize the filter gain ... not terribly efficient.
    StkFloat real = 1 - radius + (a_[2] - radius) * cos(TWO_PI * 2 * frequency / Stk::sampleRate());
    StkFloat imag = (a_[2] - radius) * sin(TWO_PI * 2 * frequency / Stk::sampleRate());
    b_[0] = sqrt( pow(real, 2) + pow(imag, 2) );
  }
}

void TwoPole :: setCoefficients( StkFloat b0, StkFloat a1, StkFloat a2, bool clearState )
{
  b_[0] = b0;
  a_[1] = a1;
  a_[2] = a2;

  if ( clearState ) this->clear();
}

} // stk namespace
