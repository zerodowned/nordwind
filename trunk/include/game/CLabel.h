#ifndef CLABEL_H_
#define CLABEL_H_

#include <Qt/qgl.h>
#include "../resource/CAsciiFonts.h"
#include <Qt/QImage.h>

namespace gui {
	class CLabel : public QGLWidget
	{
		Q_OBJECT;
		private:
			QImage* m_image;
			QString m_text;
			resource::eFontTypes m_font;
			quint8 m_fontid;
			quint32 m_hue;
		public:
			CLabel(QWidget* parent, qint32 _x, qint32 _y, QString& _text, const QString& _name = "");
			virtual ~CLabel();
			void syncImage();
			QImage& image() const;
			void setText( const QString& _str );
			QString text() const;
			void setHue( quint32 _hue );
			quint32 hue() const;
			void setFont( resource::eFontTypes _font, quint8 _id );
			quint8 fontId() const;
			resource::eFontTypes fontType() const;
		protected:
			void paintEvent( QPaintEvent* event );
		public slots:
			void onTextChange(const QString& _str);
			void onHueChange( quint32 _hue);
			void onFontChange( resource::eFontTypes _font, quint8 _id );
		signals:
			void textChange( const QString& _str );
			void hueChange( quint32 _hue );
			void fontChange( resource::eFontTypes _font, quint8 _id );
	};
}
#endif