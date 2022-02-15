#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>   

#include "ipcCore.h"
#include <iostream>

/*
 *  |   23      | 1 |  1000   |
 *  +++++++++... \0  +++++++...
 *  
 */

const int MAX_PROCESSNAME_LEN = 23;
const int MAX_MESSAGE_LEN = 1000 + 23 + 1;

CoreIPC::~CoreIPC()
{
	if (localsocket_ != -1)
	{
		close(localsocket_);
	}
	if (recvThread_.joinable())
	{
		recvThread_.join();
	}
	std::unordered_map<std::string, std::shared_ptr<IRemoteIPC>>::iterator it;
	for (it = remoteIpcMap_.begin(); it != remoteIpcMap_.end(); it++)
	{
		it->second = nullptr;
	}
	remoteIpcMap_.clear();
}

CoreIPC::CoreIPC()
{
	localsocket_ = -1;
	callback_ = nullptr;
}

int CoreIPC::startCore(std::string localname, MsgCallback callback)
{
	int len;
	struct sockaddr_un local;
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, localname.c_str());

	unlink(localname.c_str());
	localsocket_ = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (localsocket_ < 0)
	{
		return -1;
	}
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(localsocket_, (struct sockaddr*)&local, len) < 0)
	{
		return -1;
	}
	callback_ = callback;
	recvThread_ = std::thread(&CoreIPC::IPC_MsgRecvThread, this);
	return 0;
}

int CoreIPC::sendIPCMsg(std::string remoteProcessName, void *data, int size)
{
	char send_buf[1024];
	struct sockaddr_un remote;
	socklen_t len;
	int ret;

	memset(send_buf, 0, sizeof(send_buf));

	if (remoteProcessName.length() > MAX_PROCESSNAME_LEN)
	{
		return -1;
	}
	strcpy(send_buf, remoteProcessName.c_str());
	memcpy(send_buf + MAX_PROCESSNAME_LEN + 1, data, size);
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, remoteProcessName.c_str());
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	ret = sendto(localsocket_, send_buf, size + MAX_PROCESSNAME_LEN + 1, 0, (struct sockaddr*)&remote, len);
	return ret > 0 ? 0 : -1;
}

void CoreIPC::IPC_MsgRecvThread()
{
	int ret;
	socklen_t len;
	char recv_buf[1024];
	struct sockaddr_un remote;

	while (1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		ret = recvfrom(localsocket_, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&remote, &len);
		if (ret > 0)
		{
			std::string remoteProcessName = std::string(recv_buf, MAX_PROCESSNAME_LEN + 1);
			if (remoteIpcMap_.find(remoteProcessName) == remoteIpcMap_.end())
			{
				remoteIpcMap_.insert(std::make_pair(remoteProcessName, RemoteIPCDataFactory::CreateRemoteIPCData(remoteProcessName, callback_)));
			}
			char *data = recv_buf + MAX_PROCESSNAME_LEN + 1;
			remoteIpcMap_[remoteProcessName]->recvRemoteIPCMsg(data, ret - (MAX_PROCESSNAME_LEN + 1));
		}
		else
		{

		}
	}
}
