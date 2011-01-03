/*
 * Dynamic.hpp
 *
 *  Created on: Jan 2, 2011
 *      Author: idstein
 */

#ifndef DYNAMIC_HPP_
#define DYNAMIC_HPP_

/*
 *
 */
#include <QObject>
#include "../Typedefs.hpp"

namespace game {

class Dynamic: public QObject {
        Q_OBJECT;
public:
        Dynamic(const Serial& serial, QObject* parent = NULL);
        Serial serial() const; // only getter, it has to be unique!
protected:
        Serial mSerial;
};

}

#endif /* DYNAMIC_HPP_ */
