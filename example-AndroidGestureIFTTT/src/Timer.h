//
//  Timer.h
//  Bitalino
//
//  Created by James on 28/11/2017.
//
//

#ifndef Timer_h
#define Timer_h

#include <stdio.h>
#include <chrono>

class Timer {
public:
    uint64_t getTimeElapsed ( void )
    { // Since last call of reset
        return std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::high_resolution_clock::now() - start ).count(  );

    }
    void reset ( void )
    {
        start = std::chrono::high_resolution_clock::now(  );
    }
private:
    std::chrono::high_resolution_clock::time_point start;
};

#endif /* Timer_h */
