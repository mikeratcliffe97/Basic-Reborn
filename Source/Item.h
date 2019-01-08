//
// Created by m2-ratcliffe on 10/11/2018.
//

#ifndef BASICREB0RN_ITEMS_H
#define BASICREB0RN_ITEMS_H

#include <string>
#include "constant.h"

class Item
    {
    public:

    Item(std::string item_name);
    ~Item();


    std::string getItmName();

//    bool isItem_owned();
//    void setItem_owned(bool item_owned);

    private:

//    bool item_owned = false;
    std::string itm_name = "";



};


#endif //BASICREB0RN_ITEMS_H
