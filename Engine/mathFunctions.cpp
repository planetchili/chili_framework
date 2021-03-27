#include "mathFunctions.h"
#include<math.h>
#include<utility>
#include<algorithm>

float normalize(float val, float min, float max)
{
// Shift to positive to avoid issues when crossing the 0 line
if (min < 0) {
    max += 0 - min;
    val += 0 - min;
    min = 0;
}
// Shift values from 0 - max
val = val - min;
max = max - min;
return std::max(0.0f, std::min(1.0f, val / max));
}


float interpolateTo(float a, float b, float coefficient)
{
    return a + coefficient * (b - a);
}