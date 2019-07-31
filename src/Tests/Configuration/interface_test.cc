#ifdef GOOGLE_MOCK

#include "configuration_test.h"

TEST_F(ConfigurationTest, verbosity) {
  EXPECT_EQ(m_config.m_options.general.verbosity, 3u);
}

TEST_F(ConfigurationTest, interface) {
  EXPECT_EQ(m_config.m_options.general.energy_interface, config::interface_types::OPLSAA);
}

TEST_F(ConfigurationTest, energy_paramfile) {
  EXPECT_EQ(m_config.m_options.general.paramFilename, "oplsaa_mod2.prm");
}

TEST_F(ConfigurationTest, energy_QMMM_qmatoms) {
  // Apparently the atom index in the config file are decremented
  std::vector<std::size_t> qmatoms{5, 8, 9, 10, 11, 12, 13, 14};
  EXPECT_EQ(m_config.m_options.energy.qmmm.qmatoms, qmatoms);
}

TEST_F(ConfigurationTest, energy_QMMM_mminterface) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.mminterface, config::interface_types::OPLSAA);
}

TEST_F(ConfigurationTest, energy_QMMM_qminterface) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.qminterface, config::interface_types::ORCA);
}

TEST_F(ConfigurationTest, energy_QMMM_qmtofile) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.qm_to_file, true);
}

TEST_F(ConfigurationTest, energy_QMMM_linkatomtype) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.linkatom_types, std::vector<int>{85});
}

TEST_F(ConfigurationTest, energy_QMMM_zerochargebonds) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.zerocharge_bonds, 1);
}

TEST_F(ConfigurationTest, energy_QMMM_seatoms) {
  std::vector<std::size_t> seatoms{2, 5, 6, 7, 9, 10};
  EXPECT_EQ(m_config.m_options.energy.qmmm.seatoms, seatoms);
}

TEST_F(ConfigurationTest, energy_QMMM_seinterface) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.seinterface, config::interface_types::DFTB);
}

TEST_F(ConfigurationTest, energy_QMMM_smallcharges) {
  EXPECT_EQ(m_config.m_options.energy.qmmm.emb_small, 1);
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