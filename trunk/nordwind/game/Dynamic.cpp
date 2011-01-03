/*
 * Dynamic.cpp
 *
 *  Created on: Jan 2, 2011
 *      Author: idstein
 */

#include "Dynamic.hpp"

using namespace game;

Dynamic::Dynamic(const Serial &serial, QObject *parent)
    : QObject(parent),
    mSerial(serial) {
}
