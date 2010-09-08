#include "core/Module.hpp"

using namespace core;


bool Module::loadSettings() {
	bool result = false;
	QString fileName = getName().append(".ini");
	if( QFile::exists(fileName)) {
		mSettings = new Settings( getName(), fileName, Settings::getRoot() );
		Q_CHECK_PTR(m_settings);
		Q_ASSERT_X(mSettings->status()==QSettings::NoError,mSettings->fileName(), "Invalid Format" );
		qDebug() << mSettings->fileName() << ": Configuration file loaded.";
		result = true;
	}

	// fallback scenario
	if (result==false){
		if(mSettings)
			delete mSettings;
		mSettings = Settings::getRoot();
	}

};

Module::Module( QObject* _parent )
: QThread(_parent), mSettings(NULL) {
}


Module::~Module() {
	if(mSettings&&!mSettings->isRoot()) {
		delete mSettings;
	}
}

bool Module::startup() {
	qDebug() << "initialize...";
	if(loadSettings()) {
		run();
	}
	return false;
};

bool Module::shutdown() {
	qDebug() << "deinitialize...";
	quit();
	return true;
};

bool Module::reload() {
	qDebug() << "reload...";
	return (unload() && load());
};

void Module::run() {
	load();
	QThread::run();
	unload();
}

