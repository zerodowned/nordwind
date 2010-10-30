#ifndef RESOURCES_HPP__
#define RESOURCES_HPP__

#include <qobject.h>
#include "../resource/TileData.hpp"

namespace resource {

// DECLARATIONS
class Resources: public QObject {
Q_OBJECT;
public:
	Resources(QObject* _parent);
	const TileData& tileData() const;

	static QString artFile();
	static QString textureFile();
	static QString animationFile();
protected:
	QScopedPointer<TileData> mTileData;
};

}

#endif
