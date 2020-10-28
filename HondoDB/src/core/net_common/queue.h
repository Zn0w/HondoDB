/*
	NOTE : a thread-safe queue for collecting net messages
*/

#pragma once

#include <mutex>
#include <deque>

#include "message.h"


namespace hondo { namespace net {

template <typename T>
class Queue
{
	std::mutex mux_queue;
	std::deque<T> de_queue;


public:
	Queue() {}
	Queue(const Queue&) = delete;	// prohibit copying (because it has mutexes)
	~Queue() { clear(); }

	const T& front()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		return de_queue.front();
	}

	const T& back()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		return de_queue.back();
	}

	void push_back(const T& item)
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		de_queue.emplace_back(std::move(item));
	}

	void push_front(const T& item)
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		de_queue.emplace_front(std::move(item));
	}

	bool empty()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		return de_queue.empty();
	}

	size_t count()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		return de_queue.size();
	}

	void clear()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		return de_queue.clear();
	}

	T pop_front()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		auto t = std::move(de_queue.front());
		de_queue.pop_front();
		return t;
	}

	T pop_back()
	{
		std::scoped_lock<std::mutex> lock(mux_queue);
		auto t = std::move(de_queue.back());
		de_queue.pop_back();
		return t;
	}
};

} }