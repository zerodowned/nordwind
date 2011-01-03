#include "ImageIOPlugin.hpp"
#include "ImageIOHandler.hpp"
#include "AnimationIOHandler.hpp"
#include "ArtIOHandler.hpp"
#include "GumpIOHandler.hpp"
#include "TextureIOHandler.hpp"
#include "Index.hpp"
#include "MulFileEngine.hpp"

#include <QSettings>
#include <QApplication>

inline QStringList ImageIOPlugin::keys() const
{
    return QStringList() << "anim" << "art" << "map" << "gump" << "tex";
}

inline QImageIOPlugin::Capabilities ImageIOPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (!device||!keys().contains(format)||!(device->isOpen()&&device->isReadable()))
        return 0;

    Capabilities cap;
    return cap|CanRead;
}

QImageIOHandler *ImageIOPlugin::create(QIODevice *device, const QByteArray &format) const {
        if(format=="anim")
                return new AnimationIOHandler(device);
        else if(format=="art")
                return new ArtIOHandler(device);
        else if(format=="map")
                return new ArtIOHandler(device,format);
        else if(format=="gump")
                return new GumpIOHandler(device);
        else if(format=="tex")
                return new TextureIOHandler(device);
        else
                return NULL;
}

const QChar ImageIOPlugin::MULEngineHandler::separator = ':';

ImageIOPlugin::MULEngineHandler::MULEngineHandler()
: QAbstractFileEngineHandler() {
    mIndexMap["anim.mul"] = "anim.idx";
    mIndexMap["art.mul"] = "artidx.mul";
    mIndexMap["gumpart.mul"] = "gumpidx.mul";
    mIndexMap["light.mul"] = "lighidx.mul";
    mIndexMap["skills.mul"] = "skills.idx";
    mIndexMap["sound.mul"] = "soundidx.mul";
    mIndexMap["statics0.mul"] = "staidx0.mul";
    mIndexMap["texmaps.mul"] = "texidx.mul";
}

ImageIOPlugin::MULEngineHandler::~MULEngineHandler() {
}

/** Creates a MUL file handler
  * Handles indexed MUL containers
  * e.g. C:/Ultima Online/art.mul/5 for art with the id 5
  * /Ultima Online/art.mul/5/color
  */
QAbstractFileEngine* ImageIOPlugin::MULEngineHandler::create(const QString& resource) const {
    Q_FOREACH(QString mulFile, mIndexMap.keys()) {
        int lastIndex = resource.lastIndexOf(mulFile);
        if(lastIndex!=-1) {
            QString identifier = resource.mid(lastIndex+mulFile.size()).section(QDir::separator(),1,1);
            quint32 index = identifier.toUInt();
            return new MulFileEngine(resource.left(lastIndex) + mulFile,
                                     resource.left(lastIndex) + mIndexMap[mulFile],
                                     index);
        }
    }
    return 0;
}


Q_EXPORT_PLUGIN2(Data, ImageIOPlugin);

