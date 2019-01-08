//
// Created by m2-ratcliffe on 10/11/2018.
//

#include "Item.h"

Item::Item(std::string item_name)
{
    itm_name = item_name;


}

std::string Item::getItmName()
{
    return itm_name;
}

Item::~Item() {

}

//bool Item::isItem_owned() {
//
//    return item_owned;
//}
//
//void Item::setItem_owned(bool item_owned)
//{
//    Item::item_owned = item_owned;
//}
