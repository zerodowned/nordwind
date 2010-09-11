/*
 * OSIImage.hpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */

#ifndef OSIIMAGE_HPP_
#define OSIIMAGE_HPP_
#include <qimageiohandler.h>

class OSIPlugin : public QImageIOPlugin
{
    public:
        QStringList keys() const;
        Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
        QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};
Q_EXPORT_PLUGIN2(OSI, OSIPlugin)
#endif
