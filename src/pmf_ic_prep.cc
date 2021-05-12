#include "pmf_ic_prep.h"

#include "gpr.h"

#ifdef _OPENMP
 #include <mutex>
 #include <omp.h>
#endif

pmf_ic_prep::pmf_ic_prep(coords::Coordinates const& c, coords::input::format& ci, std::string const& outfile, std::string const& splinefile) :
  coord_objects(1), coord_input(&ci), outfilename(outfile), splinefilename(splinefile), dimension(Config::get().coords.umbrella.pmf_ic.indices_xi.size())
{
#ifdef _OPENMP
  coord_objects.resize(omp_get_max_threads());
#endif
  for (auto& curr_coord_obj: coord_objects)
    curr_coord_obj = c;
}

void pmf_ic_prep::run()
{
  calc_xis_zs_and_E_HLs();
  calc_E_LLs();
  calc_deltaEs();
  write_to_file();
  if (dimension == 1) write_spline_1d();
  else write_spline_2d(); 
}

void pmf_ic_prep::calc_xis_zs_and_E_HLs()
{
  auto& pes_points = coord_input->PES();
  E_HLs.resize(pes_points.size());

  if (dimension == 1) {
    xis.resize(pes_points.size());
    zs.resize(pes_points.size());
  }
  if (dimension > 1) {
    xi_2d.resize(pes_points.size());
    z_2d.resize(pes_points.size());
  }

#ifdef _OPENMP
  std::mutex print_mutex;

  #pragma omp parallel for default(none) shared(pes_points, print_mutex, std::cout)
#endif
  for (std::size_t i=0; i<pes_points.size(); ++i)   // for every structure
  {
    auto& pes = pes_points[i];
    auto& coordobj = coord_objects[omp_get_thread_num()];
    coordobj.set_xyz(pes.structure.cartesian, true);

    // calulate xi and z
    auto xi = coords::bias::Potentials::calc_xi(coordobj.xyz(), Config::get().coords.umbrella.pmf_ic.indices_xi[0]);
    auto z = mapping::xi_to_z(xi, Config::get().coords.umbrella.pmf_ic.xi0[0], Config::get().coords.umbrella.pmf_ic.L[0]);
    double xi_2;

    if (dimension == 1)  // in case of 1D
    {
      xis[i] = xi;
      zs[i] = z;
    }
    if (dimension > 1)    // in case of 2D
    {
      xi_2 = coords::bias::Potentials::calc_xi(coordobj.xyz(), Config::get().coords.umbrella.pmf_ic.indices_xi[1]);
      xi_2d[i] = std::make_pair( xi, xi_2 );
      auto z2 = mapping::xi_to_z(xi_2, Config::get().coords.umbrella.pmf_ic.xi0[1], Config::get().coords.umbrella.pmf_ic.L[1]);
      z_2d[i] = std::make_pair(z, z2);
    }

    // calculate high level energy
    auto E = coordobj.e();
    E_HLs[i] = E;
    if (Config::get().general.verbosity > 3)
    {
#ifdef _OPENMP
      std::lock_guard l(print_mutex);
#endif
      std::cout << i << " , " << xi << " , ";
      if (dimension > 1) std::cout << xi_2 << " , ";
      // Print the energy with higher precision and reset afterwards
      auto prec = std::cout.precision();
      std::cout << std::setprecision(10) << E << std::setprecision(prec) << ' ' << &coordobj << "\n";
    }
  }
  if (Config::get().general.verbosity > 1) std::cout << "finished high level calculation\n";
}

void pmf_ic_prep::calc_E_LLs()
{ 
  // catch unvalid low level interface
  if (Config::get().coords.umbrella.pmf_ic.LL_interface == config::interface_types::ILLEGAL) {
    throw std::runtime_error("Illegal low level interface!");
  }

  // create new coordinates object with low level interface
  Config::set().general.energy_interface = Config::get().coords.umbrella.pmf_ic.LL_interface;
  std::unique_ptr<coords::input::format> ci(coords::input::new_format());
  coords::Coordinates coords(ci->read(Config::get().general.inputFilename));

  // calculate low level energies
  for (auto const& pes : *ci)   // for every structure
  {
    coords.set_xyz(pes.structure.cartesian, true);
    auto E = coords.e();
    E_LLs.emplace_back(E);
    if (Config::get().general.verbosity > 3) std::cout << E << "\n";
  }
  if (Config::get().general.verbosity > 1) std::cout << "finished low level calculation\n";
}

