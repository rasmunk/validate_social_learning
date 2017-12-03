//
// Created by 40161642 on 31/05/2016.
//

#ifndef ROBOROBO_SHAREDMAP_H
#define ROBOROBO_SHAREDMAP_H

#include "core/Utilities/SharedDataStructure.h"
#include "core/Utilities/Map.h"
#include <mutex>

template<class T, class U>
class SharedMap : public SharedDataStructure, public Map<T,U> {
private:
    std::mutex _mapLock;

public:
    SharedMap() : Map<T,U>() {};
    ~SharedMap() {};
    void set(T key, U value);
    void flush();
};

template<class T, class U>
void SharedMap<T,U>::set(T key, U value)
{
    std::lock_guard<std::mutex> lock(this->_mapLock);
    Map<T,U>::set(key, value);
}

template<class T, class U>
void SharedMap<T,U>::flush()
{
    std::lock_guard<std::mutex> lock(this->_mapLock);
    Map<T,U>::flush();
}

#endif //ROBOROBO_SHAREDMAP_H
