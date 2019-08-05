
#include "closing_state.hpp"

std::atomic<ClosingState> g_closing = ClosingState::Running;
