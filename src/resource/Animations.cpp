/*
 * Animation.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */
#include "resource/Animations.hpp"
#include "core/Resources.hpp"
#include "Client.hpp"
#include <qfile.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qdebug.h>

using namespace resource;

//IndexFile& Animations::getAnimFile(Body& _body) const {
//	if(!mFileMapping.contains( _body ))
//		return _body;
//	qint8 file = mFileMapping[_body].second;
//	if(!mAnimFiles.contains(file))
//		return *this;
//	_body = mFileMapping[_body].first;
//	return file;
//}

// West = 0, Right = 1, East = 2, Down = 3, South = 4, Left = 5, North = 6, Up = 7
const quint8 Animations::mDirectionMap[8] = {3, 2, 1, 0, 1, 2, 3, 4};
const bool Animations::mDirectionFlip[8] = {true, true, true, false, false, false, false, false};

Animations::Animations(QString _indexFile, QString _dataFile, QObject* _parent )
: IndexFile(_indexFile,_dataFile,_parent) {
	mStructure.push_back( Section(Animations::Monster,SectionStructure(200,110) ) );
	mStructure.push_back( Section(Animations::Animal,SectionStructure(200,65) ) );
	mStructure.push_back( Section(Animations::Character,SectionStructure(2,175) ) );
	mStructure.push_back( Section(Animations::Equipment,SectionStructure(0xFFFFFFFF,175) ) );

//	Anim2
//		200 monsters � 110 actions
//		x animals � 65 actions
//	Anim3
//		300 animals � 65 actions
//		100 monsters � 110 actions
//		x equipement � 175 actions
//	Anim4
//		200 animals � 65 actions
//		200 monsters � 110 actions
//		x equipement ``a 175 actions
}

Animations& Animations::loadBodyDef( QString _fileName ) {
	// Format is:
	// oldbody {newbody, newbody, newbody} newhue
	QRegExp pattern("^\\s*(\\d+)\\s+\\{([^\\}]+)\\}\\s+(\\d+)\\s*$");
	QRegExp splitPattern("\\s*,\\s*");

	QFile file(_fileName);
	QTextStream stream(&file);
	Q_ASSERT_X(stream.device()->open(QIODevice::ReadOnly), __PRETTY_FUNCTION__, _fileName.toAscii().constData());
	bool ok = false;
	quint32 iLine = 0;
	while (!stream.atEnd()) {
		QString line = stream.readLine();
		++iLine;
		if (!line.startsWith("//") && line.contains(pattern)) {
			// Parse the list of new ids
			QStringList newIds = pattern.cap(2).split(splitPattern);
			Body oldBody = pattern.cap(1).toUInt(&ok);
			if(!ok)
				oldBody = pattern.cap(1).toUInt( &ok, 16 );
			if(!ok) {
				qDebug() << "Incorrect body in" << file.fileName() << " at line" << iLine;
				continue;
			}
			ID hue = pattern.cap(3).toUInt();
			if (newIds.size() > 0) {
				mFallback.insert(oldBody, QPair<Body, ID>(  newIds[0].toUInt(), hue ));
			}
		}
	}
	return *this;
}

//Animations& Animations::loadMobTypesTxt(QString path) {
//	QFile file(path);
//
//	if (!file.open(QFile::ReadOnly)) {
//		qDebug() << "Unable to open mobile type data from " << file.fileName();
//		return;
//	}
//
//	// Format is:
//	// body type(string) flags
//	QTextStream stream(&file);
//	quint32 iLine = 0;
//	while (!stream.atEnd()) {
//		QString line = stream.readLine().trimmed();
//		++iLine;
//		if (!line.startsWith("//")) {
//			QStringList parts = line.split(QRegExp("\\s+"));
//
//			// 3 parts required
//			if (parts.size() == 3) {
//				bool ok = false;
//				ushort body = parts[0].toUInt( &ok );
//				if(!ok) {
//					body = parts[0].toUInt( &ok, 16 );
//				}
//				if(!ok) {
//					qDebug() << "Incorrect body in" << file.fileName() << " at line" << iLine;
//					continue;
//				}
//
//				quint16 flags = parts[2].toUShort(&ok); // Parse flags
//				if(!ok) {
//					qDebug() << "Incorrect flag in " << file.fileName() << " at line" << iLine;
//					continue;
//				}
//
//				// Parse the body type
//                eAnimationType type;
//				if (parts[1] == "HUMAN") {
//					type = eAnim_human;
//				} else if (parts[1] == "ANIMAL") {
//					type = eAnim_animal;
//				} else if (parts[1] == "MONSTER") {
//					type = eAnim_monster;
//				} else if (parts[1] == "EQUIPMENT") {
//					type = eAnim_equipment;
//				} else {
//					qDebug() << "Unknown type in" << file.fileName() << " at line" << iLine;
//					continue;
//				}
//				m_mobTypes.insert( (quint32)body, QPair<eAnimationType, quint16>(type, flags) );
//			}
//		}
//	}
//}
//
//Animations& Animations::loadBodyConvDef(QString path) {
//	QFile file(path);
//
//	if (!file.open(QFile::ReadOnly)) {
//		qDebug() << "Unable to open body conversion data from" << file.fileName();
//		return;
//	}
//
//	// Format is:
//	// body anim2 anim3 anim4
//	QTextStream stream(&file);
//	bool ok = false;
//	quint32 iLine = 0;
//	while (!stream.atEnd()) {
//		QString line = stream.readLine().trimmed();
//		++iLine;
//		if (!line.startsWith("//")) {
//			QStringList parts = line.split(QRegExp("\\s+"));
//
//			// 4 parts required
//			if (parts.size() == 4) {
//				quint32 body = parts[0].toUInt(&ok);
//				if(!ok) {
//					body = parts[0].toUInt(&ok, 16 );
//				}
//				if(!ok) {
//					qDebug() << "Incorrect body in" << file.fileName() << " at line" << iLine;
//					continue;
//				}
//				for (int i = 1; i < 5; ++i) {
//					quint32 newbody = parts[i].toUInt(&ok);
//					if (ok && newbody != 0xFFFFFFFF) {
//						m_fileMapping.insert(body, QPair<quint32,qint8>(newbody,i));
//						break;
//					}
//				}
//			}
//		}
//	}
//}

