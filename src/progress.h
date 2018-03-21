#pragma once
#ifndef progress_h_included
#define progress_h_included


#include "Utils.h"

#include <cstdint>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <mutex>


class progress
{
public:
	progress();
	progress(progress const&) = delete;
	progress(progress&&) noexcept = default;
	progress& operator=(progress const&) = delete;
	progress& operator=(progress&&) noexcept = default;
	~progress();
public:
	void worker_thread__update_progress(std::uint32_t const& prgs);
	void worker_thread__wakeup_main_thread();
	void worker_thread__set_exit_code(int const& exit_code);
public:
	void main_thread__wait_to_finish();
	int main_thread__get_exit_code();
public:
	std::uint32_t m_max;
	std::atomic<std::uint32_t> m_curr;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::thread m_thread;
	std::atomic<int> m_exit_code;
};

template<typename F, typename... ArgsT>
void start_progress(progress& prgs, std::uint32_t const& max_steps, F const& f, ArgsT const&... args);


#include "progress.inl"


#endif
