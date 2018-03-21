#pragma once
#ifndef progress_inl_included
#define progress_inl_included


#include "progress.h"


template<typename F, typename... ArgsT>
void start_progress(progress& prgs, std::uint32_t const& max_steps, F const& f, ArgsT const&... args)
{
//	prgs.m_max = max_steps;
//	prgs.m_thread = std::thread{f, args...};
}


#endif
