#ifndef CPOSITION_H__
#define CPOSITION_H__

#include <Qt/QString.h>

namespace world {
	class CPosition {
		private:
			quint32 m_x;
			quint32 m_y;
			qint16 m_z;
			quint16 m_layer;
		public:
			CPosition();
			CPosition( quint16 _x, quint16 _y, qint8 _z, quint16 _layer = 0 );
			CPosition( quint32 _x, quint32 _y, qint16 _z, quint16 _layer = 0 );
			CPosition( const CPosition& _r );
			CPosition& move( qint32 _x = 0, qint32 _y = 0, qint16 _z = 0 );
			CPosition& setX( quint16 _x );
			CPosition& setX( quint32 _x );
			CPosition& setY( quint16 _y );
			CPosition& setY( quint32 _y );
			CPosition& setZ( qint8 _z );
			CPosition& setZ( qint16 _z );
			CPosition& setLayer( quint16 _layer );
			inline CPosition& set( quint32 _x, quint32 _y, qint32 _z ) {
				return setX( _x ).setY( _y ).setZ( _z );
			};
			inline CPosition& set( quint16 _x, quint16 _y, qint8 _z ) {
				return setX( _x ).setY( _y ).setZ( _z );
			};
			quint32 blockId() const;
			//double calcRealZ();
			quint32& x() const;
			quint32& y() const;
			qint16& z() const;
			quint16& layer() const;
	}
}
#endif