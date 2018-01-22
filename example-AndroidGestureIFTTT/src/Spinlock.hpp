//
//  Spinlock.h
//  beladrum
//
//  Created by James on 11/05/17.
//  Copyright © 2017 HKU. All rights reserved.
//

#ifndef Spinlock_h
#define Spinlock_h

#include <atomic>

class Spinlock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock ( void ) {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock ( void ) {
        locked.clear(std::memory_order_release);
    }
};

#endif /* Spinlock_h */
