#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <crtdbg.h>
#include <cstdlib>


#include "server/Server.h"

extern void error_handle(char *);

int main(int argc, char *args) 
{
#if defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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