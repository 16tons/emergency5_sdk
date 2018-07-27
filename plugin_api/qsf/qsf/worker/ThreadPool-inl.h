//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template <typename RetType>
	ThreadPool<RetType>::ThreadPool(size_t threads)
	{
		if (threads == -1)
		{
			threads = std::thread::hardware_concurrency();
		}
		if (threads <= 0)
		{
			threads = 1;
		}
		this->threads = threads;
	}

	template <typename RetType>
	ThreadPool<RetType>::~ThreadPool()
	{
		// If thread is running then wait for it to complete
		if (thread.joinable())
		{
			thread.join();
		}
	}

	template <typename RetType>
	void ThreadPool<RetType>::queueTask(const Task &task)
	{
		tasks.push(task);
	}

	template <typename RetType>
	void ThreadPool<RetType>::queueTask(Task &&task)
	{
		tasks.emplace(std::move(task));
	}

	template <typename RetType>
	void ThreadPool<RetType>::process(Callback callback)
	{
		if (callback)
		{
			thread = std::thread([&]{
				_process();
				callback();
			});
		}
		else
		{
			_process();
		}
	}

	template <typename RetType>
	void ThreadPool<RetType>::_process()
	{
		std::lock_guard<std::mutex> lock(processMutex);
		futuresDone.clear();

		while (!tasks.empty())
		{
			const size_t taskAmount = tasks.size();
			const size_t amount = (threads > taskAmount ? taskAmount : threads);

			for (size_t i = 0; i < amount; ++i)
			{
				auto task = tasks.front();
				tasks.pop();

				auto future = std::async(std::launch::async, [=] {
					return task();
					});
				futuresPending.emplace_back(std::move(future));
			}

			for (auto &future : futuresPending)
			{
				future.wait();
				futuresDone.emplace_back(std::move(future));
			}

			futuresPending.clear();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
