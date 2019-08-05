// warning free #include <stdio.h>

#pragma once

// header include
#pragma warning(push)
#pragma warning(disable: 4514)
#include <stdio.h>  // NOLINT(modernize-deprecated-headers, hicpp-deprecated-headers)
#pragma warning(pop)
