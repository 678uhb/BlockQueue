#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockQueue
{
public:
	void Push(T obj)
	{
		{
			std::unique_lock<std::mutex> ul(mtx);
			queue.push(obj);
		}
		cv.notify_all();
	}
	void Pop(T& dst)
	{
		std::unique_lock<std::mutex> ul(mtx);
		cv.wait(ul, [this]() {return !queue.empty(); });
		dst = std::move(queue.front());
		queue.pop();
	}
private:
	std::queue<T>				queue;
	std::mutex					mtx;
	std::condition_variable		cv;
};