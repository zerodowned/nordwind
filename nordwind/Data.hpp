#ifndef RESOURCES_HPP__
#define RESOURCES_HPP__

#include "Typedefs.hpp"

// DECLARATIONS
class Data : public QObject {
Q_OBJECT;
public:
	enum Type {
		Animation,
		Art,
		Map,
		Texture,
		Gump
	};
	Q_ENUMS(Type);
	static const QString resource(Type type, ID id);
};

#endif
