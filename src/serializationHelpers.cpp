#include "mud.h"
#include "conf.h"
#include "serializationHelpers.h"


bool SerializeLong(TiXmlElement* parent, unsigned long long int val)
{
    if (parent == nullptr)
        {
            return false;
        }
    if (val == 0)
        {
            parent->SetAttribute("a", 0);
            parent->SetAttribute("b", 0);
            return true;
        }

    unsigned int a = 0;
    unsigned int b = 0;
    b = val;
    a = val >> 32;
    parent->SetAttribute("a", a);
    parent->SetAttribute("b", b);
    return true;
}
unsigned long long int DeserializeLong(TiXmlElement* parent)
{
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned long long int ret = 0;

    parent->Attribute("a", &a);
    parent->Attribute("b", &b);
    ret = a;
    ret <<=32;
    ret |= (0xFFFFFFFF&b);
    return ret;
}
