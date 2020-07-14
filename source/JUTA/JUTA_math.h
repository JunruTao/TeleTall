#ifndef JUTA_MATH_H
#define JUTA_MATH_H

#include <sstream>


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

    template <typename var>
    std::string Num_To_String_Percision(const var value, const int n = 4)//n is size
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << value;
        return out.str();
    }

} // namespace JUTA_Math

#endif