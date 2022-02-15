#include "core/ipcSocekt.h"

#include <iostream>
#include "string.h"
#include <unistd.h>

void messageCallback(std::string &name, void* data, int len)
{
	std::string tmp(static_cast<char*>(data));
	std::cout << "recv process name " << name << " data=" << tmp << " len=" << len << std::endl;
}

int main(int argc, char *argv[])
{
	std::cout << "test" << std::endl;


	std::string process1(argv[1]);
	std::string process2(argv[2]);
	std::string data(argv[3]);

	IPC_Init(process1, messageCallback);

	while (1)
	{
		IPC_SendMsgToProcess(process2, (void*)data.c_str(), data.length());
		sleep(2);
	}

	getchar();

	return 0;
}