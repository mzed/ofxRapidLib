//
//  RingBufferAny.hpp
//  Can be used as a non-blocking eventpoll/ringbuffer without having to write unions etc ahead of time
//  Allowing rapid expansion of inter-thread communication with several datatypes being stored in the same queue.
//  Created by James Frink on 16/12/2017.
//  Copyright © 2017 James Frink. All rights reserved.
//

#ifndef RingBufferAny_h
#define RingBufferAny_h

#include <atomic>
#include <memory>
#include <typeindex>
#include <type_traits>

#include "RingBufferV.hpp"
#include "Spinlock.hpp"

// WARNING: stdlib objects that use dynamic allocation ( string, list, vector etc are not supported atm )
// If you use your own structures, make sure to use primitive types and stack allocated arrays
// Spinlocks in this are only to protect against wrongful use and when resizing the buffer
// Note that resizing the buffer to a smaller one than before might cause a complete reset of the heads

class RingBufferAny : protected RingBufferV< int8_t > {
public:
    struct VariableHeader {
        std::type_index type_index = typeid( nullptr );
        size_t sizeOfInput = 0;
        size_t valuesPassed = 0;
    };
    
    RingBufferAny ( void ) : RingBufferV< int8_t > (  ) {}
    RingBufferAny ( size_t sizeOfBuffer ) : RingBufferV< int8_t > (  )
    {
        setup( sizeOfBuffer );
    }
    ~RingBufferAny ( void ) {}
    
    void setup ( size_t sizeOfBuffer )
    {
        resize( sizeOfBuffer );
    }
   
    void resize ( size_t size )
    {
        inputSpin.lock(  );
        outputSpin.lock(  );
        
        this->sizeOfBuffer.store( size );
        this->RingBufferV< int8_t >::setSize( size );
        
        inputSpin.unlock(  );
        outputSpin.unlock(  );
    }
    
    size_t size ( void ) const
    {
        return sizeOfBuffer.load(  );
    }
    
    template <class T>
    void adjustSizeIfNeeded( size_t numElements )
    {
        size_t totalSize = numElements * sizeof( T );
        if ( totalSize > sizeOfBuffer.load(  ) )
        {
            resize( totalSize );
        }
    }
    
    void reset ( void )
    {
        this->RingBufferV< int8_t >::reset(  );
    }
    
    // TODO add vector/list serialization support
    
    bool anyAvailableForPop ( VariableHeader& outputHeader )
    { // Return if an element is available to be popped out of the queue, as well as the header information if available
        outputSpin.lock(  ); // Lock
        
        size_t avForRead = items_available_for_read(  );
        if ( avForRead > headerSize )
        {
            this->RingBufferV< int8_t >::getWithoutPop( reinterpret_cast< int8_t* > ( &outputHeader ), headerSize ); // Unlock case 1
            
            outputSpin.unlock(  );
            return ( avForRead >= ( headerSize + outputHeader.sizeOfInput ) );
        }
        
        outputSpin.unlock(  ); // Unlock case 2
        return false;
    }
    
    template <class T>
    bool push (T* inputData, size_t numValues)
    { // Push specified number of values starting at pointer inputData on to the queue
        VariableHeader vh;
        vh.type_index = std::type_index( typeid( T ) );
        vh.valuesPassed = numValues;
        vh.sizeOfInput = sizeof( *inputData ) * numValues;
        inputSpin.lock(  ); // Lock
        
        if ( ( headerSize + vh.sizeOfInput ) <= items_available_for_write(  ) )
        {
            // Write header
            this->RingBufferV< int8_t >::push( reinterpret_cast< int8_t* > ( &vh ), headerSize );
            // Write object(s)
            this->RingBufferV< int8_t >::push( reinterpret_cast< int8_t* > ( inputData ), vh.sizeOfInput );
            
            inputSpin.unlock(  ); // Unlock case 1
            return true;
        }
        
        inputSpin.unlock(  ); // Unlock case 2
        return false;
    }
    
    template <class T>
    void pop ( T* output, size_t outputSize )
    { // Pop single or multiple values from queue
        VariableHeader vh;
        outputSpin.lock(  ); // Lock
        
        size_t avForRead = items_available_for_read(  );
        
        // Get Header
        if ( avForRead < headerSize )
            throw std::runtime_error( "Not enough data in buffer to pop header, use anyAvailableForPop to check this!" );
        else
            this->RingBufferV< int8_t >::pop( reinterpret_cast< int8_t* > ( &vh ), headerSize );
        
        if ( std::type_index( typeid( T ) ) != vh.type_index )
            throw std::runtime_error( "Type matching error!");
        if ( vh.valuesPassed > outputSize ) // Possibility: add seperate unloading (with moveable header?)
            throw std::runtime_error( "Number of values passed on push will not fit in pop structure, use the correct return structure." );
        
        // Get Object(s)
        if ( avForRead < vh.sizeOfInput )
            throw std::runtime_error( "Not enough data in buffer to pop object(s), use anyAvailableForPop to check this!" );
        else
            this->RingBufferV<int8_t>::pop( reinterpret_cast< int8_t* > ( output ), vh.sizeOfInput );
        
        outputSpin.unlock(  ); // Unlock
    }
    
protected:
    std::atomic< size_t > sizeOfBuffer;
    static const size_t headerSize = sizeof( VariableHeader );
    
private:
    Spinlock inputSpin; // Placeholder type
    Spinlock outputSpin;// ''
};

#endif /* RingBufferAny_h */
