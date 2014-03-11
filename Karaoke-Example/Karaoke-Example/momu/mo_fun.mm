/*----------------------------------------------------------------------------
 //-----------------------------------------------------------------------------
  MoMu: A Mobile Music Toolkit
  Copyright (c) 2010 Nicholas J. Bryan, Jorge Herrera, Jieun Oh, and Ge Wang
  All rights reserved.
    http://momu.stanford.edu/toolkit/
 
  Mobile Music Research @ CCRMA
  Music, Computing, Design Group
  Stanford University
    http://momu.stanford.edu/
    http://ccrma.stanford.edu/groups/mcd/
 
 MoMu is distributed under the following BSD style open source license:
 
 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The authors encourage users of MoMu to include this copyright notice,
 and to let us know that you are using MoMu. Any person wishing to 
 distribute modifications to the Software is encouraged to send the 
 modifications to the original authors so that they can be incorporated 
 into the canonical version.
 
 The Software is provided "as is", WITHOUT ANY WARRANTY, express or implied,
 including but not limited to the warranties of MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE and NONINFRINGEMENT.  In no event shall the authors
 or copyright holders by liable for any claim, damages, or other liability,
 whether in an actino of a contract, tort or otherwise, arising from, out of
 or in connection with the Software or the use or other dealings in the 
 software.
 -----------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// name: mo_fun.mm
// desc: MoMu useful functions
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//    date: Winter 2010
//    version: 1.0.0
//
// Mobile Music research @ CCRMA, Stanford University:
//     http://momu.stanford.edu/
//-----------------------------------------------------------------------------
#include "mo_fun.h"
#include <sstream>
using namespace std;


//-----------------------------------------------------------------------------
// name: rand2i()
// desc: generates random int in [low, high]
//-----------------------------------------------------------------------------
long MoFun::rand2i( long low, long high )
{
    // swap
    if( low > high ) low ^= high ^= low ^= high;
    // diff
    long diff = high - low;
    // hmm
    if( diff == 0 ) return low;
    // go
    return low + random() % diff;
}


//-----------------------------------------------------------------------------
// name: rand2f()
// desc: generates random double in [low, high]
//-----------------------------------------------------------------------------
double MoFun::rand2f( double low, double high )
{
    // swap
    if( low > high ) {
        double temp = low; low = high; high = temp;
    }
    
    // diff
    double diff = high - low;
    // go
    return low + diff * random() / (double)0x7fffffff;
    // return low + diff * random() / (double)0x8fffffff;
    
    // jieun: this function may be buggy... I tried to generate random numbers
    // between 0 and 1, and never seemed to generate numbers in certain range
    // ge: hmm changed from 0x8fffffff to 0x7fffffff
}




//-----------------------------------------------------------------------------
// name: freq2midi()
// desc: converts frequency to midi notenum 
//-----------------------------------------------------------------------------
double MoFun::freq2midi( double freq )
{
    return (12.0 * log2(freq/440.0) + 57);
}




//-----------------------------------------------------------------------------
// name: midi2freq()
// desc: converts midi notenum to frequency
//-----------------------------------------------------------------------------
double MoFun::midi2freq( double midi )
{
    return 440.0  * pow( 2.0, (midi - 57 ) / 12 );
}




//-----------------------------------------------------------------------------
// name: map()
// desc: map value in first range to value in second range
//-----------------------------------------------------------------------------
double MoFun::map( double value, double min1, double max1, double min2, double max2 )
{
    return ((value - min1) / (max1-min1)) * (max2 - min2) + min2;
}




//-----------------------------------------------------------------------------
// name: clamp()
// desc: clamp value
//-----------------------------------------------------------------------------
double MoFun::clamp( double value, double min, double max )
{
    // sanity check
    if( max < min )
    { double v = min; min = max; max = v; }

    if( value < min ) return min;
    if( value > max ) return max;
    
    return value;
}




//-----------------------------------------------------------------------------
// name: clampf()
// desc: clamp value
//-----------------------------------------------------------------------------
float MoFun::clampf( float value, float min, float max )
{
    // sanity check
    if( max < min )
    { float v = min; min = max; max = v; }
    
    if( value < min ) return min;
    if( value > max ) return max;
    
    return value;
}




//-----------------------------------------------------------------------------
// name: tokenize()
// desc: tokenize a line
//-----------------------------------------------------------------------------
void MoFun::tokenize( const string & line, vector<string> & results )
{
    // clear
    results.clear();
    
    // make a string stream
    istringstream ss( line );
    
    // a token
    string token;

    // tokenize
    while( ss >> token )
    {
        // append to results
        results.push_back( token );
    }
}




//-----------------------------------------------------------------------------
// name: scanForSubstrAtEnd()
// desc: scan a string for a substring from the end, support repetitions
//       (e.g., ... or !!! or ?!?!); hmm this might be easier with regex
//-----------------------------------------------------------------------------
static bool scanForSubstrAtEnd( const string & s, const string & substr, 
                                string & leftResult, string & rightResult )
{
    // get length of string
    int s_len = (int)s.length();
    int sub_len = (int)substr.length();
    string left = s, right;
    bool found = false;
    
    // clear
    leftResult = rightResult = "";
    
    // sanity check
    if( s_len < sub_len ) return false;
    if( substr == "" ) return false;

    // if s is substr
    if( s == substr )
    {
        // the substring
        rightResult = substr;
        // found
        return true;
    }

    // iterate
    do {
        // set
        found = false;
        // check end of string
        if( left.substr( s_len-sub_len, sub_len ) == substr )
        {
            // found
            left = left.substr( 0, s_len - sub_len );
            right += substr;
            found = true;
        }
    } while( found );
    
    // didn't find
    if( left == s ) return false;
    else
    {
        // the string
        leftResult = left;
        // the substr, possible repitition
        rightResult = right;
        // found
        return true;
    }
}




//-----------------------------------------------------------------------------
// name: tokenize()
// desc: tokenize a line, with punctuation set (e.g., "!", "...", etc.)
//-----------------------------------------------------------------------------
void MoFun::tokenize( const string & line, vector<string> & results,
                      const std::vector<string> & set )
{
    // clear
    results.clear();

    // make a string stream
    istringstream ss( line );

    // a token
    string token, left, right;
    bool found;

    // tokenize
    while( ss >> token )
    {
        // reset
        found = "";

        // go through set
        for( int i = 0; i < set.size(); i++ )
        {
            // check substr from the end
            found = scanForSubstrAtEnd( token, set[i], left, right );
            // check result
            if( found )
            {
                // push both
                results.push_back( left );
                results.push_back( right );
                // stop after the first one
                break;
            }
        }

        // if nothing found above
        if( !found )
        {
            // append to results
            results.push_back( token );
        }
    }
}
