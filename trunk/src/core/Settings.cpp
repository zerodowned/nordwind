/*
 * Settings.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#include "core/Settings.hpp"

using namespace core;

Settings* Settings::mRoot = NULL;

Settings::Settings( QString _name, QString _fileName, QObject* _parent )
: QSettings( _fileName, QSettings::IniFormat, _parent ) {
	setObjectName( _name );
	if(mRoot==NULL) {
		mRoot = this;
	} else {
		if(_parent==NULL) {
			setParent(mRoot);
		}
	}
}

Settings::~Settings() {
	if( mRoot == this )
		mRoot = NULL;
}
