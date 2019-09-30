/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/timers/timer.hpp
//
//	Summary		:	Data type with elapsed time property, which increases towards a length property when updated with delta time.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	28/09/2019
*/

#pragma once

#include <cmath>
#include <type_traits>

namespace cress
{
	namespace moo
	{
		namespace TimerTags
		{
			struct Uninitialised {};
			constexpr Uninitialised const uninitialised{};
		}

		struct TimerIntegralTraits
		{
			template<class Duration>
			static constexpr Duration mod(Duration const& n, Duration const& d)
			{
				return n % d;
			}

			template<class Count, class ModResult>
			static constexpr Count convertCount(ModResult const& v)
			{
				return static_cast<Count>(v);
			}
		};

		struct TimerFloatingTraits : TimerIntegralTraits
		{
			template<class Duration>
			static constexpr Duration mod(Duration n, Duration d)
			{
				return std::fmod(n, d);
			}
		};

		/*
		//	Data type with elapsed time property, which increases towards a length property when updated with delta time.
		//
		//	Example:

			cress::timers::Timer<float> timer{ 0.5f };
			for (int frame = 0; frame < 1000; ++frame)
			{
				float const deltaTime = 1.0f / 60.0f;
				int const n = timer.updateRepeating(deltaTime);
				if (n > 0)
				{
					std::cout << "Timer has finished " << n << " times on frame " << frame << ".\n";
				}
			}

		*/
		template<class Duration_, class Count_ = int, class Traits_ = std::conditional_t<
			std::disjunction_v<std::is_same<Duration_, float>, std::is_same<Duration_, double>, std::is_same<Duration_, long double>>,
			TimerFloatingTraits,
			TimerIntegralTraits>
		>
			class Timer
		{
		private:
			Duration_ m_length;
			Duration_ m_elapsed;

		public:
			using This = Timer;
			using Duration = Duration_;
			using Count = Count_;
			using Traits = Traits_;

			~Timer() noexcept = default;
			constexpr Timer(Timer&&) noexcept = default;
			constexpr Timer& operator=(Timer&&) noexcept = default;
			constexpr Timer(Timer const&) = default;
			constexpr Timer& operator=(Timer const&) = default;
			Timer() = delete;
			Timer(TimerTags::Uninitialised) noexcept {}

			explicit constexpr Timer(Duration_&& length, Duration_&& elapsed) noexcept
				: m_length(static_cast<Duration_&&>(length))
				, m_elapsed(static_cast<Duration_&&>(elapsed))
			{}

			explicit constexpr Timer(Duration_ const& length, Duration_ const& elapsed)
				: m_length(length)
				, m_elapsed(elapsed)
			{}

			explicit constexpr Timer(Duration_ const& length, Duration_&& elapsed)
				: m_length(length)
				, m_elapsed(static_cast<Duration_&&>(elapsed))
			{}

			explicit constexpr Timer(Duration_&& length, Duration_ const& elapsed)
				: m_length(static_cast<Duration_&&>(length))
				, m_elapsed(elapsed)
			{}

			explicit constexpr Timer(Duration_ const& length)
				: m_length(length)
				, m_elapsed()
			{}

			explicit constexpr Timer(Duration_&& length)
				: m_length(static_cast<Duration_&&>(length))
				, m_elapsed()
			{}


			constexpr Duration_ const& getLength() const noexcept { return m_length; }

			constexpr Duration_ const& getElapsed() const noexcept { return m_elapsed; }
			constexpr void setElapsed(Duration_ const& elapsed) { m_elapsed = elapsed; }
			constexpr void setElapsed(Duration_&& elapsed) { m_elapsed = static_cast<Duration_&&>(elapsed); }
			constexpr void resetElapsed() { m_elapsed = Duration_(); }

			constexpr Duration_ getRemaining() const { return m_length - m_elapsed; }
			constexpr void setRemaining(Duration_ const& remaining) { m_elapsed = m_length - remaining; }
			constexpr void setRemaining(Duration_&& remaining) { m_elapsed = m_length - static_cast<Duration_&&>(remaining); }


			/*
			//	Increases elapsed time by delta time, repeating from zero when it reaches length.
			//	Returns the number of times the timer repeated.
			*/
			constexpr Count_ updateRepeating(Duration_ const& deltaTime)
			{
				Duration_ const x = m_elapsed + deltaTime;
				if (x >= m_length)
				{
					m_elapsed = Traits_::mod(x, m_length);
					return Traits_::template convertCount<Count_>(x / m_length);
				}
				else
				{
					m_elapsed = x;
					return Count_();
				}
			}

			/*
			//	Returns true if elapsed time has reached length and was clamped to length.
			*/
			constexpr bool updateClampedFinished(Duration_ const& deltaTime)
			{
				m_elapsed += deltaTime;
				if (m_elapsed >= m_length)
				{
					m_elapsed = m_length;
					return true;
				}
				else
				{
					return false;
				}
			}

			/*
			//	Returns true if elapsed time has reached length and was reset to zero.
			*/
			constexpr bool updateResetFinished(Duration_ const& deltaTime)
			{
				m_elapsed += deltaTime;
				if (m_elapsed >= m_length)
				{
					m_elapsed = Duration_();
					return true;
				}
				else
				{
					return false;
				}
			}
		};
	}
}
