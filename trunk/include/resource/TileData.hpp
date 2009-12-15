/*
 * TileInfoLand.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef TILEDATA_HPP_
#define TILEDATA_HPP_

#include "..\Typedefs.hpp"

namespace resource {

	namespace game {

		class TileData {
			public:
				struct LandInfo {
					Flags mFlags;
					quint16 mTextureID;
					char mName[20];
				};
				struct TileInfo {
					Flags mFlags; // See below
					quint8 mWeight; // 255 if not moveable
					quint8 mQuality; // See below
					quint16 mUnknown1;
					quint8 mUnknown2;
					quint8 mQuantity; // See below
					quint16 mAnimation; // The BodyID of the item's animation
					quint8 mUnknown3;
					quint8 mHue; // Unknown use, perhaps colored lighting?
					quint8 mUnknown4;
					quint8 mUnknown5; // Known in some circles as "Value"
					quint8 mHeight;
					char mName[20];
				};
				typedef QHash< ID, StaticInfo > StaticInfos;
				typedef QHash< ID, TileInfo > TileInfos;
				TileData( QString _fileName );
				~TileData();
				StaticInfos getStaticInfos() const;
				TileInfos getTileInfos() const;
			private:
				StaticInfos mStaticInfos;
				TileInfos mTileInfos;
		};

	}

}

#endif /* TILEINFOLAND_HPP_ */
