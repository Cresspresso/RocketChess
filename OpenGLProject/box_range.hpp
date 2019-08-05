
#pragma once

#include "common.hpp"
#include "math_utils.hpp"

struct BoxRange
{
	vec3 min = vec3();
	vec3 max = vec3(1);

	vec3 clamp(vec3 const& v);
	bool contains(vec3 const& v);
};
