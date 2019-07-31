#ifdef GOOGLE_MOCK

#ifndef CONFIGURATION_TEST_H
#define CONFIGURATION_TEST_H

#include <gtest/gtest.h>
#include "../../configuration.h"

class ConfigurationTest : public testing::Test{
public:
  ConfigurationTest():
      m_config("../../CAST.txt")
  {}

  Config m_config;
};

#endif // CONFIGURATION_TEST_H

#endif // GOOGLE_MOCK
