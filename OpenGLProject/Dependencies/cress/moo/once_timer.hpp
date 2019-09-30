/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/timers/once_timer.hpp
//
//	Summary		:	Data type with remaining time property, which decreases towards zero when updated with delta time.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	28/09/2019
*/

#pragma once

namespace cress
{
	namespace moo
	{
		namespace OnceTimerTags
		{
			struct Uninitialised {};
			constexpr Uninitialised const uninitialised{};
		}

		/*
		//	Data type with remaining time property, which decreases towards zero when updated with delta time.
		//
		//	Example:

			float const length = 0.5f;
			cress::timers::OnceTimer<float> timer{ length };
			for (int frame = 0; frame < 1000; ++frame)
			{
				float const deltaTime = 1.0f / 60.0f;
				timer.updateUnclamped(deltaTime);
				if (timer.isFinished())
				{
					std::cout << "Timer has finished once on frame " << frame << ".\n";
					timer.setRemaining(length);
				}
			}

		*/
		template<class Duration_>
		class OnceTimer
		{
		private:
			Duration_ m_remaining;

		public:
			using This = OnceTimer;
			using Duration = Duration_;

			~OnceTimer() noexcept = default;
			constexpr OnceTimer(OnceTimer&&) noexcept = default;
			constexpr OnceTimer& operator=(OnceTimer&&) noexcept = default;
			constexpr OnceTimer(OnceTimer const&) = default;
			constexpr OnceTimer& operator=(OnceTimer const&) = default;
			constexpr OnceTimer() : m_remaining() {}
			constexpr OnceTimer(OnceTimerTags::Uninitialised) noexcept {}
			explicit constexpr OnceTimer(Duration_&& remaining) noexcept : m_remaining(static_cast<Duration_&&>(remaining)) {}
			explicit constexpr OnceTimer(Duration_ const& remaining) : m_remaining(remaining) {}

			constexpr Duration_ const& getRemaining() const noexcept { return m_remaining; }
			constexpr void setRemaining(Duration_ const& remaining) { m_remaining = remaining; }
			constexpr void setRemaining(Duration_&& remaining) { m_remaining = static_cast<Duration_&&>(remaining); }

			/*
			//	True if remaining time <= zero.
			*/
			constexpr bool isFinished() const { return m_remaining <= Duration_(); }

			/*
			//	Decreases remaining time by delta time.
			//	Remaining time may drop below zero.
			//	Faster than updateClamped().
			*/
			constexpr void updateUnclamped(Duration_ const& deltaTime)
			{
				m_remaining -= deltaTime;
			}

			/*
			//	Decreases remaining time by delta time.
			//	Remaining time may drop below zero.
			//	Faster than updateClamped().
			//	Returns isFinished().
			*/
			constexpr bool updateUnclampedFinished(Duration_ const& deltaTime)
			{
				updateUnclamped(deltaTime);
				return isFinished();
			}

			/*
			//	Decreases remaining time by delta time.
			//	Remaining time will be clamped to zero.
			*/
			constexpr void updateClamped(Duration_ const& deltaTime)
			{
				updateUnclamped(deltaTime);

				Duration_ zero = Duration_();
				if (m_remaining < zero)
				{
					m_remaining = static_cast<Duration_&&>(zero);
				}
			}

			/*
			//	Decreases remaining time by delta time.
			//	Remaining time will be clamped to zero.
			//	Returns isFinished().
			*/
			constexpr bool updateClampedFinished(Duration_ const& deltaTime)
			{
				updateClamped(deltaTime);
				return isFinished();
			}
		};
	}
}
