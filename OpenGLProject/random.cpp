
#include "random.hpp"

std::default_random_engine g_rng{ std::random_device{}() };
