#include "Item.hpp"

using namespace game;


Item::Item(const Serial &serial, QObject *parent, const ID &id, const ID &hue)
    : Dynamic(serial,parent),
    Static(id,hue) {
}
