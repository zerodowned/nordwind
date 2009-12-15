#ifndef CGUIIMAGE_H
#define CGUIIMAGE_H

#include <Qt/QObject.h>
#include <Qt/qgl.h>

namespace resource {
	class CGump;
}

namespace gui {

	class CGumpImage : public QGLWidget {
		Q_OBJECT;
		private:
			resource::CGump* m_gump;
			quint32 m_id;
			quint32 m_hue;
			bool m_partialHue;
			bool m_resize;
			bool m_tiled;
			void buildConnections();
		public:
			CGumpImage( QWidget* _parent, qint32 _x, qint32 _y, quint32 _id, quint32 _hue = 0, bool _partialHue = false, const QString& _name = "", quint16 _width = 0xFFFF, quint16 _height = 0xFFFF, bool _resize = true, bool _tiled = false );
			virtual ~CGumpImage();
			quint32 id() const;
			void setId( quint32 _id );
			quint32 hue() const;
			void setHue( quint32 _hue );
			bool partialHue() const;
			void setPartialHue( bool _partialHue );
			bool isResize() const;
			void setResize( bool _resize );
			bool isTiled() const;
			void setTiled( bool _tile );
			void setType( quint32 _id, quint32 _hue = 0, bool _partialHue = false, bool _resize = true );
		protected:
			void paintEvent( QPaintEvent* event );
		public slots:
			void onGumpChange();
			void onHueChange();
			void onIdChange();
			void onResizeChange();
			void onPartialHueChange();
		signals:
			void gumpChange();
			void idChange();
			void hueChange();
			void resizeChange();
			void partialHueChange();
	};
}

#endif