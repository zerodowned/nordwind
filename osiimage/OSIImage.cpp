#include "OSIImage.hpp"
#include "OSIImageIOHandler.hpp"

inline QStringList OSIPlugin::keys() const
{
    return QStringList() << QLatin1String("animation") << QLatin1String("art") << QLatin1String("gump") << QLatin1String("texture");
}

inline QImageIOPlugin::Capabilities OSIPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (!device||!(device->isOpen()&&device->isReadable()) || !keys().contains(format))
        return 0;

    Capabilities cap;
    if(OSIImageIOHandler::canRead(device,format))
        return cap |= CanRead;

    return cap;
}

QImageIOHandler *OSIPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *hand = new OSIImageIOHandler(device,format);
    return hand;
}
