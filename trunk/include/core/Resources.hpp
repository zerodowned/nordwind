#ifndef RESOURCES_HPP__
#define RESOURCES_HPP__

#include <qcache.h>
#include <qobject.h>
#include <qsettings.h>
#include "../resource/Animations.hpp"
#include "../resource/Arts.hpp"
#include "../resource/Facets.hpp"
#include "../resource/Gumps.hpp"
#include "../resource/TileData.hpp"
#include "../resource/Textures.hpp"
#include "../resource/GID.hpp"

namespace resource {

	// DECLARATIONS

	class Land {
		public:
			Land( const GID& _gid );
			GID mGID;
			QPixmap mArt;
			LandInfo mInfo;
			QPixmap mTexture;
	};

	class Tile {
		public:
			Tile( const GID& _gid );
			GID mGID;
			QPixmap mArt;
			TileInfo mInfo;
	};

	class Animation {
		public:
			Animation( const GID& _gid );
			Animation& addSequence( Action _action, Direction _direction, const Sequence& _sequence);
			Sequence getSequence(Action _action, Direction _direction);
			bool isAvailable(Action _action, Direction _direction);
			GID mGID;
		private:
			Actions mActions;
	};

	class Resources : public QObject {
		Q_OBJECT;
		public:
			Resources(QSettings& _settings, QObject* _parent);

			QSharedPointer<Land> getLand( ID _id, ID _hueID = 0, bool _partialHue = false );
			QSharedPointer<Tile> getTile( ID _id, ID _hueID = 0, bool _partialHue = false );
			QSharedPointer<Animation> getAnimation( Body _body, ID _hueID = 0, bool _partialHue = false );

			QSharedPointer<Animations> animations();
			QSharedPointer<Arts> arts();
			QSharedPointer<Gumps> gumps();
			QSharedPointer<Hues> hues();
			QSharedPointer<Facets> facets();
			QSharedPointer<Textures> textures();
			QSharedPointer<TileData> tileData();

			void moveToCache( Land* _land );
			void moveToCache( Tile* _tile );
			void moveToCache( Animation* _animation );
		protected:
			QSharedPointer<Animations> mAnimationsLoader;
			QSharedPointer<Arts> mArtsLoader;
			QSharedPointer<Gumps> mGumpsLoader;
			QSharedPointer<Hues> mHues;
			QSharedPointer<Facets> mFacetsLoader;
			QSharedPointer<Textures> mTexturesLoader;
			QSharedPointer<TileData> mTileData;
		private:
			QHash<GID, QWeakPointer<Land> > mLands;
			QCache<GID, Land > mLandsCache;
			QHash<GID, QWeakPointer<Tile> > mTiles;
			QCache<GID, Tile > mTilesCache;
			QHash<GID, QWeakPointer<Animation> > mAnimations;
			QCache<GID, Animation > mAnimationsCache;
	};


	// BEGIN OF INLINE IMPLEMENTATIONS

	inline Land::Land( const GID& _gid )
	: mGID(_gid) {
	}

	inline Tile::Tile( const GID& _gid )
	: mGID(_gid) {
	}

	inline Animation::Animation( const GID& _gid )
	: mGID(_gid) {
	}

	inline Animation& Animation::addSequence( Action _action, Direction _direction, const Sequence& _sequence ) {
		mActions[_action][_direction] = _sequence;
		return *this;
	}

	inline bool Animation::isAvailable(Action _action, Direction _direction) {
		return (mActions.contains(_action)&&mActions[_action].contains(_direction)) ? true : false;
	}

	inline QSharedPointer<Animations> Resources::animations() {
		return mAnimationsLoader;
	}

	inline QSharedPointer<Arts> Resources::arts() {
		return mArtsLoader;
	}

	inline QSharedPointer<Gumps> Resources::gumps() {
		return mGumpsLoader;
	}

	inline QSharedPointer<Hues> Resources::hues() {
		return mHues;
	}

	inline QSharedPointer<Facets> Resources::facets() {
		return mFacetsLoader;
	}

	inline QSharedPointer<Textures> Resources::textures() {
		return mTexturesLoader;
	}

	inline QSharedPointer<TileData> Resources::tileData() {
		return mTileData;
	}
}

#endif
