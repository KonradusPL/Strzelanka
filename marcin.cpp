#include "marcin.h"

float sinus(int kat)
{
    return sin((kat * M_PI) / 180.f);
}
float cosinus(int kat)
{
    return cos((kat * M_PI) / 180.f);
}
string toString(int l)
{
    ostringstream ss;
    ss << l;
    return ss.str();
}
