/*
 * TileInfoLand.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef TILEDATA_HPP_
#define TILEDATA_HPP_

#include "../util/Typedefs.hpp"

namespace resource {

	class TileData; //forward

	class TileData : public QObject {
	public:
		enum Flag {
			Background 		= 1 << 0,
			Weapon 	   		= 1 << 1,
			Transparent 	= 1 << 2,
			Translucent		= 1 << 3,
			Wall			= 1 << 4,
			Damaging		= 1 << 5,
			Impassable		= 1 << 6,
			Wet				= 1 << 7,
			Unknown			= 1 << 8,
			Surface			= 1 << 9,
			Bridge			= 1 << 10,
			Generic 		= 1 << 11,
			Stackable 		= Generic,
			Window 			= 1 << 12,
			NoShoot			= 1 << 13,
			PrefixA			= 1 << 14,
			PrefixAn		= 1 << 15,
			Internal 		= 1 << 16, // (things like hair, beards, etc)
			Foliage			= 1 << 17,
			PartialHue		= 1 << 18,
			Unknown1		= 1 << 19,
			Map				= 1 << 20,
			Container		= 1 << 21,
			Wearable		= 1 << 22,
			LightSource		= 1 << 23,
			Animated		= 1 << 24,
			NoDiagonal		= 1 << 25,
			Unknown2		= 1 << 26,
			Armor			= 1 << 27,
			Roof			= 1 << 28,
			Door			= 1 << 29,
			StairBack		= 1 << 30,
			StairRight		= 1 << 31
		};
		Q_DECLARE_FLAGS(Flags, Flag);
		class Entry {
			friend class TileData;
			public:
				Entry();
				virtual ~Entry();
				QString getName() const;
				Flags getFlags() const;
				virtual bool isTile() const;
				virtual bool isValid() const;
			private:
				Flags mFlags;
				QString mName;
		};
		class LandInfo: public Entry {
			friend class TileData;
			public:
				quint16 getTextureID() const;
				virtual bool isTile() const;
				virtual bool isValid() const;
			private:
				quint16 mTextureID;
		};
		class TileInfo: public Entry {
			friend class TileData;
			public:
				quint8 getHeight() const;
				quint8 getHue() const;
				quint8 getQuality() const;
				quint8 getQuantity() const;
				quint16 getHitpoints() const;
				quint16 getAnimation() const;
				quint8 getUnknown1() const;
				quint8 getUnknown2() const;
				quint8 getStackOffset() const;
				quint8 getUnknown3() const;
				quint8 getWeight() const;
				virtual bool isTile() const;
				virtual bool isValid() const;
			private:
				quint8 mWeight; // 255 if not moveable
				quint8 mQuality; // See below
				quint16 mHitpoints;
				quint8 mUnknown1;
				quint8 mQuantity; // See below
				quint16 mAnimation; // The BodyID of the item's animation
				quint8 mUnknown2;
				quint8 mHue; // Unknown use, perhaps colored lighting?
				quint8 mStackOffset; // if flag Generic the pixel offset of the second item if the item is stackable
				quint8 mUnknown3; // Known in some circles as "Value"
				quint8 mHeight;
		};
		typedef QVector< QSharedPointer<Entry> > Entries;
		TileData(QString _fileName, QObject* _parent);
		virtual ~TileData();
		QSharedPointer<Entry> getEntry(ID _id) const;
		Entries getEntries() const;
	private:
		Entries mEntries;
	};

	typedef QSharedPointer<TileData::LandInfo> LandInfo;
	typedef QSharedPointer<TileData::TileInfo> TileInfo;

	inline TileData::Entry::Entry() {
	}

	inline TileData::Entry::~Entry() {
	}

	inline QString TileData::Entry::getName() const {
		return mName;
	}

	inline TileData::Flags TileData::Entry::getFlags() const {
		return mFlags;
	}

	inline bool TileData::Entry::isTile() const {
		return false;
	}

	inline bool TileData::Entry::isValid() const {
		return false;
	}

	inline bool TileData::LandInfo::isTile() const {
		return false;
	}

	inline bool TileData::LandInfo::isValid() const {
		return true;
	}

	inline quint16 TileData::LandInfo::getTextureID() const {
		return mTextureID;
	}

	inline quint16 TileData::TileInfo::getAnimation() const {
		return mAnimation;
	}

	inline quint8 TileData::TileInfo::getHeight() const {
		return mHeight;
	}

	inline quint8 TileData::TileInfo::getHue() const {
		return mHue;
	}

	inline quint8 TileData::TileInfo::getQuality() const {
		return mQuality;
	}

	inline quint8 TileData::TileInfo::getQuantity() const {
		return mQuantity;
	}

	inline quint16 TileData::TileInfo::getHitpoints() const {
		return mHitpoints;
	}

	inline quint8 TileData::TileInfo::getUnknown1() const {
		return mUnknown1;
	}

	inline quint8 TileData::TileInfo::getUnknown2() const {
		return mUnknown2;
	}

	inline quint8 TileData::TileInfo::getStackOffset() const {
		return mStackOffset;
	}

	inline quint8 TileData::TileInfo::getUnknown3() const {
		return mUnknown3;
	}

	inline quint8 TileData::TileInfo::getWeight() const {
		return mWeight;
	}

	inline bool TileData::TileInfo::isTile() const {
		return true;
	}

	inline bool TileData::TileInfo::isValid() const {
		return true;
	}

	inline QSharedPointer<TileData::Entry> TileData::getEntry( ID _id ) const {
		return mEntries.value( _id );
	}

	inline TileData::Entries TileData::getEntries() const {
		return mEntries;
	}
}

Q_DECLARE_OPERATORS_FOR_FLAGS(resource::TileData::Flags);

#endif /* TILEINFOLAND_HPP_ */
