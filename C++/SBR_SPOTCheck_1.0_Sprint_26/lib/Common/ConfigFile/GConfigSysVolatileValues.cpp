// GConfigSysVolatileValues.h


#include "GConfigSysVolatileValues.h"


std::string GConfigSysVolatileValues::m_FileName;


void GConfigSysVolatileValues::SetExecutableName(std::string FileName)
{
   m_FileName = FileName;
}


std::string GConfigSysVolatileValues::GetExecutableName()
{
   return m_FileName;
}

