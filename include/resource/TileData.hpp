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

	class TileData : public QObject {
		Q_OBJECT;
		Q_FLAGS(Flag Flags);
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
			class LandInfo {
				public:
					quint16 mTextureID;
					Flags mFlags;
					QString mName;
					operator QString() const;
			};
			class TileInfo {
				public:
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
					Flags mFlags;
					QString mName;
					operator QString() const;
			};
			TileData(QString _fileName, QObject* _parent);
			virtual ~TileData();
			LandInfo getLandInfo(ID _id) const;
			TileInfo getTileInfo(ID _id) const;
		private:
			QVector<LandInfo> mLandInfos;
			QVector<TileInfo> mTileInfos;
	};

	typedef TileData::LandInfo LandInfo;
	typedef TileData::TileInfo TileInfo;

	inline LandInfo TileData::getLandInfo( ID _id ) const {
		return mLandInfos.value( _id );
	}

	inline TileInfo TileData::getTileInfo( ID _id ) const {
		return mTileInfos.value( _id );
	}
}

Q_DECLARE_OPERATORS_FOR_FLAGS(resource::TileData::Flags);

#endif /* TILEINFOLAND_HPP_ */
