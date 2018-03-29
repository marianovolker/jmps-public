/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file log_tool.cpp
 * @author gparis
 * @date Tuesday, September 30, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <log_tool.h>

using namespace gvr::log;

DEFINE_LOG_MODULE(log6);
DEFINE_LOG_MODULE(log7);
DEFINE_LOG_MODULE(log8);

/**
 * @brief      Log Tool main entry point.
 *
 * This log tool is an external tool to perform a simple test over the log library libgvrlog.
 *
 * @param [in] argc Amount of arguments passed to the dummy tool command line.
 * @param [in] argv List of arguments passed to the dummy tool command line.
 * @return     Command line expected result code. Dummy Tool always return 0, which means ended successfully.
 *
 * @since 1.0.0
 */
int main (int argc, char* *argv)
{
  if( ! init("configuration/Log.ini", gvr::log::CNORMAL_USE_OF_LOG_LIBRARY) )
  {
    std::cerr << "Logger initialization failed. "
                 "Please check the existence of log directory or the configuration file/directory.\n"
                 "Test abnormally terminated.\n\n";
    return 1;
  }

  LOG_Info(log6, "Test tool for the gvrlog library.");
  LOG_VInfo(log6, "Copyright (c) %u Gilbarco Inc. Confidential and Proprietary.\n\n", 2014);

           long long ll1 = 1234000000000000LL;
           long long ll2 = 567890123456LL;
           long long ll3;
  unsigned long long ll4 = 0xFFFF000000000000ULL;
           long long ll5 = 0x0000FFFFFFFFFFFFLL;
  unsigned long long ll6;
  unsigned long long ll7 = 65535 * 16777216ULL * 16777216ULL;
  unsigned long long ll8 = 16777216 * 16777216ULL - 1;
  unsigned long long ll9;
  ll3 = ll1 + ll2;
  ll6 = ll4 + ll5;
  ll9 = ll7 + ll8;

  LOG_VInfo(log7,
      "char %c, short %d, short %u\n"
      "int %d, uint %u, long %d, ulong %u\n"
      "long long %lld, long ulong %llu\n"
      "long long %lld, long ulong %llX\n"
      "long long %lld, long ulong %llX\n"
      "double float %1.4f, long double float %1.12Lf\n\n",
      '*', -32768, 32767,
      -1234567890, 1234567890, 0xFFFFFFF0, 0xFFFFFFF0,
      -ll3, ll3, ll6, ll6, ll9, ll9,
       3.141592654, 3.14159265358979323846264338328L);

  LOG_VInfo(log7,
      "char %c, short %d, short %u\n"
      "int %d, uint %u, long %d, ulong %u\n"
      "long long %lld, long ulong %llu\n"
      "long long %lld, long ulong %llX\n"
      "long long %lld, long ulong %llX\n"
      "double float %1.4f, long double float %1.12Lf\n\n",
      '*', -32768, 32767,
      -1234567890, 1234567890, 0xFFFFFFF0, 0xFFFFFFF0,
      -ll3, ll3, ll6, ll6, ll9, ll9,
       3.141592654, 3.14159265358979323846264338328L);

  LOG_VInfo(log8,
      "char %c, short %d, short %u\n"
      "int %d, uint %u, long %d, ulong %u\n"
      "long long %lld, long ulong %llu\n"
      "long long %lld, long ulong %llX\n"
      "long long %lld, long ulong %llX\n"
      "double float %1.4f, long double float %1.12Lf\n\n",
      '*', -32768, 32767,
      -1234567890, 1234567890, 0xFFFFFFF0, 0xFFFFFFF0,
      -ll3, ll3, ll6, ll6, ll9, ll9,
       3.141592654, 3.14159265358979323846264338328L);

  LOG_VInfo(log8,
      "char %c, short %d, short %u\n"
      "int %d, uint %u, long %d, ulong %u\n"
      "long long %lld, long ulong %llu\n"
      "long long %lld, long ulong %llX\n"
      "long long %lld, long ulong %llX\n"
      "double float %1.4f, long double float %1.12Lf\n\n",
      '*', -32768, 32767,
      -1234567890, 1234567890, 0xFFFFFFF0, 0xFFFFFFF0,
      -ll3, ll3, ll6, ll6, ll9, ll9,
       3.141592654, 3.14159265358979323846264338328L);


  cleanUp();
  return 0;
}
