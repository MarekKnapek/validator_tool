#pragma once


#include "Utils.h"
#include "progress.h"
#include "utils/verify.h"

#include <cwchar>
#include <memory>
#include <chrono>


using namespace std::chrono_literals;


int print_progress(std::unique_ptr<progress> const& prgs)
{
	VERIFY(prgs);

	std::uint32_t curr;
	{
		std::lock_guard<std::mutex> lg{prgs->m_mutex};
		curr = prgs->m_curr.load(std::memory_order_relaxed);
	}
	std::uint32_t const max_steps = prgs->m_max;
	double per_cent = static_cast<double>(curr) / static_cast<double>(max_steps) * 100.0;
	std::wprintf(L"Progress is: %6.2f %%.\n", per_cent);

	for(;;)
	{
		{
			std::unique_lock<std::mutex> lck{prgs->m_mutex};
			// Sleep for 200ms. Update 5x per second.
			// Worker thread might wakeup us sooner.
			prgs->m_cv.wait_for(lck, 200ms);
			curr = prgs->m_curr.load(std::memory_order_relaxed);
		}
		if(curr == max_steps)
		{
			// Job finished. Wait for thread run-time stuff (C++ library, C library, operating system) to finish.
			prgs->main_thread__wait_to_finish();
			std::wprintf(L"%s", L"Progress is: 100.00 %.\n");
			return prgs->main_thread__get_exit_code();
		}
		else
		{
			double const per_cent_new = static_cast<double>(curr) / static_cast<double>(max_steps) * 100.0;
			if(per_cent_new >= per_cent + 0.01)
			{
				std::wprintf(L"Progress is: %6.2f %%.\n", per_cent_new);
				per_cent = per_cent_new;
			}
			else
			{
				// Nothing new to show.
				// Possibly cased by spurious wakeup.
				// Or sleep interval is too short. (Waking up too often per second).
				// Or amount of work after which worker thread wakes us up is too small. (Wakeup call after each byte is processed?)
			}
		}
	}
}
