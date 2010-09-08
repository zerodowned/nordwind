#include "Resources.hpp"
#include "Client.hpp"
#include "resource/MulFileEngine.hpp"
#include "resource/IndexFileHandle.hpp"
#include <qsettings.h>
#include <qfiledialog.h>
#include <qdir.h>
#include <qdatetime.h>
#include <qdebug.h>

using namespace resource;

Resources::Resources( QObject* _parent )
: QObject(_parent) {
	QTime time;
	QSettings& settings( Client::settings() );

	QString folderPath = settings.value("path").toString();
	if(folderPath.isEmpty())
		folderPath = QFileDialog::getExistingDirectory(NULL,tr("Select UO Data folder"),QDir::currentPath());
	QDir path(folderPath);

	settings.beginGroup("muls");
	time.start();
	Q_FOREACH(QString type, settings.childGroups()) {
		settings.beginGroup(type);
		QString mulFile = settings.value("file").toString();
		QString indexFile = settings.value("index").toString();
		if(!indexFile.isNull())
			IndexFileHandle::instance(mulFile,indexFile);
		QFile::exists(QString("%1:%2").arg(settings.value("file").toString()).arg(0));
		settings.endGroup();
		qDebug() << type << "loaded with MulFileEngine took aprox." << time.restart() << "ms";
	}
	settings.endGroup();

	// old loader style
	settings.beginGroup("resource");
//	QString hue = path.filePath(settings.value("hue", "hues.mul").toString());
//	mHues.reset(new Hues(hue, this));
//	qDebug() << QString("Hue load took aprox. %1 ms").arg(time.restart());

	QString tileData = path.filePath(settings.value("tiledata","tiledata.mul").toString());
	mTileData.reset(new TileData(tileData));
	qDebug() << QString("Tiledata load took aprox. %1 ms").arg(time.restart());
	settings.endGroup();
}

QString Resources::artFile() {
	return Client::settings().value("muls/art/file","art.mul").toString();
}

QString Resources::textureFile() {
	return Client::settings().value("muls/texture/file","texmaps.mul").toString();
}

QString Resources::animationFile() {
	return Client::settings().value("muls/animation/file","anim.mul").toString();
}

const TileData& Resources::tileData() const {
	return *mTileData;
}