void pmf_ic_prep::calc_deltaEs()
{
  for (auto i{ 0u }; i < E_HLs.size(); ++i)
  {
    deltaEs.emplace_back(E_HLs[i] - E_LLs[i]);
  }
}

void pmf_ic_prep::write_to_file()
{
  std::ofstream outfile(outfilename, std::ios_base::out);
  outfile.precision(10);
  outfile << "xi,z,";
  if (dimension > 1) outfile << "xi_2, z_2,";
  outfile<<"E_HL, E_LL, deltaE";                            
  for (auto i{ 0u }; i < E_HLs.size(); ++i)
  {
    if (dimension == 1) outfile << "\n" << xis[i] << "," << zs[i] << ","<<E_HLs[i] << "," << E_LLs[i] << "," << deltaEs[i];
    else outfile << "\n" << xi_2d[i].first << "," << z_2d[i].first << ","<<xi_2d[i].second << "," << z_2d[i].second << "," << E_HLs[i] << "," << E_LLs[i] << "," << deltaEs[i];
  }
  outfile.close();
}

void pmf_ic_prep::write_spline_1d()
{
  Spline1D s;                // create spline
  s.fill(zs, deltaEs);

  auto gpr = gpr::gpr_interpolator_1d(gpr::exponential_kernel(10), xis, deltaEs);

  // write spline to file
  std::ofstream splinefile(splinefilename, std::ios_base::out);
  splinefile.precision(10);
  splinefile << "xi,spline,gpr";   // headline
  auto const& start = Config::get().coords.umbrella.pmf_ic.ranges[0].start;
  auto const& stop = Config::get().coords.umbrella.pmf_ic.ranges[0].stop;
  auto const& step = Config::get().coords.umbrella.pmf_ic.ranges[0].step;
  for (auto xi{ start }; xi <= stop; xi += step)
  {
    auto z = mapping::xi_to_z(xi, Config::get().coords.umbrella.pmf_ic.xi0[0], Config::get().coords.umbrella.pmf_ic.L[0]);
    auto y = s.get_value(z);
    splinefile << "\n" << xi << "," << y << ',' << gpr.interpolate({xi});
  }
  splinefile.close();
}

void pmf_ic_prep::write_spline_2d()
{
  Spline2D s;                // create spline
  s.fill(z_2d, deltaEs);   

  // write spline to file
  std::ofstream splinefile(splinefilename, std::ios_base::out);
  splinefile.precision(10);
  auto const& start1 = Config::get().coords.umbrella.pmf_ic.ranges[0].start;
  auto const& stop1 = Config::get().coords.umbrella.pmf_ic.ranges[0].stop;
  auto const& step1 = Config::get().coords.umbrella.pmf_ic.ranges[0].step;

  for (auto xi{ start1 }; xi <= stop1; xi += step1) splinefile << "," << xi;  // headline
  splinefile << ",xi_1";

  auto const& start2 = Config::get().coords.umbrella.pmf_ic.ranges[1].start;
  auto const& stop2 = Config::get().coords.umbrella.pmf_ic.ranges[1].stop;
  auto const& step2 = Config::get().coords.umbrella.pmf_ic.ranges[1].step;

  for (auto xi2{ start2 }; xi2 <= stop2; xi2 += step2)    // rows = xi_2
  {
    splinefile << "\n" << xi2;
    for (auto xi1{ start1 }; xi1 <= stop1; xi1 += step1)  // columns = xi_1
    {
      auto z1 = mapping::xi_to_z(xi1, Config::get().coords.umbrella.pmf_ic.xi0[0], Config::get().coords.umbrella.pmf_ic.L[0]);
      auto z2 = mapping::xi_to_z(xi2, Config::get().coords.umbrella.pmf_ic.xi0[1], Config::get().coords.umbrella.pmf_ic.L[1]);
      splinefile << "," << s.get_value(std::make_pair(z1, z2));
    }
  }
  splinefile << "\nxi_2";
}
