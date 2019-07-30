#ifdef GOOGLE_MOCK
#include <gtest/gtest.h>

#include "../../configuration.h"

TEST(Configuration, PSI4_path) {
  EXPECT_EQ(Config::get().energy.psi4.path, "/apps/psi4/psi4");
}

TEST(Configuration, PSI4_memory) {
  EXPECT_EQ(Config::get().energy.psi4.memory, "4GB");
}

TEST(Configuration, PSI4_basis) {
  EXPECT_EQ(Config::get().energy.psi4.basis, "6-31G");
}

TEST(Configuration, PSI4_method) {
  EXPECT_EQ(Config::get().energy.psi4.method, "HF");
}

TEST(Configuration, PSI4_spin) {
  EXPECT_EQ(std::stoi(Config::get().energy.psi4.spin), 1);
}

TEST(Configuration, PSI4_charge) {
  EXPECT_EQ(std::stoi(Config::get().energy.psi4.charge), 0);
}

TEST(Configuration, PSI4_threads) {
  EXPECT_EQ(std::stoi(Config::get().energy.psi4.threads), 4);
}

#endif // GOOGLE_MOCK