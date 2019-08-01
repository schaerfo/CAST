#ifdef GOOGLE_MOCK

#ifndef CONFIGURATION_TEST_H
#define CONFIGURATION_TEST_H

#include <gtest/gtest.h>
#include "../../configuration.h"

class ConfigurationTest : public testing::Test{
protected:
  static void SetUpTestCase() {
    m_config = Config("../../CAST.txt");
  }

  static Config m_config;
};

#endif // CONFIGURATION_TEST_H

#endif // GOOGLE_MOCK
