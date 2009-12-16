#include "Client.hpp"

int main(int argc, char *argv[]) {
	Client client(argc, argv);
    return (client.load()) ? client.exec() : -1;
}
