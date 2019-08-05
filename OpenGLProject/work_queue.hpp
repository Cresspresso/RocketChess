/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	work_queue.hpp
**
**	Summary		:	Thread-safe queue.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <optional>

#include "compatibility_threading.hpp"



//namespace cress::project
//{
	template<class T>
	class WorkQueue
	{
	public:



		/*
		 *	Insert an item at the back of the queue and signal any thread that might be
		 *	waiting for the queue to be populated.
		 */
		void push(T&& item)
		{
			lock_guard lock{ mutex };
			queue.push(std::forward<T>(item));
			++size_;
			cond.notify_one();
		}



		/*
		 *	Insert an item at the back of the queue and signal any thread that might be
		 *	waiting for the queue to be populated.
		 */
		void push(T const& item)
		{
			lock_guard lock{ mutex };
			queue.push(item);
			++size_;
			cond.notify_one();
		}



		/*
		 *	Retrieves the front item from the queue and pops it off as well.
		 *	Returns (std::nullopt) if the queue is empty.
		 */
		std::optional<T> tryPop()
		{
			unique_lock lock{ mutex };
			if (queue.empty())
			{
				return std::nullopt;
			}
			T item = std::move(queue.front());
			queue.pop();
			--size_;
			lock.unlock();
			return std::optional<T>{ std::move(item) };
		}



		/*
		 *	Retrieves the front item from the queue and pops it off as well.
		 *	If the queue is empty block the thread from running until an item becomes available.
		 */
		T waitPop()
		{
			unique_lock lock{ mutex };
			cond.wait(lock, [this] { return !queue.empty(); });
			T item = std::move(queue.front());
			queue.pop();
			--size_;
			lock.unlock();
			return item;
		}



		/*
		 *	Returns the number of items in the queue.
		 */
		std::atomic_size_t const& size() const noexcept { return size_; }

		/*
		 *	Returns true if the queue is empty.
		 */
		bool empty() const noexcept { return size_ == 0; }



	private:



		std::atomic_size_t size_ = 0;
		std::queue<T> queue{};
		std::mutex mutex{};
		std::condition_variable cond{};



	};
	//~ class WorkQueue
//}
////~ namespace cress::project
