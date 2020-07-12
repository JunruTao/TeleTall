#ifndef JUTA_MATH_H
#define JUTA_MATH_H

namespace JUTA_Math
{
    template <class var>
    void Clamp(var &value, var min_bound, var max_bound)
    {
        if (value <= min_bound)
        {
            value = min_bound;
        }
        else if (value >= max_bound)
        {
            value = max_bound;
        }
    }
} // namespace JUTA_Math

#endif