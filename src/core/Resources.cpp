#include "core/Resources.hpp"
#include "resource/Cache.hpp"
#include <qdir.h>

using namespace resource;

Resources::Resources( QSettings& _settings, QObject* _parent )
: QObject(_parent) {
	_settings.beginGroup("resources");

	Cache::instance();

	QDir path( _settings.value( "path", "." ).toString() );

	QString hue = _settings.value("hue", "hues.mul").toString();
	mHues = QSharedPointer<Hues>(new Hues(path.filePath(hue), this), &Hues::deleteLater);

	QString animIndex = _settings.value("animations/index","anim.idx").toString();
	QString animData = _settings.value("animations/data","anim.mul").toString();
	mAnimations = QSharedPointer<Animations>(new Animations(animIndex,animData,this), &Animations::deleteLater);

	QString artIndex = _settings.value("arts/index","artidx.mul").toString();
	QString artData = _settings.value("arts/data","art.mul").toString();
	QString artAnim = _settings.value("arts/animdata","animdata.mul").toString();
	mArts = QSharedPointer<Arts>(new Arts(artAnim,artIndex,artData,this), &Arts::deleteLater);

	mFacets = QSharedPointer<Facets>(new Facets(this), &Facets::deleteLater);
	for( int i = _settings.beginReadArray("facets"); i > 0; i-- ) {
		_settings.setArrayIndex(i);
		QString name = _settings.value("name").toString();
		QString map = _settings.value("map").toString();
		QSize dimension = _settings.value("size").toSize();
		QString staticsIndex = _settings.value("index").toString();
		QString staticsData = _settings.value("data").toString();
		mFacets->addFacet(name,map,dimension,staticsIndex,staticsData);
	}
	_settings.endArray();

	QString gumpIndex = _settings.value("gumps/index","gumps.idx").toString();
	QString gumpData = _settings.value("gumps/data","gumps.mul").toString();
	mGumps = QSharedPointer<Gumps>(new Gumps(gumpIndex,gumpData,this), &Gumps::deleteLater);

	QString textureIndex = _settings.value("textures/index","texidx.mul").toString();
	QString textureData = _settings.value("textures/data","texmaps.mul").toString();
//	mTextures = QSharedPointer<Textures>(new Textures(textureIndex,textureData,this), &Textures::deleteLater);

	QString tileData = _settings.value("tiledata","tiledata.mul").toString();
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
	return mGumps->getEntry(_id, getHue(_id,_partialHue));
}

Texture Resources::getTexture( ID _id, ID _hueID, bool _partialHue ) {
	return mTextures->getEntry(_id,getHue(_hueID,_partialHue));
}

Facet Resources::getFacet( QString _name ) {
	return mFacets->getFacet(_name);
}
