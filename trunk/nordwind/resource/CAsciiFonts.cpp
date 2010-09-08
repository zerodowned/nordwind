#include "CAsciiFonts.h"
#include "../core/CLog.h"
#include <Qt/QFile.h>
#include <Qt/QDataStream.h>
#include <Qt/QImage.h>
#include "../core/CSettings.h"
#include <Qt/QStringList.h>
#include "CHues.h"

using namespace resource;

CAsciiEngine::CAsciiEngine() {
	m_fonts.resize( 10 );
	load();
}

CAsciiEngine::~CAsciiEngine() {
	unload();
}

void CAsciiEngine::load() {
	QString path = core::CSettings::instance().value( "Data/path", "" ).toString();
	// Set filenames
	QFile data(path + core::CSettings::instance().value( "Data/fonts_mul", "fonts.mul" ).toString() );

	// Open files
	if (!data.open(QIODevice::ReadOnly)) {
		CRIT_ERR(tr("Unable to open ascii font data at %1.").arg(data.fileName()));
	}

	// Set the devices for the streams
	QDataStream dataStream(&data);
	dataStream.setByteOrder(QDataStream::LittleEndian);

	// Load the font data
	quint8 header;
	quint16 pixel;
	quint8 i = 0;
	quint8 height, width; // Height of the entire font (max. height of all chars)
	while(!dataStream.atEnd()) {
		dataStream >> header;
		IAsciiFont& font = m_fonts[i];
		font.maxheight = 0;
		font.character.resize( 224 );

		for( quint8 c = 0; c < 224; ++c ) {
			dataStream >> width >> height >> header;

			IAsciiChar& character = font.getChar( c+32 );
			character.width = width;
			character.height = height;
			if( width > 0 && height > 0) {
				if (height > font.maxheight && c < 96) {
					font.maxheight = height;
				}
				quint16 size = width * height;
				character.data32.fill( 0x00000000, size );

				for( quint8 y = 0; y< height; y++ ) {
					for( quint8 x = 0; x < width; x++ ) {
						dataStream >> pixel;
						if(pixel!=0) {
							quint8 index = (pixel >> 10) & 0x1F;
                            quint8 r = index << 3; // Multiply with 8
                            quint8 g = (pixel >> 2) & 0xF8;
                            quint8 b = (pixel << 3) & 0xF8;
							quint16 pos = y*width+x;
							character.data32[pos] = qRgb( r, g, b );
						}
					}
				}
			} else {
				character.data32.clear();
			};
		}
		NOTICE(QString("Loaded ascii font %1 (Height: %2).").arg(i).arg(font.maxheight).toStdString());
		i++;
	}

	data.close();
}

void CAsciiEngine::unload() {
	m_fonts.clear();
}

void CAsciiEngine::reload() {
	unload();
	load();
}

QPixmap* CAsciiEngine::buildTextWrapped(quint8 font, const QString &text, quint16 maxWidth, quint32 hue, bool shaded, eTextAlign align, bool hueAll) {
	// Insert Newslines if the word would exceed the maxWidth boundary
	if(font>=m_fonts.size())
		return NULL;
	quint16 lineLength = 0;
	QString wrapped;
	quint8 spaceWidth = m_fonts[font].getChar(' ').width;

	// Split into words using the spaces
	QStringList parts = text.split(" ");
	for (QStringList::const_iterator it = parts.begin(); it != parts.end(); ++it) {
		QString word = *it;
		bool first = (it == parts.begin());
		quint16 wordWidth = 0;
		for (qint32 i = 0; i < word.length(); ++i)
			wordWidth += m_fonts[font].getChar(word.at(i).toLatin1()).width;

		if (!first) {
			wordWidth += spaceWidth;
		}

		// Check if the word still fits
		if (lineLength > 0 && lineLength + wordWidth > maxWidth) {
			wrapped += "\n";
			if (!first) {
				wordWidth -= spaceWidth; // We're not prepending the space anyway
			}
			lineLength = wordWidth;
		} else {
			if (!first) {
				wrapped += " ";				
			}
			lineLength += wordWidth;
		}

		wrapped += word;
	}

	return buildText(font, wrapped, hue, shaded, align, hueAll);
}

