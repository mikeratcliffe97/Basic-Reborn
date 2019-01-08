//
// Created by m2-ratcliffe on 02/11/2018.
//

#include <Engine/OGLGame.h>

#ifndef BASICREB0RN_CONSTANT_H
#define BASICREB0RN_CONSTANT_H

#endif //BASICREB0RN_CONSTANT_H



const int MAP_SIZE = 32;

const int MAX_INPUT = 10;

const int ITEM_NUM = 32;

const int MAX_SCORE = 8000;
const std::string PASS = "2603";


enum class Verbs : int
    {
    H = 1, //Help
    T = 2, //Take
    U = 3, //Use
    R = 4, //Read
    A = 5, //Activate

    };

enum class Directions : int
{
    N = 1,
    E = 2,
    S = 3,
    W = 4,
    NullDir = 5
};
