#include "Client.hpp"
#include <qplugin.h>
#include "resource/MulFileEngine.hpp"

int main(int argc, char *argv[]) {
	Client client(argc, argv);
	resource::MulFileEngineHandler mulEngineHandler;
    return client.load() ? client.exec() : -1;
}
