//
// Created by 40161642 on 03/06/2016.
//

#ifndef ROBOROBO_MAP_H
#define ROBOROBO_MAP_H

#include <vector>
#include <map>

template<class T, class U>
class Map {
private:
    std::map<T, U> _map;

public:
    Map() {};
    Map(const Map<T,U>& map) { _map = map._map; };
    Map(const Map<T,U>* map) { _map = map->_map; };
    virtual ~Map() {};

    double getValueAt(T key);
    long long int getSize();
    virtual void set(T key, U value);
    virtual void flush();
};

template<class T, class U>
double Map<T,U>::getValueAt(T key)
{
    return this->_map[key];
}

template<class T, class U>
long long int Map<T,U>::getSize()
{
    return this->_map.size();
}

template<class T, class U>
void Map<T,U>::set(T key, U value)
{
    this->_map[key] = value;
}

template<class T, class U>
void Map<T,U>::flush()
{
    this->_map.clear();
}

#endif //ROBOROBO_MAP_H
