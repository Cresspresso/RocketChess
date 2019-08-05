
#include "world_math.hpp"

#include "box_range.hpp"

vec3 BoxRange::clamp(vec3 const& v)
{ 
	using ::clamp;
	vec3 r;
	r.x = clamp(v.x, min.x, max.x);
	r.y = clamp(v.y, min.y, max.y);
	r.z = clamp(v.z, min.z, max.z);
	return r;
}

bool BoxRange::contains(vec3 const& v)
{
	return isInsideBox(v, min, max);
}
