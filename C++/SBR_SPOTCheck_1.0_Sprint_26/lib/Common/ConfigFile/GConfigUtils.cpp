#include "GConfigUtils.h"
#include "Common/Util/DEBUGOUT.h"
#include <algorithm>




void GConfigUtils::StringSplit(std::string Value, char s8Delim, std::list<std::string>& Results)
{
   std::string TempValue = Value;

   while(TempValue.length() > 0)
   {
      unsigned int u32Pos = TempValue.find(s8Delim);
      unsigned int uMAX = unsigned(std::string::npos);

      if(u32Pos == uMAX)
      {
         Results.push_back(TempValue);
         TempValue.clear();
      }
      else if(u32Pos == 0)
      {
         TempValue = TempValue.substr(1);
      }
      else
      {
         Results.push_back(TempValue.substr(0, u32Pos));
         TempValue = TempValue.substr(u32Pos+1);
      }
   }
}



std::string GConfigUtils::CombinedString(std::list<std::string> Parts, char s8Delim)
{
   std::string ReturnVal;

   for(std::list<std::string>::iterator Itr = Parts.begin();Itr != Parts.end();++Itr)
   {
      ReturnVal += s8Delim + *Itr;
   }

   return ReturnVal;
}



std::string GConfigUtils::Trim (std::string Value)
{
   return RTrim(LTrim(Value));
}



std::string GConfigUtils::LTrim(std::string Value)
{
   std::string Return;

   unsigned int nLength;
                              
   for (nLength=0;nLength<Value.length();nLength++)
   {
      if(Value[nLength] != ' ' && Value[nLength] != '\t' && Value[nLength] != '\r' && Value[nLength] != '\n')
      {
         break;
      }
   }

   Return = Value.substr(nLength);

   return Return;
}



std::string GConfigUtils::RTrim(std::string Value)
{
   unsigned int nLength = Value.size();       
                              
   while( nLength-- >= 0 )
   {
      if(Value[nLength] != ' ' && Value[nLength] != '\t' && Value[nLength] != '\r' && Value[nLength] != '\n')
      {
         break;                   
      }
   }
   Value.resize(++nLength);

   return Value;
}


int GConfigUtils::char2upper(int c)
{
	if((c >= 'a') && (c <= 'z'))
	{
		return (c - ('a' - 'A'));
	}
	return (c);
}



void GConfigUtils::SFReplace(std::string& TheString, std::string Old, std::string New)
{
   unsigned int uPos = TheString.find(Old);
   unsigned int uMAX = unsigned(std::string::npos);

   if(uPos == uMAX)
   {
      LOG(eCommon, eError,
          "GConfigUtils::SFReplace() - %s not found in %s\n",Old.c_str(),TheString.c_str());
      return;
   }

   TheString.replace(uPos, Old.length(), New);
}



void GConfigUtils::IFReplace(std::string& TheString, std::string Old, std::string New)
{
   std::string TempTheString = TheString;

   std::transform(TempTheString.begin(), TempTheString.end(), TempTheString.begin(), char2upper);
   std::transform(Old.begin(), Old.end(), Old.begin(), char2upper);

   unsigned int uPos = TempTheString.find(Old);
   unsigned int uMAX = unsigned(std::string::npos);

   if(uPos == uMAX)
   {
      LOG(eCommon, eError,
          "GConfigUtils::IFReplace() - %s not found in %s\n",Old.c_str(),TheString.c_str());
      return;
   }

   TheString.replace(uPos, Old.length(), New);
}



void GConfigUtils::SRReplace(std::string& TheString, std::string Old, std::string New)
{
   unsigned int uPos = TheString.rfind(Old);
   unsigned int uMAX = unsigned(std::string::npos);

   if(uPos == uMAX)
   {
      LOG(eUnCategorized, eError,
          "GConfigUtils::SRReplace() - %s not found in %s\n",Old.c_str(),TheString.c_str());
      return;
   }

   TheString.replace(uPos, Old.length(), New);
}



void GConfigUtils::IRReplace(std::string& TheString, std::string Old, std::string New)
{
   std::string TempTheString = TheString;

   std::transform(TempTheString.begin(), TempTheString.end(), TempTheString.begin(), char2upper);
   std::transform(Old.begin(), Old.end(), Old.begin(), char2upper);

   unsigned int uPos = TempTheString.rfind(Old);
   unsigned int uMAX = unsigned(std::string::npos);

   if(uPos == uMAX)
   {
      LOG(eConfig, eError,
          "GConfigUtils::IRReplace() - %s not found in %s\n",Old.c_str(),TheString.c_str());
      return;
   }

   TheString.replace(uPos, Old.length(), New);
}

