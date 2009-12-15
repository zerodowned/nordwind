#ifndef CBORDERGUMP_H_
#define CBORDERGUMP_H_

#include <Qt/qgl.h>

namespace resource {
	class CGump;
}

namespace gui {
	class CBorderGump : public QGLWidget
	{
		Q_OBJECT;
		private:
			resource::CGump* m_gumps[9];
			quint32 m_ids[9];
			QPoint m_vectors[16];
			quint32 m_hue;
			bool m_valid;
			bool m_partialHue;
			void updateMask();
			void loadGumps();
			void calcVectors( quint16 _width, quint16 _height );
		public:
			CBorderGump(QWidget* _parent, qint32 _x, qint32 _y, quint16 _width, quint16 _height, quint32 _id, quint32 _hue = 0, bool _partialHue = false, const QString& _name = "");
			virtual ~CBorderGump();
			void setIds( quint32 _topleft, quint32 _top, quint32 _topright, quint32 _left, quint32 _center, quint32 _right, quint32 _bottomleft, quint32 _bottom, quint32 _bottomright );
			void setId( quint32 _center );
			quint32 id() const;
			void setHue( quint32 _hue );
			quint32 hue() const;
			void setPartialHue( bool _partialHue );
			bool partialHue() const;
			// 0---1-----2---3
			// | 0 |  1  | 2 |
			// 4---5-----6---7
			// | 3 |  4  | 5 |
			// 8---9-----10-11
			// | 6 |  7  | 8 |
			// 12-13-----14-15
			QPoint& vector( quint8 _i );
			quint16 anchorsWidth() const;
			quint16 anchorsHeight() const;
			void resize( qint32 _w, qint32 _h );
		protected:
			void paintEvent( QPaintEvent* event ); 
		public slots:
		signals:
	};
}
#endif