//
// Created by m2-ratcliffe on 09/11/2018.
//
#include <string>
#ifndef BASICREB0RN_PLAYER_H
#define BASICREB0RN_PLAYER_H

#include "Item.h"

class Player {
public:

    Player();

    ~Player();

    int getPlayerIndex();
    std::string getPlayerInput();
    void subtractPlayerInput(int val);
    void setInput(const std::string &player_input);
    void setIndex_num(int index_num);
    void addItem(Item* item);
    Item* getItem(int i);
    bool checkItem(std::string item_name);

private:
    Item* inventory[ITEM_NUM] = {nullptr};
    int item_count = 0;

    std::string player_input = "";
    int index_num = 0;




};
#endif //BASICREB0RN_PLAYER_H
