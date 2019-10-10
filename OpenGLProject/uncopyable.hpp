
#pragma once

struct Uncopyable
{
	~Uncopyable() noexcept = default;
	constexpr Uncopyable() noexcept = default;
	Uncopyable(Uncopyable const&) = delete;
	Uncopyable& operator=(Uncopyable const&) = delete;
	Uncopyable(Uncopyable &&) = delete;
	Uncopyable& operator=(Uncopyable &&) = delete;
};
