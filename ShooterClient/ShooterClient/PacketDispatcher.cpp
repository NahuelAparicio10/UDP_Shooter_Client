#include "PacketDispatcher.h"

PacketDispatcher::PacketDispatcher() : _running(false)
{
}

PacketDispatcher::~PacketDispatcher()
{
	Stop();
}

void PacketDispatcher::EnqueuePacket(const RawPacketJob& job)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(job);
}

void PacketDispatcher::RegisterHandler(PacketType type, std::function<void(const RawPacketJob&)> handler) {	_handlers[type] = handler; }

void PacketDispatcher::Start()
{
	_running = true;
	_dispatchThread = std::thread(&PacketDispatcher::DispatchLoop, this);
}

void PacketDispatcher::Stop()
{
	_running = false;
	if (_dispatchThread.joinable()) _dispatchThread.join();
}

void PacketDispatcher::DispatchLoop()
{
    while (_running) 
    {
        RawPacketJob job;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty()) continue;
            job = _queue.front();
            _queue.pop();
        }

        auto it = _handlers.find(job.type);
        if (it != _handlers.end()) 
        {
            it->second(job);
        }
    }
}
