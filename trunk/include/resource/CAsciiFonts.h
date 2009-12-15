#ifndef CASCIIFONTS_H
#define CASCIIFONTS_H

#include <Qt/QString.h>
#include <Qt/QObject.h>
#include <Qt/QVector.h>
#include "../core/ISingleton.h"
#include <Qt/QRgb.h>

class QImage;

namespace resource {
	enum eTextAlign {
		eAlign_left = 0,
		eAlign_center,
		eAlign_right
	};
	enum eFontTypes {
		eFont_Ascii
	};
	class CAsciiEngine : public QObject, public Singleton<CAsciiEngine> {
		friend class Singleton<CAsciiEngine>;
		Q_OBJECT;
		private:
			struct IAsciiChar {
				QVector<Colour> data32;
				quint8 width;
				quint8 height;
			};
			struct IAsciiFont {
				// grayscaled -> quint8
				QVector<IAsciiChar> character;
				quint8 maxheight;
				IAsciiChar& getChar( quint8 _char ) {
					return character[(_char>=32) ? _char-32 : 0];
				};
			};
			// The fonts are all read in at once since 
			// the file has no indexer and is relatively small (ca. 800kb)
			QVector<IAsciiFont> m_fonts;
			CAsciiEngine();
			~CAsciiEngine();
		public:
			void load();
			void unload();
			void reload();

			quint8 getFontHeight( quint8 _font );
			quint16 getTextWidth( quint8 _font, const QString& _text, quint32 _start = 0, quint32 _end = 0xFFFFFFFF );
			// Build a text string
			QImage* buildText(quint8 font, const QString &text, quint32 hue = 0, bool shaded = false, eTextAlign align = eAlign_left, bool hueAll = false);
			QImage* buildTextWrapped(quint8 font, const QString &text, quint16 maxWidth, quint32 hue = 0, bool shaded = false, eTextAlign align = eAlign_left, bool hueAll = false);
	};
}

#endif
