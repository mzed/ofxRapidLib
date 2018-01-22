//
//  IFTTTThreaded.hpp
//  Non-(b)locking Threaded Webhook event caller
//
//  Created by James on 10/01/2018.
//  Copyright © 2018 James. All rights reserved.
//

#ifndef IFTTTThreaded_hpp
#define IFTTTThreaded_hpp

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <string>
#include <sstream>
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#include "RingBufferAny.hpp"

#ifdef __ANDROID__
#include <ostream>
namespace std
{ // Workaround for android
   template <typename T>
   std::string to_string(T Value)
   {
       std::ostringstream TempStream;
       TempStream << Value;
       return TempStream.str();
   }

   inline long double strtold(const char * str, char ** str_end)
   {
       return strtod(str, str_end);
   }
}
#endif

class IFTTTThreaded
{
public:
    // Gets rid of thread
    ~IFTTTThreaded ( void )
    { // Stop thread
        if ( running.load(  ) )
        {
            running.store( false );
            processMainThread.join(  );
        }
    }
    
    // Spawn thread ( returns true on success )
    bool setup ( size_t eventQueueSize = 4096, useconds_t threadSleepTime = 500000 )
    {
        this->eventQueue.setup( eventQueueSize ); // Some space for messages
        
        if ( !running.load(  ) )
        {
            this->threadSleepTime = threadSleepTime;
            
            // Spawn process thread
            running.store( true );
            processMainThread = std::thread( &IFTTTThreaded::mainLoop, this );
            return true;
        }
        return false;
    }
    
    // Add event to the send queue
    void triggerEvent ( const char* eventName, const char* userKey, std::string val1 = "",
                       std::string val2 = "", std::string val3 = "" )
    {
        std::ostringstream postBuffer;
        std::ostringstream jsonBuffer;
        jsonBuffer << "{ \"value1\" : \"" << val1;
        jsonBuffer << "\", \"value2\" : \"" << val2;
        jsonBuffer << "\", \"value3\" : \"" << val3;
        jsonBuffer << "\" }";
        std::string jsonString = jsonBuffer.str(  );
        
        // Header
        postBuffer << "POST /trigger/" << eventName << "/with/key/" << userKey << " HTTP/1.1\r\n";
        postBuffer << "Content-Type: application/json\r\n";
        postBuffer << "Host: " << host << "\r\n";
        postBuffer << "Connection: close \r\n"; // keep-alive didn't seem to work well
        postBuffer << "Content-Length: " << std::to_string( jsonString.length(  ) ) << "\r\n\r\n";
        
        // Content
        postBuffer << jsonString;
        
        // Message as string
        std::string postBufferString = postBuffer.str(  );
        
        // Add to queue
        while ( !eventQueue.push( const_cast< char * >( postBufferString.c_str(  ) ), postBufferString.length(  ) ) )
        {
            // Double the size of the sendBuffer if message doesn't fit ( not clean and the allocator does block! )
            eventQueue.adjustSizeIfNeeded< char >( postBufferString.length(  ) * 2 );
            printf( "Warning, having to resize the eventQueue for message to fit, please allocate more space in setup\n" );
        }
    }
    
protected:
    
    bool establishConnection ( void )
    {
        // create the socket
        sockfd = socket( AF_INET, SOCK_STREAM, 0 );
        if ( sockfd < 0 )
            return false;
        // lookup the ip address
        server = gethostbyname( host );
        if ( server == NULL )
            return false;
        
        // fill in the structure
        memset( &serv_addr, 0, sizeof( serv_addr ) );
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons( portNo );
        memcpy( &serv_addr.sin_addr.s_addr, server->h_addr, server->h_length );
        
        // connect the socket
        if ( connect( sockfd, reinterpret_cast< struct sockaddr* >( &serv_addr ), sizeof( serv_addr ) ) < 0 )
            return false;
        
        return true;
    }
    
    bool sendMessage ( const char* message, size_t messageLength )
    {
        const uint16_t maxResponseSize = 4096;
        size_t sent, received;
        long bytes;
        char response[ maxResponseSize ];
        
        // send the request
        sent = 0;
        do {
            bytes = send( sockfd, message + sent, messageLength - sent, NULL );
            if ( bytes < 0 )
                return false;
            if ( bytes == 0 )
                break;
            sent += bytes;
        } while ( sent < messageLength );
        
        // receive the response ( just getting rid of system buffer and making sure there is a response, not checking status )
        // TODO: check status ( 200: OK ? )
        
        messageLength = maxResponseSize - 1;
        memset( response, 0, maxResponseSize );
        received = 0;
        do {
            bytes = read( sockfd, response + received, messageLength - received );
            if (bytes < 0)
                return false;
            if ( bytes == 0 )
                break;
            received += bytes;
        } while ( received < messageLength );
        
        if ( received == messageLength )
            throw std::runtime_error("ERROR storing complete response from socket");
        
        return true;
    }
    
    void mainLoop ( void )
    {
        while ( running.load(  ) )
        {
            RingBufferAny::VariableHeader popHeader;
            while ( eventQueue.anyAvailableForPop( popHeader ) )
            { // Not keeping connection alive because IFTTTThreaded doesn't respond when using that
                // Connect to server
                while ( !establishConnection(  ) ) { usleep( 500000 ); }
                
                // Get message from queue
                char message[ popHeader.valuesPassed ];
                eventQueue.pop( message, popHeader.valuesPassed );
                
                // Send message
                while ( !sendMessage( message, popHeader.valuesPassed ) )
                {   // If message can't be sent close socket and retry establishing connection ( not sure if needs to be closed or reconnected ? )
                    // TODO: test if this actually works
                    close( sockfd );
                    establishConnection(  );
                    usleep( 500000 );
                }
                // Close socket
                close( sockfd );
            }
            // Put the thread to sleep for x time
            usleep( threadSleepTime );
        }
    }
    
private:
    int32_t sockfd;
    struct hostent* server;
    struct sockaddr_in serv_addr;
    
    const uint16_t portNo = 80;
    const char* host = "maker.ifttt.com";
    
    std::thread processMainThread;
    std::atomic< bool > running;
    
    RingBufferAny eventQueue;
    
    
    useconds_t threadSleepTime; // 500ms sleep time ( event calls are slow anyway )
};

#endif /* IFTTTThreaded_hpp */
