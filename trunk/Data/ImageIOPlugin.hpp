/*
 * OSIImage.hpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */

#ifndef OSIIMAGE_HPP_
#define OSIIMAGE_HPP_

#include <QImageIOPlugin>
#include <QAbstractFileEngineHandler>
#include <QSettings>
#include <QHash>

class QSettings;
class ImageIOPlugin : public QImageIOPlugin
{
    public:
        QStringList keys() const;
        Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
        QImageIOHandler* create(QIODevice *device, const QByteArray &format = QByteArray()) const;
        class MULEngineHandler: public QAbstractFileEngineHandler {
        public:
                MULEngineHandler();
                virtual ~MULEngineHandler();
                QAbstractFileEngine *create(const QString &fileName) const;
                static const QChar separator;
                QHash<QString,QString> mTypeMap;
                QHash<QString,QString> mIndexMap;
                QScopedPointer<QSettings> mSettings;
        };
    private:
        MULEngineHandler mMulFile;
        static QStringList mKeys;
};

#endif
