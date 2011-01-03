#ifndef ITEM_HPP
#define ITEM_HPP

#include "Static.hpp"
#include "../Dynamic.hpp"

namespace game {
class Item : public Dynamic, public Static
{
    Q_OBJECT;
public:
    Item(const Serial& serial, QObject* parent, const ID& id, const ID& hue);
};
}
#endif // ITEM_HPP
