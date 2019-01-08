//
// Created by m2-ratcliffe on 06/11/2018.
//

#ifndef BASICREB0RN_LOCATION_H
#define BASICREB0RN_LOCATION_H
#include <string>

#include "Item.h"

class Location {
public:
    Location(std::string location_name, Directions directions[4]);

    ~Location();

     std::string getLocName();
     Directions getDirection(int i);
    void setItem(Item *new_item);
    Item *getItem();


private:
    std::string loc_name = "";
    Directions dir[4];
    Item* item = nullptr;



};


#endif //BASICREB0RN_LOCATION_H
