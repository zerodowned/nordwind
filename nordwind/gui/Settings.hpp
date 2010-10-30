/*
 * Settings.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <qsettings.h>

namespace core {

	class Settings : public QSettings {
		private:
			static Settings* mRoot;
		public:
			static Settings* getRoot();
			static Settings* getByName( QString _name );
			Settings( QString _name, QString _fileName, QObject* _parent = NULL );
			virtual ~Settings();
			bool isRoot() const;
	};

	/** Get root Settings
	 * This function returns the root Settings.
	 * Attention!: It will only return a valid point, if the
	 * root Settings exists.
	 *
	 * @return Pointer to root Settings or NULL.
	 */
	inline Settings* Settings::getRoot() {
		return mRoot;
	}

	/** Get Settings by Name
	 * Returns a Settings Object by name exclusive root!
	 *
	 * @return Pointer to Settings object or NULL.
	 */
	inline Settings* Settings::getByName( QString _name ) {
		return getRoot()->findChild<Settings*>( _name );
	}

	/** Is this object this root
	 *
	 * @return true if this object is root otherwise false.
	 */
	inline bool Settings::isRoot() const {
		return (mRoot==this) ? true : false;
	}
}

#endif /* SETTINGS_HPP_ */
