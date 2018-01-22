/*
 * RingBufferV.hpp
 */

#ifndef RingBufferV_HPP
#define RingBufferV_HPP

#include <iostream>
#include <atomic>
#include <string>
#include <unistd.h>
#include <string.h> // memcpy
#include <cstring>
#include <vector>

// Vector implementation of RingBuffer for better resizing + allowing later implementation of own non-blocking freestore allocator without having to resort to using boost lib

template <class T>
class RingBufferV
{
public:
    RingBufferV ( void );
    RingBufferV ( size_t size );
    ~RingBufferV ( void );
    void setup ( size_t size );
    
    size_t push ( const T* data, size_t n );
    size_t pop ( T* data, size_t n );
    size_t getWithoutPop ( T* data, size_t n );
    size_t items_available_for_write ( void ) const;
    size_t items_available_for_read ( void ) const;
    bool isLockFree ( void ) const;
    void pushMayBlock ( bool block );
    void popMayBlock ( bool block );
    void setBlockingNap ( size_t blockingNap );
    void setSize ( size_t size );
    void reset ( void );
    
private:
    size_t array_size;
    std::vector< T > buffer;
    static constexpr long type_size = sizeof( T ); // Not used atm

    std::atomic< size_t > tail_index = { 0 }; // write pointer
    std::atomic< size_t > head_index = { 0 }; // read pointer

    bool blockingPush;
    bool blockingPop;
    useconds_t blockingNap = 500; // 0.5 ms
}; // RingBufferV{}

template < class T >
RingBufferV< T >::RingBufferV ( void )
{
    blockingPush = false;
    blockingPop = false;
} // RingBufferV( void )

template < class T >
RingBufferV< T >::RingBufferV ( size_t array_size )
{
    blockingPush = false;
    blockingPop = false;
    
    setup( array_size );
} // RingBufferV( void )

template < class T >
RingBufferV< T >::~RingBufferV ( void )
{
    
} // ~RingBufferV( void )

template < class T >
void RingBufferV< T >::setup ( size_t array_size )
{
    setSize( array_size ); // allocate storage
}

template < class T >
size_t RingBufferV< T >::items_available_for_write ( void ) const
{
    long pointerspace = head_index.load(  ) - tail_index.load(  ); // signed
    
    if ( pointerspace > 0 )
        return pointerspace; // NB: > 0 so NOT including 0
    else
        return pointerspace + array_size;
} // items_available_for_write( void )

template < class T >
size_t RingBufferV< T >::items_available_for_read ( void ) const
{
    long pointerspace = tail_index.load(  ) - head_index.load(  ); // signed
    
    if ( pointerspace >= 0 )
        return pointerspace; // NB: >= 0 so including 0
    else
        return pointerspace + array_size;
} // items_available_for_read( void )

template < class T >
void RingBufferV< T >::pushMayBlock ( bool block )
{
    this->blockingPush = block;
} // pushMayBlock( void )

template < class T >
void RingBufferV< T >::popMayBlock ( bool block )
{
    this->blockingPop = block;
} // popMayBlock( void )

template < class T >
void RingBufferV< T >::setBlockingNap ( size_t blockingNap )
{
    this->blockingNap = blockingNap;
} // setBlockingNap( void )


/*
 * Try to write as many items as possible and return the number actually written
 */
template < class T >
size_t RingBufferV< T >::push ( const T* data, size_t n )
{
    size_t space = array_size, n_to_write, first_chunk, second_chunk, current_tail;
    
    //space = items_available_for_write();
    if ( blockingPush )
    {
        while ( ( space = items_available_for_write(  ) ) < n )
        { // blocking
            usleep( blockingNap );
        } // while, Any better solutions to this?
    } // if
    
    n_to_write = ( n <= space ) ? n : space; // limit
    
    current_tail = tail_index.load(  );
    if ( current_tail + n_to_write <= array_size )
    { // chunk fits without wrapping
        std::copy_n( data, n_to_write, buffer.begin(  ) + current_tail );
        tail_index.store( current_tail + n_to_write );
        
    } else { // chunk has to wrap
        first_chunk = array_size - current_tail;
        std::copy_n( data, first_chunk, buffer.begin(  ) + current_tail );
        second_chunk = n_to_write - first_chunk;
        std::copy_n( data + first_chunk, second_chunk, buffer.begin(  ) );
        tail_index.store( second_chunk );
    }
    
    return n_to_write;
} // push( const T* data, size_t n )


/*
 * Try to read as many items as possible and return the number actually read
 */
template < class T >
size_t RingBufferV< T >::pop ( T* data, size_t n )
{
    size_t space = array_size, n_to_read, first_chunk, second_chunk, current_head;
    
    //space = items_available_for_read(); // if checking outside of thread, not necessary?
    if ( blockingPop )
    {
        while ( ( space = items_available_for_read(  ) ) < n )
        { // blocking
            usleep( blockingNap );
        } // while
    } // if
    
    if ( space == 0 )
        return 0;
    
    n_to_read = ( n <= space ) ? n : space; // limit
    
    current_head = head_index.load(  );
    if ( current_head + n_to_read <= array_size )
    { // no wrapping necessary
        std::copy_n( buffer.begin(  ) + current_head, n_to_read, data );
        head_index.store( current_head + n_to_read );
        
    } else { // read has to wrap
        first_chunk = array_size - current_head;
        std::copy_n( buffer.begin(  ) + current_head, first_chunk, data );
        second_chunk = n_to_read - first_chunk;
        std::copy_n( buffer.begin(  ), second_chunk, data + first_chunk );
        head_index.store( second_chunk );

    }
    return n_to_read;
} // pop( T* data, size_t n )

/*
 * Try to read as many items as possible and return the number actually read
 */
template < class T >
size_t RingBufferV< T >::getWithoutPop ( T* data, size_t n )
{
    size_t space = array_size, n_to_read, first_chunk, second_chunk, current_head;
    
    //space = items_available_for_read(); //if checking outside of thread, not necessary?
    if ( blockingPop )
    {
        while ( ( space = items_available_for_read(  ) ) < n )
        { // blocking
            usleep( blockingNap );
        } // while
    } // if
    
    if ( space == 0 )
        return 0;
    
    n_to_read = ( n <= space ) ? n : space; // limit
    
    current_head = head_index.load(  );
    if ( current_head + n_to_read <= array_size )
    { // no wrapping necessary
        std::copy_n( buffer.begin(  ) + current_head, n_to_read, data );
        //head_index.store(current_head + n_to_read);
        
    } else { // read has to wrap
        first_chunk = array_size - current_head;
        std::copy_n( buffer.begin(  ) + current_head, first_chunk, data );
        second_chunk = n_to_read - first_chunk;
        std::copy_n( buffer.begin(  ), second_chunk, data + first_chunk );
        //head_index.store(second_chunk);
        
    }
    return n_to_read;
} // getWithoutPop( T* data, size_t n )

template <class T>
bool RingBufferV<T>::isLockFree ( void ) const
{
    return ( tail_index.is_lock_free(  ) && head_index.is_lock_free(  ) );
} // isLockFree( void )


template <class T>
void RingBufferV< T >::setSize( size_t size )
{
    this->array_size = size;
    buffer.resize( array_size ); // allocate storage
    if ( tail_index.load(  ) <= array_size || head_index.load(  ) <= array_size )
    {
        reset(  );
    }
}

template <class T>
void RingBufferV<T>::reset( void )
{
    tail_index.store( 0 );
    head_index.store( 0 );
} // isLockFree( void )

#endif
