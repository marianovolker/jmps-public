// GConfigSysVolatileValues.h

#ifndef GConfigSysVolatileValues_h
#define GConfigSysVolatileValues_h

#include <string>

class GConfigSysVolatileValues
{
public:
   static void SetExecutableName(std::string FileName);
   static std::string GetExecutableName();

private:
   static std::string m_FileName;
};

#endif
