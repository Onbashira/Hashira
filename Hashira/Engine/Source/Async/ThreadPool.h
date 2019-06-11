#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future> 
#include <functional>
#include <stdexcept>



namespace Hashira {
	class ThreadPool {

	public:

	private:
		//!スレッド
		std::vector< std::thread > _workers;

		//!タスク
		std::queue< std::function<void()> > _tasks;

		//!ミューテックス
		std::mutex _queueMutex;

		//!状態変数
		std::condition_variable _condition;

		//!停止フラグ
		bool _stop;
	
	public:

		/**
		 * @fn
		 * @brief コンストラクタ
		 * @param[in] threads 
		 */
		ThreadPool(size_t threads);

		~ThreadPool();

		/**
		* @fn
		* @brief キューにタスクを投げる。
		* @param[in] f 関数オブジェクト
		* @param[in] args 関数オブジェクトが必要とする引数
		 */
		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
			->std::future<typename std::result_of<F(Args...)>::type>;
		
		/**
		 * @fn
		 * @brief ワーカーの破棄
		 */
		void DiscardWorkers();


	private:

	};
}

inline Hashira::ThreadPool::ThreadPool(size_t threads)
	: _stop(false)
{
	for (size_t i = 0; i < threads; ++i)
		_workers.emplace_back([this]
	{
		for (;;)
		{
			std::function<void()> task;

			{
				std::unique_lock<std::mutex> lock(this->_queueMutex);
				this->_condition.wait(lock,
					[this] { return this->_stop || !this->_tasks.empty(); });
				if (this->_stop && this->_tasks.empty())
					return;
				task = std::move(this->_tasks.front());
				this->_tasks.pop();
			}

			task();
		}
	}
	);
}

inline Hashira::ThreadPool::~ThreadPool()
{
	DiscardWorkers();
}

template<class F, class... Args>
auto Hashira::ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(_queueMutex);

		if (_stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		_tasks.emplace([task]() { (*task)(); });
	}
	_condition.notify_one();
	return res;
}

inline void Hashira::ThreadPool::DiscardWorkers()
{
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		_stop = true;
	}
	_condition.notify_all();
	for (std::thread &worker : _workers) {
		if (worker.joinable()) {
			worker.join();
		}
	}
}




