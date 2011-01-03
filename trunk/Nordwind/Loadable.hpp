/*
 * Loadable.hpp
 *
 *  Created on: Nov 1, 2010
 *      Author: idstein
 */

#ifndef LOADABLE_HPP_
#define LOADABLE_HPP_

#include <QtPlugin>

class Loadable {
public:
	virtual ~Loadable() {};
};

Q_DECLARE_INTERFACE(Loadable, "de.siebenwind.Nordwind.Loadable/0.1");


#endif /* LOADABLE_HPP_ */
