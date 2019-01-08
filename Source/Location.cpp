//
// Created by m2-ratcliffe on 06/11/2018.
//

#include <string>
#include "Location.h"


Location::Location(std::string location_name, Directions direction[4])
{
    loc_name = location_name;

    for (int i = 0; i < 4; ++i) {

        dir[i] = direction[i];

    }
}

 std::string Location::getLocName()
{
    return loc_name;
}



Location::~Location() {

}

Directions Location::getDirection(int i) {
    return dir[i];
}

void Location::setItem(Item *new_item)
{
    item = new_item;
}

Item *Location::getItem()
{
    return item;
}

