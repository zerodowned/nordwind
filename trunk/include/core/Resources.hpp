#ifndef RESOURCES_HPP__
#define RESOURCES_HPP__

#include <qobject.h>
#include "../util/Singleton.hpp"
#include "../resource/Animations.hpp"
#include "../resource/Arts.hpp"
#include "../resource/Facets.hpp"
#include "../resource/Gumps.hpp"
#include "../resource/Hues.hpp"
#include "../resource/Textures.hpp"
#include "../resource/TileData.hpp"
#include <qsettings.h>

namespace resource {

	class Resources : public QObject {
		Q_OBJECT;
	private:
		QSharedPointer<Animations> mAnimations;
		QSharedPointer<Arts> mArts;
		QSharedPointer<Gumps> mGumps;
		QSharedPointer<Hues> mHues;
		QSharedPointer<Facets> mFacets;
		QSharedPointer<Textures> mTextures;
		QSharedPointer<TileData> mTileData;
	public:
		  Resources( QSettings& _settings, QObject* _parent);
		  virtual ~Resources();

		  Animation getAnimation( Body _id, Action _action, Direction _direction, ID _hueID = 0, bool _partialHue = false ) ;
		  Texture getLandTexture( ID _id, ID _hueID = 0, bool _partialHue = false ) ;
		  Art getLandArt( ID _id, ID _hueID = 0, bool _partialHue = false ) ;
		  Art getStaticArt( ID _id, ID _hueID = 0, bool _partialHue = false ) ;
//		  QImage* getCursor( qint32& _hotX, qint32& _hotY, quint32 _id, quint32 _hue, bool _partialHue );
		  Gump getGump( ID _id, ID _hueID = 0, bool _partialHue = false) ;
		  Texture getTexture( ID _id, ID _hueID = 0, bool _partialHue = false );
		  QSharedPointer<TileData::Entry> getTileInfo( ID _id ) const;
		  Hue getHue( ID _hueID, bool _partialHue = false ) const;
		  Facet getFacet( QString _name );
	};

	inline QSharedPointer<TileData::Entry> Resources::getTileInfo( ID _id ) const {
		return mTileData->getEntry( _id );
	}

	inline Hue Resources::getHue( ID _hueID, bool _partialHue ) const {
		return mHues->getHue(_hueID,_partialHue);
	}
}
#endif
