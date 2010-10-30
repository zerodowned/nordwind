/*
 * Settings.cpp
 *
 *  Created on: Sep 12, 2010
 *      Author: idstein
 */
#include "Settings.hpp"

Settings::~Settings() {}

Settings::Settings()
: QSettings("config.ini",QSettings::IniFormat,qApp) {
}
