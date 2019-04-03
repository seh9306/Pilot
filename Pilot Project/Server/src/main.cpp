
#include "server/Server.h"

extern void error_handle(char *);

int main(int argc, char *args) 
{
	// create Server
	Server server;
	int code = 0;

	// initialize server
	if ((code = server.Init()) == -1) 
	{

		switch (code) 
		{

		case kBindError:
			error_handle("bind() error !");
			exit(1);
			break;
		case kListenError:
			error_handle("listen() error !");
			exit(1);
			break;

		}
	}

	// start server
	if ((code = server.Start()) != 0)
	{
		error_handle("Start() error !");
		exit(1);
	}

	return 0;
}