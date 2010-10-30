/*
 * TileInfoLand.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef TILEDATA_HPP_
#define TILEDATA_HPP_

#include "../Typedefs.hpp"
#include <qvector.h>

namespace data {

class Info {
	friend class TileData;
public:
	enum Flag {
		Background = 1 << 0,
		Weapon = 1 << 1,
		Transparent = 1 << 2,
		Translucent = 1 << 3,
		Wall = 1 << 4,
		Damaging = 1 << 5,
		Impassable = 1 << 6,
		Wet = 1 << 7,
		Unknown = 1 << 8,
		Surface = 1 << 9,
		Bridge = 1 << 10,
		Generic = 1 << 11,
		Stackable = Generic,
		Window = 1 << 12,
		NoShoot = 1 << 13,
		PrefixA = 1 << 14,
		PrefixAn = 1 << 15,
		Internal = 1 << 16, // (things like hair, beards, etc)
		Foliage = 1 << 17,
		PartialHue = 1 << 18,
		Unknown1 = 1 << 19,
		Map = 1 << 20,
		Container = 1 << 21,
		Wearable = 1 << 22,
		LightSource = 1 << 23,
		Animated = 1 << 24,
		NoDiagonal = 1 << 25,
		Unknown2 = 1 << 26,
		Armor = 1 << 27,
		Roof = 1 << 28,
		Door = 1 << 29,
		StairBack = 1 << 30,
		StairRight = 1 << 31
	};
	Q_DECLARE_FLAGS(Flags,Flag);
	Info();
	Flags flags() const;
	quint16 textureID() const;
	QString name() const;
	quint8 weight() const;
	quint8 quality() const;
	quint16 hitpoints() const;
	quint8 quantity() const;
	quint16 animation() const;
	quint8 hue() const;
	quint8 stackOffset() const;
	quint8 value() const;
	quint8 height() const;
	bool hasFullInfo() const;
private:
	bool mFullInfo;
	quint16 mGeneric; // TextureID or Quality and Weight (255 = not movable)
	Flags mFlags;
	QString mName;
	quint16 mHitpoints;
	quint8 mUnknown1;
	quint8 mQuantity; // See below
	quint16 mAnimation; // The BodyID of the item's animation
	quint8 mUnknown2;
	quint8 mHue; // Unknown use, perhaps colored lighting?
	quint8 mStackOffset; // if flag Generic the pixel offset of the second item if the item is stackable
	quint8 mValue; // unknown3
	quint8 mHeight;
};

class TileData: public QObject, public QVector<Info>, public Singleton<TileData> {
	Q_OBJECT;
	friend class Singleton<TileData>;
public:
	explicit TileData(QString fileName);
	virtual ~TileData();
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS( data::Info::Flags);

#endif /* TILEINFOLAND_HPP_ */
