#ifdef GOOGLE_MOCK

#include "configuration_test.h"

// We cannot statically initialize m_config with the actual configuration
// from ../../CAST.txt since the parsing relies on other static variables
// which may be not yet initialized.
Config ConfigurationTest::m_config;

#endif // GOOGLE_MOCK