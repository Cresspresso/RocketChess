
#include <mutex>

#include "compatibility.hpp"

using lock_guard = std::lock_guard<std::mutex>;
using unique_lock = std::unique_lock<std::mutex>;
