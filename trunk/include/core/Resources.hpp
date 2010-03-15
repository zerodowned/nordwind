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
	class Resources : public QObject {
		Q_OBJECT;
		public:
			class Land : public GID {
				public:
					Land( const GID& _gid );
					QPixmap getArt() const;
					Land& setArt( QPixmap _art );
					const LandInfo& getInfo() const;
					Land& setInfo( const LandInfo& _info );
					QPixmap getTexture() const;
					Land& setTexture( QPixmap _texture );
					QPainterPath getShape() const;
					operator QString() const;
				private:
					LandInfo mInfo;
					QPainterPath mShape;
					QPixmap mArt;
					QPixmap mTexture;
					QHash<QRect,QPair<QPainterPath,QPixmap> > mStrechedTiles;
			};

			class Tile : public GID {
				public:
					Tile( const GID& _gid );
					QPixmap getArt() const;
					Tile& setArt( QPixmap _art );
					const TileInfo& getInfo() const;
					Tile& setInfo( const TileInfo& _info );
					QPainterPath getShape() const;
					operator QString() const;
				private:
					TileInfo mInfo;
					QPainterPath mShape;
					QPixmap mArt;
			};

			class Animation : public GID {
				public:
					Animation( const GID& _gid );
					Sequence getSequence(Action _action, Direction _direction);
					bool isAvailable(Action _action, Direction _direction);
					Body getBody() const;
				private:
					Actions mActions;
			};
			Resources(QObject* _parent);

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

	typedef QSharedPointer<Resources::Land> Land;
	typedef QSharedPointer<Resources::Tile> Tile;
	typedef QSharedPointer<Resources::Animation> Animation;

	// BEGIN OF INLINE IMPLEMENTATIONS

	inline Resources::Land::Land( const GID& _gid )
	: GID(_gid) {
	}

	inline QPixmap Resources::Land::getArt() const {
		return mArt;
	}

	inline Resources::Land& Resources::Land::setArt(QPixmap _art) {
		mArt = _art;
		return *this;
	}

	inline const LandInfo& Resources::Land::getInfo() const {
		return mInfo;
	}

	inline Resources::Land& Resources::Land::setInfo(const LandInfo& _info) {
		mInfo = _info;
		return *this;
	}

	inline QPixmap Resources::Land::getTexture() const {
		return mTexture;
	}

	inline Resources::Land& Resources::Land::setTexture(QPixmap _texture) {
		mTexture = _texture;
		return *this;
	}

	inline Resources::Land::operator QString() const {
		return QString("GID:%1\n%2").arg(GID::operator QString()).arg(mInfo);
	}

	inline Resources::Tile::Tile( const GID& _gid )
	: GID(_gid) {
	}

	inline QPixmap Resources::Tile::getArt() const {
		return mArt;
	}

	inline Resources::Tile& Resources::Tile::setArt(QPixmap _art) {
		mArt = _art;
		return *this;
	}

	inline const TileInfo& Resources::Tile::getInfo() const {
		return mInfo;
	}

	inline Resources::Tile& Resources::Tile::setInfo(const TileInfo& _info) {
		mInfo = _info;
		return *this;
	}

	inline Resources::Animation::Animation( const GID& _gid )
	: GID(_gid) {
	}

	inline bool Resources::Animation::isAvailable(Action _action, Direction _direction) {
		return (mActions.contains(_action)&&mActions[_action].contains(_direction)) ? true : false;
	}

	inline Body Resources::Animation::getBody() const {
		return getID();
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
