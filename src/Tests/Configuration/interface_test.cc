#ifdef GOOGLE_MOCK

#include "configuration_test.h"

TEST_F(ConfigurationTest, verbosity) {
  EXPECT_EQ(m_config.m_options.general.verbosity, 3u);
}

TEST_F(ConfigurationTest, interface) {
  EXPECT_EQ(m_config.m_options.general.energy_interface, config::interface_types::OPLSAA);
}

TEST_F(ConfigurationTest, energy_QMMM_qmatoms) {
  // Apparently the atom index in the config file are decremented
  std::vector<std::size_t> qmatoms{5, 8, 9, 10, 11, 12, 13, 14};
  EXPECT_EQ(m_config.m_options.energy.qmmm.qmatoms, qmatoms);
}

TEST_F(ConfigurationTest, energy_PSI4_path) {
  EXPECT_EQ(m_config.m_options.energy.psi4.path, "/apps/psi4/psi4");
}

TEST_F(ConfigurationTest, energy_PSI4_memory) {
  EXPECT_EQ(m_config.m_options.energy.psi4.memory, "4GB");
}

TEST_F(ConfigurationTest, energy_PSI4_basis) {
  EXPECT_EQ(m_config.m_options.energy.psi4.basis, "6-31G");
}

TEST_F(ConfigurationTest, energy_PSI4_method) {
  EXPECT_EQ(m_config.m_options.energy.psi4.method, "HF");
}

TEST_F(ConfigurationTest, energy_PSI4_spin) {
  EXPECT_EQ(std::stoi(m_config.m_options.energy.psi4.spin), 1);
}

TEST_F(ConfigurationTest, energy_PSI4_charge) {
  EXPECT_EQ(std::stoi(m_config.m_options.energy.psi4.charge), 0);
}

TEST_F(ConfigurationTest, energy_PSI4_threads) {
  EXPECT_EQ(std::stoi(m_config.m_options.energy.psi4.threads), 4);
}

#endif // GOOGLE_MOCK