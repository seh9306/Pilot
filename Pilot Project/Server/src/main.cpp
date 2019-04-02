
#include "server/Server.h"

int main(int argc, char *args) {
	Server server;
	server.Init();
	server.Start();

	return 0;
}