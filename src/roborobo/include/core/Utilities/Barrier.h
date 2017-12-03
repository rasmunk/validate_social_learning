//
// Created by 40161642 on 30/06/2016.
//

#ifndef ROBOROBO_BARRIER_H
#define ROBOROBO_BARRIER_H
#include <atomic>

class Barrier {
private:
    const int _size;
    std::atomic<int> _threads;
    std::atomic<int> _syncStep;

public:
    Barrier(int threads) : _threads(threads), _size(threads), _syncStep(0) {}
    void wait();
};


#endif //ROBOROBO_BARRIER_H
