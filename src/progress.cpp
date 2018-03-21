#include "progress.h"

#include "utils/verify.h"


progress::progress() :
	m_max(),
	m_curr(),
	m_mutex(),
	m_cv(),
	m_thread(),
	m_exit_code()
{
}

progress::~progress()
{
	main_thread__wait_to_finish();
}

void progress::worker_thread__update_progress(std::uint32_t const& prgs)
{
	{
		std::lock_guard<std::mutex> lg{m_mutex};
		VERIFY(std::this_thread::get_id() == m_thread.get_id());
		VERIFY(prgs >= m_curr.load(std::memory_order_relaxed));
		m_curr.store(prgs, std::memory_order_relaxed);
	}
}

void progress::worker_thread__wakeup_main_thread()
{
	m_cv.notify_one();
}

void progress::worker_thread__set_exit_code(int const& exit_code)
{
	{
		std::lock_guard<std::mutex> lg{m_mutex};
		VERIFY(std::this_thread::get_id() == m_thread.get_id());
		m_exit_code.store(exit_code, std::memory_order_relaxed);
	}
}

void progress::main_thread__wait_to_finish()
{
	{
		std::lock_guard<std::mutex> lg{m_mutex};
		VERIFY(std::this_thread::get_id() != m_thread.get_id());
	}
	if(m_thread.joinable())
	{
		m_thread.join();
	}
}

int progress::main_thread__get_exit_code()
{
	int exit_code;
	{
		std::lock_guard<std::mutex> lg{m_mutex};
		VERIFY(std::this_thread::get_id() != m_thread.get_id());
		exit_code = m_exit_code.load(std::memory_order_relaxed);
	}
	return exit_code;
}
