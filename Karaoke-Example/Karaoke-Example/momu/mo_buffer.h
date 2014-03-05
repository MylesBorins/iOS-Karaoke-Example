/*----------------------------------------------------------------------------
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
// name: mo_buffer.h
// desc: MoMu buffers
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//
//    date: Spring 2012
//    version: 1.0.0
//
// Mobile Music research @ CCRMA, Stanford University:
//     http://momu.stanford.edu/
//-----------------------------------------------------------------------------
#ifndef __MO_BUFFER_H__
#define __MO_BUFFER_H__

#include <iostream>




//-----------------------------------------------------------------------------
// name: class MoCircleBuffer
// desc: templated circular buffer class
//-----------------------------------------------------------------------------
template <typename T>
class MoCircleBuffer
{
public:
    MoCircleBuffer( long length = 0 );
    ~MoCircleBuffer();

public:
    // reset length of buffer (capacity)
    void init( long length );
    // get length
    long length() const;
    // clear (does no explicit memory management)
    void clear();
    
public:
    // put an element into the buffer - the item will be copied
    // NOTE: if over-capacity, will discard least recently put item
    void put( const T & item );
    // number of valid elements in buffer
    long numElements() const;
    // are there more elements?
    bool more() const;
    // get elements without advancing - returns number of valid elements
    long peek( T * array, long numItems, unsigned long stride = 0 );
    // pop
    long pop( long numItems = 1 );

protected: // helper functions
    inline void advanceWrite();
    inline void advanceRead();
    
protected:
    // the buffer
    T * m_buffer;
    // the buffer length (capacity)
    long m_length;
    // write index
    long m_writeIndex;
    // read index
    long m_readIndex;
    // num elements
    long m_numElements;
};




//-----------------------------------------------------------------------------
// name: MoCircleBuffer()
// desc: constructor
//-----------------------------------------------------------------------------
template <typename T>
MoCircleBuffer<T>::MoCircleBuffer( long length )
{
    // zero out first
    m_buffer = NULL;
    m_length = m_readIndex = m_writeIndex = m_numElements = 0;
    
    // call init
    this->init( length );
}




//-----------------------------------------------------------------------------
// name: ~MoCircleBuffer
// desc: destructor
//-----------------------------------------------------------------------------
template <typename T>
MoCircleBuffer<T>::~MoCircleBuffer()
{
    
}




//-----------------------------------------------------------------------------
// name: init()
// desc: reset length of buffer
//-----------------------------------------------------------------------------
template <typename T>
void MoCircleBuffer<T>::init( long length )
{
    // clean up is necessary
    if( m_buffer )
    {
        // delete array - should call destructors and zero out variable
        SAFE_DELETE_ARRAY( m_buffer );
        // zero out
        m_length = m_readIndex = m_writeIndex = m_numElements = 0;
    }
    
    // sanity check
    if( length < 0 )
    {
        // doh
        std::cerr << "[MoCircleBuffer]: error invalid length '" 
                  << length << "' requested" << std::endl;
        
        return;
    }
    
    // check for zero length
    if( length == 0 ) return;
    
    // allocate
    m_buffer = new T[length];
    // check
    if( m_buffer < 0 )
    {
        // doh
        std::cerr << "[MoCircleBuffer]: failed to allocate buffer of length '" 
                  << length << "'..." << std::endl;

        return;
    }
    
    // save
    m_length = length;
    // zero out
    m_readIndex = m_writeIndex = m_numElements = 0;
}




//-----------------------------------------------------------------------------
// name: length()
// desc: get length
//-----------------------------------------------------------------------------
template <typename T>
long MoCircleBuffer<T>::length() const
{
    return m_length;
}




//-----------------------------------------------------------------------------
// name: clear()
// desc: clear (does no explicit memory management)
//-----------------------------------------------------------------------------
template <typename T>
void MoCircleBuffer<T>::clear()
{
    // zero out
    m_readIndex = m_writeIndex = m_numElements = 0;
}




//-----------------------------------------------------------------------------
// name: advanceWrite()
// desc: helper to advance write index
//-----------------------------------------------------------------------------
template <typename T>
void MoCircleBuffer<T>::advanceWrite()
{
    // increment
    m_writeIndex++;
    // increment count
    m_numElements++;
    
    // check for bounds
    if( m_writeIndex >= m_length )
    {
        // wrap
        m_writeIndex -= m_length;
    }
}




//-----------------------------------------------------------------------------
// name: advanceRead()
// desc: helper to advance read index
//-----------------------------------------------------------------------------
template <typename T>
void MoCircleBuffer<T>::advanceRead()
{
    // increment
    m_readIndex++;
    // decrement count
    m_numElements--;
    
    // check for bounds
    if( m_readIndex >= m_length )
    {
        // wrap
        m_readIndex -= m_length;
    }
}




//-----------------------------------------------------------------------------
// name: put()
// desc: put an element into the buffer - the item will be copied
//       if over-capacity, will discard least recently put item
//-----------------------------------------------------------------------------
template <typename T>
void MoCircleBuffer<T>::put( const T & item )
{
    // sanity check
    if( m_buffer == NULL ) return;
    
    // copy it
    m_buffer[m_writeIndex] = item;
    
    // advance write index
    advanceWrite();
    
    // if read and write pointer are the same, over-capacity
    if( m_writeIndex == m_readIndex )
    {
        // advance read!
        advanceRead();
    }
}




//-----------------------------------------------------------------------------
// name: numElements()
// desc: get number of valid elements in buffer
//-----------------------------------------------------------------------------
template <typename T>
long MoCircleBuffer<T>::numElements() const
{
    // return our count
    return m_numElements;
}




//-----------------------------------------------------------------------------
// name: hasMore()
// desc: are there more elements?
//-----------------------------------------------------------------------------
template <typename T>
bool MoCircleBuffer<T>::more() const
{
    return m_numElements > 0;
}




//-----------------------------------------------------------------------------
// name: peek
// desc: get elements without advancing - returns number returned
//-----------------------------------------------------------------------------
template <typename T>
long MoCircleBuffer<T>::peek( T * array, long numItems, unsigned long stride )
{
    // sanity check
    if( m_buffer == NULL ) return 0;
    
    // sanity check (so the wrap can be sure to land inbounds)
    if( stride >= m_length ) return 0;
    
    // count
    long count = 0;
    // actual count, taking stride out of the equation
    long actualCount = 0;
    
    // starting index
    long index = m_readIndex;
    
    // while need more but haven't reached write index...
    while( (count < numItems) && (count < m_numElements) )
    {
        // copy
        array[actualCount] = m_buffer[index];
        // increment
        count++; count += stride;
        // advance
        index++; index += stride;
        // actual count, don't stride
        actualCount++;
        // wrap
        if( index >= m_length ) index -= m_length;
    }
    
    return actualCount;
}




//-----------------------------------------------------------------------------
// name: pop()
// desc: pop one or more elements
//-----------------------------------------------------------------------------
template <typename T>
long MoCircleBuffer<T>::pop( long numItems )
{
    // sanity check
    if( m_buffer == NULL ) return 0;
    
    // count
    long count = 0;
    
    // while there is more to pop and need to pop more
    while( more() && count < numItems )
    {
        // advance read
        advanceRead();
        // increment count
        count++;
    }
    
    return count;
}




#endif
