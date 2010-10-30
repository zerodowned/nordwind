#include "Data.hpp"
#include "Settings.hpp"
#include "data/MulFileEngine.hpp"

#include <qdebug.h>

using namespace data;

const QString Data::resource(Data::Type type, ID id) {
	Settings& settings(Settings::instance());
	switch(type) {
	case Data::Animation:
		return settings.value("muls/animation/file","anim.mul").toString() + MulFileEngine::separator + QString::number(id);
	case Data::Art:
		return settings.value("muls/art/file","art.mul").toString() + MulFileEngine::separator + QString::number(id);
	case Data::Texture:
		return settings.value("muls/texture/file","texmaps.mul").toString() + MulFileEngine::separator + QString::number(id);
	default:
		qWarning() << "Unknown resource" << type;
		return QString();
	}
}
