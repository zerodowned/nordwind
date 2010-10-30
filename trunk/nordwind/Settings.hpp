/*
 * Settings.hpp
 *
 *  Created on: Sep 11, 2010
 *      Author: idstein
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <qsettings.h>
#include "Typedefs.hpp"

class Settings : public QSettings, public Singleton<Settings> {
Q_OBJECT;
friend class Singleton<Settings>;
public:
	Settings();
	virtual ~Settings();
};

#endif /* SETTINGS_HPP_ */
