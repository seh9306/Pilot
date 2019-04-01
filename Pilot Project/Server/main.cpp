
#include "main.h"
#include "server.h"

int main(int argc, char *args) {
	(new Server(9030))->init()->start();
	return 0;
}