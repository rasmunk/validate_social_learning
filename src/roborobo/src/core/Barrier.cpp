//
// Created by 40161642 on 30/06/2016.
//
#include "core/Utilities/Barrier.h"
#include <iostream>


void Barrier::wait() {
    int syncStep = _syncStep.load();
    _threads.fetch_sub(1);
    if (_threads.load() == 0) {
        _threads.store(_size);
        _syncStep.fetch_add(1);
    } else {
        while (_syncStep.load() == syncStep);
    }
}
