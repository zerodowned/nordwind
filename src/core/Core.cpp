#include "CCore.h"
#include "CLog.h"
#include "CSettings.h"
#include "../resource/CAnimations.h"
//#include "../resource/CAsciiFonts.h"
#include "../resource/CArts.h"
#include "../resource/CGumps.h"
#include "../resource/CHues.h"

using namespace core;

Core::Core(QObject* _parent)
    : QObject(_parent) {
	// initialize config.ini and log files
	CSettings::getInstance();
	Log::getInstance();
	INFO("Log initialized");
	INFO(QString("Config %1 loaded.").arg(core::CSettings::getInstance().fileName()));
	load();
}

Core::~Core(void)	{
	unload();
	// deinitialize settings and logs
	core::Log::destroyInstance();
	core::CSettings::destroyInstance();
}

void Core::load() {
	// initialize resource files
    resource::CResourceCache::instance();
	resource::Hues::instance();
	//resource::CAsciiEngine::instance();
    resource::Gumps::instance();
	resource::CArts::instance();
	resource::CAnimations::instance();
}

void Core::unload() {
	CSettings::instance().sync();
	// deinitialize resources
	resource::CAnimations::destroyInstance();
	resource::CArts::destroyInstance();
	resource::Gumps::destroyInstance();
	//resource::CAsciiEngine::destroyInstance();
	resource::Hues::destroyInstance();
	resource::CResourceCache::destroyInstance();
}

void Core::reload() {
	unload();
	load();
}