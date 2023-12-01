#include <thread>
#include <list>
#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>

namespace tp {

template<uint16_t thread_count>
class thread_pool {
public:
	thread_pool() {
		m_threads.reserve(thread_count);

		for (uint16_t i = 0; i < thread_count; ++i) {
			m_threads.emplace_back(_thread, this);
		}
	}

	~thread_pool() {
		join();
	}

	void add_task(std::function<void(void)> task) {
		std::unique_lock lock {m_tasks_mutex};

		m_tasks.emplace_back(std::move(task));

		lock.unlock();
		m_semaphore.release();
	}

	void join() {
		static bool joined = false;

		if (joined) return;

		m_active = false;

		for (uint16_t i = 0; i < thread_count; ++i) {
			m_semaphore.release();
		}

		for (auto& thr : m_threads) {
			thr.join();
		}

		joined = true;
	}
	
private:
	bool tasks_available() {
		std::lock_guard guard {m_tasks_mutex};

		return m_tasks.size() > 0;
	}

	static void _thread(thread_pool* pt_ptr) {
		while(true) {
			pt_ptr->m_semaphore.acquire();

			std::function<void(void)> task;
			bool task_found = false;

			std::unique_lock tasks_lock(pt_ptr->m_tasks_mutex);

			if (pt_ptr->m_tasks.size() > 0) {
				task = pt_ptr->m_tasks.front();
				pt_ptr->m_tasks.pop_front();

				task_found = true;
			}
			else {
				if (!pt_ptr->m_active)
					break;
			}

			tasks_lock.unlock();

			if (task_found)
				task();
		}
	}

	std::vector<std::jthread> m_threads;
	std::list<std::function<void(void)>> m_tasks;

	std::counting_semaphore<std::numeric_limits<uint16_t>::max()> m_semaphore {0};

	std::mutex m_tasks_mutex;

	volatile bool m_active = true;
};

}
