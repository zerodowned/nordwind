#include "Client.hpp"

int main(int argc, char *argv[]) {
	Client client(argc, argv);
//	client.load();
    return client.exec();
}
