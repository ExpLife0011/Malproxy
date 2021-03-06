#pragma once
#include "AutoGenerated/malproxy.grpc.pb.h"
#include "Framework/Event.h"
#include "RpcCallbacks.h"
#include <thread>

class MalproxyServer final : public malproxy::MalproxyInterface::Service
{
public:
	enum State
	{
		INITIALIZED,
		STARTING,
		STARTED,
		SHUTTING_DOWN,
		SHUTDOWN,
		ERR
	};

	MalproxyServer(const RpcServerCallbacks& callbacks);
	virtual ~MalproxyServer();

	void Wait();
	void Shutdown();
	
	grpc::Status CallFunc(grpc::ServerContext* context, const malproxy::CallFuncRequest* request, malproxy::CallFuncResponse* response) override;
	grpc::Status LoadRemoteLibrary(grpc::ServerContext* context, const malproxy::LoadLibraryRequest* request, malproxy::LoadLibraryResponse* response) override;
	grpc::Status FreeRemoteLibrary(grpc::ServerContext* context, const malproxy::FreeLibraryRequest* request, malproxy::Empty* response) override;

private:
	RpcServerCallbacks _callbacks;
	std::unique_ptr<grpc::Server> _server;
	std::thread _server_thread;
	State _state = INITIALIZED;
	Event _server_done;
	Event _shutdown_done;
};


class Tag
{
public:
	Tag(std::function<void()> completion_routine, std::function<void()> error_routine = nullptr);
	virtual ~Tag() { }
	void *getTagId();
	void Go();
	void Error();

private:
	std::function<void()> _completion_routine;
	std::function<void()> _error_routine;
};