QPixmap *CAsciiEngine::buildText(quint8 font, const QString &text, quint32 hueid, bool shaded, eTextAlign align, bool hueAll) {
	if (font > m_fonts.size()) {
		font = 3; // Default back to font 3 if the font is invalid
	}

	quint16 width = 0; // Total width of the text	
	quint16 height = m_fonts[font].maxheight; // Total height of the text
	quint16 lineWidth = 0; // Length of the current line
	QVector<quint16> lineWidths; // Vector with the lengths of lines
	
	// Iterate over the string once to get the width of the string
	QString::ConstIterator it;
	for (it = text.begin(); it != text.end(); ++it) {
		if (*it == '\n') {
			if (lineWidth > width) {
				width = lineWidth;
			}
			lineWidths.append(lineWidth);
			lineWidth = 0;
		} else {
			lineWidth += m_fonts[font].getChar( (*it).toLatin1()).width;
		}
	}

	if (lineWidth > 0) {
		lineWidths.append(lineWidth);

		if (lineWidth > width) {
			width = lineWidth;
		}
	}

	quint16 baseline = height; // Store the baseline
	height = lineWidths.size() * height; // Increase the height of the text	

	QPixmap *img = 0; // The resulting text line

	if (width > 0) {
		img = new QPixmap(width, height, QPixmap::Format_ARGB32 );
		img->fill(0); // Clear the background of the surface

		// Start copying the characters over
		quint16 destx = 0;
		quint16 desty = 0;
		switch (align) {
			case eAlign_left:
				destx = 0; // Start on the left border
				break;
			case eAlign_center:
				destx = (width - lineWidths.front()) >> 1; // Take whats left of the total width and divide it into two
				break;
			case eAlign_right:
				destx = (width - lineWidths.front()); // Take the right part and use it as an offset
				break;
		}
		lineWidths.pop_front();

		for (it = text.begin(); it != text.end(); ++it) {
			if (*it == '\n') {
				if (!lineWidths.isEmpty()) {
					switch (align) {
						case eAlign_left:
							destx = 0; // Start on the left border
							break;
						case eAlign_center:
							destx = (width - lineWidths.front()) >> 1; // Take whats left of the total width and divide it into two
							break;
						case eAlign_right:
							destx = (width - lineWidths.front()); // Take the right part and use it as an offset
							break;
					}
					lineWidths.pop_front();
				}
				baseline += m_fonts[font].maxheight;
			} else {
				IAsciiChar& ch = m_fonts[font].getChar((*it).toLatin1());
				desty = baseline - ch.height;
				// MemCpy Line by Line
				for (quint16 yl = 0; yl < ch.height; ++yl) {
					quint32 *src = ch.data32.data()+yl*ch.width;
					quint8 *dest = img->scanLine(yl+desty)+destx*4;
					memcpy(dest, src, ch.width * 4);
				}
				destx += ch.width; // Increase for the next draw
			}
		}
	}

	QVector<Colour>* hue = Hues::instance().getHue( hueid );
	if(img && hue) {
		// Iterate over the surface
		quint8 r, g, b;
		Colour pixel;

		for (int x = 0; x < img->width(); ++x) {
			for (int y = 0; y < img->height(); ++y) {
				pixel = img->pixel(x, y);
				if (pixel != 0) {
					r = qRed(pixel);
					b = qBlue(pixel);
					g = qGreen(pixel);
					if (hueAll || (r == g && g == b)) {
						img->setPixel( x, y, hue->at(r >> 3) );
					}
				}
			}
		}
	}

	return img;
}

quint16 CAsciiEngine::getTextWidth( quint8 _font, const QString& _text, quint32 _start, quint32 _end ) {
	if (_font > m_fonts.size()) {
		_font = 3; // Default back to font 3 if the font is invalid
	}

	quint16 width = 0; // Total width of the text	
	
	for(quint32 i=_start; i < _end && i < _text.size(); i++ ) {
		if (*it == '\n') {
			width = 0;
		} else {
			lineWidth += m_fonts[font].getChar( _text.at(i).toLatin1()).width;
		}
	}
}