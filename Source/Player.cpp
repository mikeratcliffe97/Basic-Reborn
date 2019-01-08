//
// Created by m2-ratcliffe on 09/11/2018.
//

#include "Player.h"
#include <string>

Player::Player(){


}


Player::~Player()
{
    for (int i = 0; i < ITEM_NUM; ++i)
    {
        delete inventory[i];
        inventory[i] = nullptr;
    }

}

std::string Player::getPlayerInput()
{
    return player_input;
}

int Player::getPlayerIndex()
{
    return index_num;
}

void Player::setIndex_num(int index_num) {

    Player::index_num = index_num;
}

void Player::setInput(const std::string &player_input) {
    Player::player_input = player_input;
}

void Player::subtractPlayerInput(int val)
{
    player_input.resize(player_input.size() - val);
}

void Player::addItem(Item *item)
{
    inventory[item_count] = item;
    ++item_count;
}

Item *Player::getItem(int i)
{
    return inventory[i];
}

bool Player::checkItem(std::string item_name)
{
    for (int i = 0; i < ITEM_NUM; ++i)
    {
        if(inventory[i] && inventory[i]->getItmName() == item_name)
        {
            return true;
        }
    }
    return false;
}