Sequence Animations::getSequence( Body _body, Hue _hue, Action _action, Direction _direction ) {
	getFallback(_body,_action,_direction,_hue);
	QByteArray data = getData( calculateID(_body, _action, _direction) );
	return !data.isEmpty() ? decode(data,_action,_direction,_hue) : Sequence();
}

Animations& Animations::getFallback( Body& _body, Action& _action, Direction& _direction, Hue&_hue ) {
	if(mFallback.contains(_body)) {
		_hue = Client::getInstance()->resources()->hues()->getHue( mFallback[_body].second, _hue->isPartialHue() );
		_body = mFallback[_body].first;
	}
	return *this;
}

ID Animations::calculateID(Body _body, Action _action, Direction _direction) {
	ID result = 0;
	foreach( Animations::Section section, mStructure ) {
		if(_body<section.second.first) {
			result += _body * section.second.second;
			break;
		} else {
			result += section.second.first * section.second.second;
			_body -= section.second.first;
		}
	}
	result += _action * 5 + mDirectionMap[_direction&7];
	return result;
}

Sequence Animations::decode(QByteArray _data,Action _action,Direction _direction,Hue _hue ) {
	QDataStream stream(_data);
	stream.setByteOrder(QDataStream::LittleEndian);

	QVector<Colour16> col16Table( 0xFF );
	stream.readRawData( (char*)col16Table.data(), sizeof(Colour16)<<8 );
	QVector<Colour> hueTable;
	foreach( Colour16 col16, col16Table ) {
		hueTable.push_back( _hue->mapToHue(col16) );
	}

	quint32 frameCount = 0;
	stream >> frameCount;
	QVector<quint32> offsetTable( frameCount );
	stream.readRawData( (char*)offsetTable.data(), sizeof(quint32)*frameCount );

	Sequence sequence;
	foreach(quint32 offset, offsetTable) {
		stream.device()->seek(offset);
		sequence.push_back( decodeFrame(stream,_direction,hueTable) );
	}
	return sequence;
}

Frame Animations::decodeFrame( QDataStream& _stream, Direction _direction, QVector<Colour> _hueTable ) {
	qint16 centerX, centerY;
	_stream >> centerX >> centerY;
	Center center( centerX, centerY );

	quint16 width, height;
	_stream >> width >> height;
	QImage image( width, height, QImage::Format_ARGB32_Premultiplied);
	image.fill(0);

	if( height != 0 && width != 0 ) {
		// Start reading the frame runs
		do {
			quint32 header = 0x7FFF7FFF;
			_stream >> header; // Read the frame header

			// This is the end marker
			if (header == 0x7FFF7FFF) {
				break;
			}

			// Unpack the header
			qint16 xoffset, yoffset;
			quint16 length; // Chunk properties
			xoffset = (header >> 22) & 0x3FF;
			yoffset = (header >> 12) & 0x3FF;
			length = header & 0xFFF;

			if (xoffset & 0x200) {
				xoffset |= 0xFC00; // This is suboptimal
			}
			if (yoffset & 0x200) {
				yoffset |= 0xFC00; // This is suboptimal
			}

			qint32 y = centerY + yoffset + height;
			qint32 x = centerX + xoffset;
			if(mDirectionFlip[_direction]) {
				x = width - x;
			}
			for (quint16 i = 0; i < length; i++, x++) {
				quint8 pixel;
				_stream >> pixel;
				image.setPixel( x, y, _hueTable[pixel] );
			}
		} while (true);
	}
	return Frame(center,QPixmap::fromImage(image));
}
