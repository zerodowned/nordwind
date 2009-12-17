#include "core/Resources.hpp"
#include "resource/Cache.hpp"
#include <qdir.h>

using namespace resource;

Resources::Resources( QSettings& _settings, QObject* _parent )
: QObject(_parent) {
	_settings.beginGroup("resources");

	Cache::instance();

	QDir path( _settings.value( "path", "." ).toString() );

	QString hue = path.filePath(_settings.value("hue", "hues.mul").toString());
	mHues = QSharedPointer<Hues>(new Hues(hue, this), &Hues::deleteLater);

	QString animIndex = path.filePath(_settings.value("animations/index","anim.idx").toString());
	QString animData = path.filePath(_settings.value("animations/data","anim.mul").toString());
	mAnimations = QSharedPointer<Animations>(new Animations(animIndex,animData,this), &Animations::deleteLater);

	QString artIndex = path.filePath(_settings.value("arts/index","artidx.mul").toString());
	QString artData = path.filePath(_settings.value("arts/data","art.mul").toString());
	QString artAnim = path.filePath(_settings.value("arts/animdata","animdata.mul").toString());
	mArts = QSharedPointer<Arts>(new Arts(artAnim,artIndex,artData,this), &Arts::deleteLater);

	mFacets = QSharedPointer<Facets>(new Facets(this), &Facets::deleteLater);
<<<<<<< .mine
        for( int i = _settings.beginReadArray("facets")-1; i >= 0; i-- ) {
=======
	for( int i = _settings.beginReadArray("facets")-1; i >= 0; i-- ) {
>>>>>>> .r23
		_settings.setArrayIndex(i);
                QString name = _settings.value("name").toString();
		QString map = path.filePath(_settings.value("map").toString());
		QSize dimension = _settings.value("size").toSize();
		QString staticsIndex = path.filePath(_settings.value("index").toString());
		QString staticsData = path.filePath(_settings.value("data").toString());
		mFacets->addFacet(name,map,dimension,staticsIndex,staticsData);
	}
	_settings.endArray();

	QString gumpIndex = path.filePath(_settings.value("gumps/index","gumps.idx").toString());
	QString gumpData = path.filePath(_settings.value("gumps/data","gumps.mul").toString());
	mGumps = QSharedPointer<Gumps>(new Gumps(gumpIndex,gumpData,this), &Gumps::deleteLater);

	QString textureIndex = path.filePath(_settings.value("textures/index","texidx.mul").toString());
	QString textureData = path.filePath(_settings.value("textures/data","texmaps.mul").toString());
//	mTextures = QSharedPointer<Textures>(new Textures(textureIndex,textureData,this), &Textures::deleteLater);

	QString tileData = path.filePath(_settings.value("tiledata","tiledata.mul").toString());
	mTileData = QSharedPointer<TileData>(new TileData(tileData,this), &TileData::deleteLater);

	_settings.endGroup();
}

Resources::~Resources() {
	Cache::destroyInstance();
}

Animation Resources::getAnimation( Body _id, Action _action, Direction _direction, ID _hueID, bool _partialHue ) {
	return mAnimations->getAnimation( _id, _action, _direction, getHue(_hueID,_partialHue));
}

Texture Resources::getLandTexture( ID _id, ID _hueID, bool _partialHue ) {
	return mTextures->getEntry(_id, getHue(_hueID,_partialHue));
}

Art Resources::getLandArt( ID _id, ID _hueID, bool _partialHue ) {
	return mArts->getArtLand( _id, getHue(_hueID,_partialHue));
}
Art Resources::getStaticArt( ID _id, ID _hueID, bool _partialHue ) {
	return mArts->getArtStatic(_id,getHue(_hueID,_partialHue));
}
//		  QImage* getCursor( qint32& _hotX, qint32& _hotY, quint32 _id, quint32 _hue, bool _partialHue );
Gump Resources::getGump( ID _id, ID _hueID, bool _partialHue) {
	return mGumps->getEntry(_id, getHue(_hueID,_partialHue));
}

Texture Resources::getTexture( ID _id, ID _hueID, bool _partialHue ) {
	return mTextures->getEntry(_id,getHue(_hueID,_partialHue));
}

Facet Resources::getFacet( QString _name ) {
	return mFacets->getFacet(_name);
}
