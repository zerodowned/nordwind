#include <core/Resources.hpp>
#include <Client.hpp>
#include <QtCore/qdir.h>

using namespace resource;

Sequence Resources::Animation::getSequence( Action _action, Direction _direction ) {
	if(!isAvailable(_action,_direction))
		mActions[_action][_direction] = Client::getInstance()->resources()->animations()->getSequence( getBody(), getHue(),_action,_direction);
	return mActions[_action][_direction];
}

template<typename T>
static void cache( T* _cache ) {
	Client::getInstance()->resources()->moveToCache(_cache);
}

Resources::Resources( QObject* _parent )
: QObject(_parent) {
	QSettings& _settings( Client::getInstance()->settings() );
	_settings.beginGroup("resources");

	QDir path( _settings.value( "path", "." ).toString() );

	QString hue = path.filePath(_settings.value("hue", "hues.mul").toString());
	mHues = QSharedPointer<Hues>(new Hues(hue, this), &Hues::deleteLater);

	QString animIndex = path.filePath(_settings.value("animations/index","anim.idx").toString());
	QString animData = path.filePath(_settings.value("animations/data","anim.mul").toString());
	mAnimationsLoader = QSharedPointer<Animations>(new Animations(animIndex,animData,this), &Animations::deleteLater);

	QString artIndex = path.filePath(_settings.value("arts/index","artidx.mul").toString());
	QString artData = path.filePath(_settings.value("arts/data","art.mul").toString());
	QString artAnim = path.filePath(_settings.value("arts/animdata","animdata.mul").toString());
	mArtsLoader = QSharedPointer<Arts>(new Arts(artAnim,artIndex,artData,this), &Arts::deleteLater);

	mFacetsLoader = QSharedPointer<Facets>(new Facets(this), &Facets::deleteLater);
	for( int i = _settings.beginReadArray("facets")-1; i >= 0; i-- ) {
		_settings.setArrayIndex(i);
		QString name = _settings.value("name").toString();
		QString map = path.filePath(_settings.value("map").toString());
		QSize dimension = _settings.value("size").toSize();
		QString staticsIndex = path.filePath(_settings.value("index").toString());
		QString staticsData = path.filePath(_settings.value("data").toString());
		mFacetsLoader->addFacet(name,map,dimension,staticsIndex,staticsData);
	}
	_settings.endArray();

	QString gumpIndex = path.filePath(_settings.value("gumps/index","gumps.idx").toString());
	QString gumpData = path.filePath(_settings.value("gumps/data","gumps.mul").toString());
	mGumpsLoader = QSharedPointer<Gumps>(new Gumps(gumpIndex,gumpData,this), &Gumps::deleteLater);

	QString textureIndex = path.filePath(_settings.value("textures/index","texidx.mul").toString());
	QString textureData = path.filePath(_settings.value("textures/data","texmaps.mul").toString());
        mTexturesLoader = QSharedPointer<Textures>(new Textures(textureIndex,textureData,this), &Textures::deleteLater);

	QString tileData = path.filePath(_settings.value("tiledata","tiledata.mul").toString());
	mTileData = QSharedPointer<TileData>(new TileData(tileData,this), &TileData::deleteLater);

	_settings.endGroup();
}

Land Resources::getLand( ID _id, ID _hueID, bool _partialHue ) {
	GID gid(_id,mHues->getHue(_hueID,_partialHue));
	QSharedPointer<Land> result;
	if(mLands.contains(gid)) {
		result = mLands[gid].toStrongRef();
	} else {
		if(mLandsCache.contains(gid)) {
			result = QSharedPointer<Land>(mLandsCache.take(gid),cache<Land>);
		} else {
			result = QSharedPointer<Land>(new Land(gid),cache<Land>);
			result->setArt( mArtsLoader->getArtLand(result->getID(),result->getHue()) )
					.setInfo( mTileData->getLandInfo(result->getID()) );
			if(result->getInfo().mTextureID!=0)
				result->setTexture( mTexturesLoader->getEntry(result->getInfo().mTextureID,result->getHue()) );
		}
		if(!result.isNull())
			mLands.insert(gid,result.toWeakRef());
	}
	return result;
}

void Resources::moveToCache(Land* _land) {
	if(_land)
		delete _land;
}

Tile Resources::getTile( ID _id, ID _hueID, bool _partialHue ) {
	GID gid(_id,mHues->getHue(_hueID,_partialHue));
	QSharedPointer<Tile> result;
	if(mTiles.contains(gid)) {
		result = mTiles[gid].toStrongRef();
	} else {
		if(mTilesCache.contains(gid)) {
			result = QSharedPointer<Tile>(mTilesCache.take(gid),cache<Tile>);
		} else {
			result = QSharedPointer<Tile>(new Tile(gid),cache<Tile>);
			result->setInfo( mTileData->getTileInfo(result->getID()) );
			if(result->getInfo().mFlags.testFlag(TileData::PartialHue))
				result->setHue(mHues->getHue(_hueID,true));
			result->setArt(mArtsLoader->getArtStatic(_id,result->getHue()));
		}
		if(!result.isNull())
			mTiles.insert(gid,result.toWeakRef());
	}
	return result;
}

void Resources::moveToCache(Tile* _tile) {
	if(_tile)
		delete _tile;
}

Animation Resources::getAnimation( Body _body, ID _hueID, bool _partialHue ) {
	GID gid(_body,mHues->getHue(_hueID,_partialHue));
	QSharedPointer<Animation> result;
	if(mAnimations.contains(gid)) {
		result = mAnimations[gid].toStrongRef();
	} else {
		if(mAnimationsCache.contains(gid)) {
			result = QSharedPointer<Animation>(mAnimationsCache.take(gid),cache<Animation>);
		} else {
			result = QSharedPointer<Animation>(new Animation(gid),cache<Animation>);
		}
		if(!result.isNull())
			mAnimations.insert(gid,result.toWeakRef());
	}
	return result;
}

void Resources::moveToCache(Animation* _animation) {
	if(_animation)
		delete _animation;
}
