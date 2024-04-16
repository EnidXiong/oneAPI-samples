#include <sycl/sycl.hpp>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_complex.hpp>

#include "exception_handler.hpp"

#include <iostream>
#include <vector>

using namespace sycl;

constexpr access::mode kSyclRead = access::mode::read;
constexpr access::mode kSyclWrite = access::mode::write;
// constexpr access::mode kSyclReadWrite = access::mode::read_write;

template <int W, int S, int W2, int S2, int W3, int S3>
void test_mult(queue &Queue, const ac_complex<ac_int<W, S> > &a,
               const ac_complex<ac_int<W2, S2> > &b,
               ac_complex<ac_int<W3, S3> > &c) {
  buffer<ac_complex<ac_int<W, S> >, 1> inp1(&a, 1);
  buffer<ac_complex<ac_int<W2, S2> >, 1> inp2(&b, 1);
  buffer<ac_complex<ac_int<W3, S3> >, 1> result(&c, 1);

  Queue.submit([&](handler &h) {
    auto x = inp1.template get_access<kSyclRead>(h);
    auto y = inp2.template get_access<kSyclRead>(h);
    auto res = result.template get_access<kSyclWrite>(h);
    h.single_task<class mult>([=] { res[0] = x[0] * y[0]; });
  });
  Queue.wait();
}

template <typename T>
void check_result(T expected, T found, std::string test_name, bool& status) {
  if (expected != found) {
    std::cout << "Result mismatch!\n"
              << "expected:\t" << expected << "\n"
              << "found:\t" << found << "\n";
    status = true;
  }
}

int main() {
#if FPGA_SIMULATOR
  auto selector = sycl::ext::intel::fpga_simulator_selector_v;
#elif FPGA_HARDWARE
  auto selector = sycl::ext::intel::fpga_selector_v;
#else  // #if FPGA_EMULATOR
  auto selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  bool failed = false;
  
  try {
    queue q(selector, fpga_tools::exception_handler);

    auto device = q.get_device();

    std::cout << "Running on device: "
              << device.get_info<sycl::info::device::name>().c_str()
              << std::endl;

    ac_complex<ac_int<21,true>> res;

    res = 0;
     test_mult<10, true, 10, true, 21, true>(q, ac_complex<ac_int<10,true> >(10, 20),
                                            ac_complex<ac_int<10,true> >(5, 10), res);
    check_result(ac_complex<ac_int<21,true> >(-150, 200), res, "test_mult", failed);
  } catch (exception const &e) {
  // Catches exceptions in the host code.
    std::cerr << "Caught a SYCL host exception:\n" << e.what() << "\n";

    // Most likely the runtime couldn't find FPGA hardware!
    if (e.code().value() == CL_DEVICE_NOT_FOUND) {
      std::cerr << "If you are targeting an FPGA, please ensure that your "
                    "system has a correctly configured FPGA board.\n";
      std::cerr << "Run sys_check in the oneAPI root directory to verify.\n";
      std::cerr << "If you are targeting the FPGA emulator, compile with "
                    "-DFPGA_EMULATOR.\n";
    }
    std ::terminate();
  } 

  if (!failed) {
    std::cout << "PASSED: kernel results are correct.\n";
  } else {
    std::cout << "FAILED\n";
  }

  return failed ? 1 : 0;
}
