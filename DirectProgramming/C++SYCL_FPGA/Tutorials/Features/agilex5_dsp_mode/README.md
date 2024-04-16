# `Agilex 5 DSP` Sample

This sample is an FPGA tutorial that demonstrates the new 16-bit complex multiplication DSP mode that Agilex 5 supports, and how users can take advantage of this feature when targeting Agilex 5 Family (Sundance Mesa).

| Area                 | Description
|:--                   |:--
| What you will learn  | How to use and what's the benefit of the complex multiplication DSP mode introduced in Agilex 5. <br> How to check the DSP resource usage in the compiler report
| Time to complete     | 15 minutes
| Category             | Concepts and Functionality

## Purpose

This FPGA tutorial shows how users can achieve complex integer multiplication operations with lower DSP consumption on Agilex 5 Family.

Complex integer multiplication involves multiplying two complex numbers, each represented by a pair of integers (real and imaginary parts). In this example with Agilex 5 family, we can see if the operation mode is switched to complex_mult in the output RTL, and whether the DSP usage is exactly half of what it usually would be with Agilex7.

> **Note**: See the [FPGA Optimization Guide for Intel® oneAPI Toolkits Developer Guide](https://www.intel.com/content/www/us/en/docs/oneapi-fpga-add-on/developer-guide/2024-0/declare-the-ac-complex-data-type.html) to see how to declare the `ac_complex` data types.

## Prerequisites

| Optimized for       | Description
|:---                 |:---
| OS                  | Ubuntu* 20.04 <br> RHEL*/CentOS* 8 <br> SUSE* 15 <br> Windows* 10 <br> Windows Server* 2019
| Hardware            | Intel® Agilex® 5
| Software            | Intel® oneAPI DPC++/C++ Compiler

> **Note**: Even though the Intel DPC++/C++ oneAPI compiler is enough to compile for emulation, generating reports and generating RTL, there are extra software requirements for the simulation flow and FPGA compiles.
>
> For using the simulator flow, Intel® Quartus® Prime Pro Edition (or Standard Edition when targeting Cyclone® V) and one of the following simulators must be installed and accessible through your PATH:
> - Questa*-Intel® FPGA Edition
> - Questa*-Intel® FPGA Starter Edition
> - ModelSim® SE
>
> When using the hardware compile flow, Intel® Quartus® Prime Pro Edition (or Standard Edition when targeting Cyclone® V) must be installed and accessible through your PATH.

> **Warning** Make sure you add the device files associated with the FPGA that you are targeting to your Intel® Quartus® Prime installation.

This sample is part of the FPGA code samples.
It is categorized as a Tier 2 sample that demonstrates a compiler feature.

```mermaid
flowchart LR
   tier1("Tier 1: Get Started")
   tier2("Tier 2: Explore the Fundamentals")
   tier3("Tier 3: Explore the Advanced Techniques")
   tier4("Tier 4: Explore the Reference Designs")

   tier1 --> tier2 --> tier3 --> tier4

   style tier1 fill:#0071c1,stroke:#0071c1,stroke-width:1px,color:#fff
   style tier2 fill:#f96,stroke:#333,stroke-width:1px,color:#fff
   style tier3 fill:#0071c1,stroke:#0071c1,stroke-width:1px,color:#fff
   style tier4 fill:#0071c1,stroke:#0071c1,stroke-width:1px,color:#fff
```

Find more information about how to navigate this part of the code samples in the [FPGA top-level README.md](/DirectProgramming/C++SYCL_FPGA/README.md).
You can also find more information about [troubleshooting build errors](/DirectProgramming/C++SYCL_FPGA/README.md#troubleshooting), [running the sample on the Intel® DevCloud](/DirectProgramming/C++SYCL_FPGA/README.md#build-and-run-the-samples-on-intel-devcloud-optional), [using Visual Studio Code with the code samples](/DirectProgramming/C++SYCL_FPGA/README.md#use-visual-studio-code-vs-code-optional), [links to selected documentation](/DirectProgramming/C++SYCL_FPGA/README.md#documentation), and more.

## Key Implementation Details

The sample illustrates the important concepts.

- ac_complex multiplication can be inferred by the new DSP mode in Agilex 5, and be achieved with half of the DSP consumption compared to Agilex 7.
- The input size for the mult-add pair should be <= int16 or <= uint15 (uint16 is not supported due to limitation in the physical silicon), and the output size should be the 'natural word growth' of the input size (e.g. int16 -> int33). 

### Simple Code Example

The following are an example of declaring ac_complex variables:

```cpp
ac_complex<ac_int<W, S> > i(2, 1);
```
Here, `W` is the width in bits and `S` is a bool indicating if the number is signed. Signed numbers use the most significant bit (MSB) to store the sign bit.
The underlying data type can be ac_int, ac_fixed, ap_float, and standard-C integer or floating-point data types.

To use the `ac_complex` type in your code, you must include the following header:

```cpp
#include <sycl/ext/intel/ac_types/ac_complex.hpp> 
```
Additionally, you must pass the  `-qactypes` option to the `icpx` command on Linux or the `/Qactypes` option to the `icx-cl` command on Windows when compiling your SYCL program in order to ensure that the headers are correctly included. In this tutorial, this is done in `src/CMakeLists.txt`.

## Build the `Agilex 5 DSP mode` Tutorial

>**Note**: When working with the command-line interface (CLI), you should configure the oneAPI toolkits using environment variables. Set up your CLI environment by sourcing the `setvars` script in the root of your oneAPI installation every time you open a new terminal window. This practice ensures that your compiler, libraries, and tools are ready for development.
>
> Linux*:
> - For system wide installations: `. /opt/intel/oneapi/setvars.sh`
> - For private installations: ` . ~/intel/oneapi/setvars.sh`
> - For non-POSIX shells, like csh, use the following command: `bash -c 'source <install-dir>/setvars.sh ; exec csh'`
>
> Windows*:
> - `C:\"Program Files (x86)"\Intel\oneAPI\setvars.bat`
> - Windows PowerShell*, use the following command: `cmd.exe "/K" '"C:\Program Files (x86)\Intel\oneAPI\setvars.bat" && powershell'`
>
> For more information on configuring environment variables, see [Use the setvars Script with Linux* or macOS*](https://www.intel.com/content/www/us/en/develop/documentation/oneapi-programming-guide/top/oneapi-development-environment-setup/use-the-setvars-script-with-linux-or-macos.html) or [Use the setvars Script with Windows*](https://www.intel.com/content/www/us/en/develop/documentation/oneapi-programming-guide/top/oneapi-development-environment-setup/use-the-setvars-script-with-windows.html).

### On Linux*

1. Change to the sample directory.
2. Build the program for Intel® Agilex® 5 device family, which is the default.
   ```
   mkdir build
   cd build
   cmake ..
   ```
   > **Note**: You can change the default target by using the command:
   >  ```
   >  cmake .. -DFPGA_DEVICE=<FPGA device family or FPGA part number>
   >  ```
   >
   > Alternatively, you can target an explicit FPGA board variant and BSP by using the following command:
   >  ```
   >  cmake .. -DFPGA_DEVICE=<board-support-package>:<board-variant>
   >  ```
  > **Note**: You can poll your system for available BSPs using the `aoc -list-boards` command. The board list that is printed out will be of the form
  > ```
  > $> aoc -list-boards
  > Board list:
  >   <board-variant>
  >      Board Package: <path/to/board/package>/board-support-package
  >   <board-variant2>
  >      Board Package: <path/to/board/package>/board-support-package
  > ```
   >
   > You will only be able to run an executable on the FPGA if you specified a BSP.

3. Compile the design. (The provided targets match the recommended development flow.)

   1. Compile and run for emulation (fast compile time, targets emulates an FPGA device).
      ```
      make fpga_emu
      ```
   2. Generate the HTML optimization reports. (See [Read the Reports](#read-the-reports) below for information on finding and understanding the reports.)
      ```
      make report
      ```
   3. Compile for simulation (fast compile time, targets simulated FPGA device).
      ```
      make fpga_sim
      ```
   4. Compile and run on FPGA hardware (longer compile time, targets an FPGA device).
      ```
      make fpga
      ```

### On Windows*

1. Change to the sample directory.
2. Build the program for the Intel® Agilex® 5 device family, which is the default.
   ```
   mkdir build
   cd build
   cmake -G "NMake Makefiles" ..
   ```
   > **Note**: You can change the default target by using the command:
   >  ```
   >  cmake -G "NMake Makefiles" .. -DFPGA_DEVICE=<FPGA device family or FPGA part number>
   >  ```
   >
   > Alternatively, you can target an explicit FPGA board variant and BSP by using the following command:
   >  ```
   >  cmake -G "NMake Makefiles" .. -DFPGA_DEVICE=<board-support-package>:<board-variant>
   >  ```
  > **Note**: You can poll your system for available BSPs using the `aoc -list-boards` command. The board list that is printed out will be of the form
  > ```
  > $> aoc -list-boards
  > Board list:
  >   <board-variant>
  >      Board Package: <path/to/board/package>/board-support-package
  >   <board-variant2>
  >      Board Package: <path/to/board/package>/board-support-package
  > ```
   >
   > You will only be able to run an executable on the FPGA if you specified a BSP.

3. Compile the design. (The provided targets match the recommended development flow.)

   1. Compile for emulation (fast compile time, targets emulated FPGA device).
      ```
      nmake fpga_emu
      ```
   2. Generate the optimization report. (See [Read the Reports](#read-the-reports) below for information on finding and understanding the reports.)
      ```
      nmake report
      ```
   3. Compile for simulation (fast compile time, targets simulated FPGA device, reduced problem size).
      ```
      nmake fpga_sim
      ```
   4. Compile for FPGA hardware (longer compile time, targets FPGA device):
      ```
      nmake fpga
      ```
> **Note**: If you encounter any issues with long paths when compiling under Windows*, you may have to create your 'build' directory in a shorter path, for example c:\samples\build.  You can then run cmake from that directory, and provide cmake with the full path to your sample directory, for example:
>
>  ```
  > C:\samples\build> cmake -G "NMake Makefiles" C:\long\path\to\code\sample\CMakeLists.txt
>  ```
### Read the Reports

Locate `report.html` in the `Agilex5_DSP_report.prj/reports/` directory.

On the main report, navigate to the top right tab *Area Estimates* page, and click to expand on the "Kernel System" row in the Area Estimates table. Identify the row "mult", and scroll right to see the DSP resource usage for the kernel's computation. You should be able to see that the DSP resource used is 1.

Repeat the build step, but this time with Agilex 7 passed in as the device family,
```   
cmake .. -DFPGA_DEVICE=Agilex7
```
Locate `report.html` in the `Agilex7_DSP_report.prj/reports/` directory, navigate to the same Area estimates table, you should be able to see that the DSP resource used is now 2. 

You can also check the directory `kernel_hdl/` under `Agilex5_DSP_report.prj/` where all the generated RTL is stored. Doing a quick grep for `"complex_mult"`, we can see that the `"operation_mode"` is set to `"complex_mult"` when compiling for Agilex5, as opposed to `"operation_mode("m18x18_sumof2)"` when we compile to Agilex7.

## Run the `Agilex 5 DSP mode` Sample

### On Linux

1. Run the sample on the FPGA emulator (the kernel executes on the CPU).
   ```
   ./Agilex5_DSP.fpga_emu
   ```
2. Run the sample of the FPGA simulator device (the kernel executes on the CPU).
   ```
   CL_CONTEXT_MPSIM_DEVICE_INTELFPGA=1 ./Agilex5_DSP.fpga_sim
   ```
3. Run the sample on the FPGA device (only if you ran `cmake` with `-DFPGA_DEVICE=<board-support-package>:<board-variant>`).
   ```
   ./Agilex5_DSP.fpga
   ```

### On Windows

1. Run the sample on the FPGA emulator (the kernel executes on the CPU).
   ```
   Agilex5_DSP.fpga_emu.exe
   ```
2. Run the sample of the FPGA simulator device (the kernel executes on the CPU).
   ```
   set CL_CONTEXT_MPSIM_DEVICE_INTELFPGA=1
   Agilex5_DSP.fpga_sim.exe
   set CL_CONTEXT_MPSIM_DEVICE_INTELFPGA=
   ```
3. Run the sample on the FPGA device (only if you ran `cmake` with `-DFPGA_DEVICE=<board-support-package>:<board-variant>`).
   ```
   Agilex5_DSP.fpga.exe
   ```

## Example Output

You will see the device used. If successful, the program displays output similar to the following:

```
PASSED: kernel results are correct.
```


### Understand the Results

Selecting the Agilex 5 family for performing ac_complex multiplications can help lower the DSP resource usage by half in comparison to the Agilex 7 family.

## License

Code samples are licensed under the MIT license. See [License.txt](/License.txt) for details.

Third-party program Licenses can be found here: [third-party-programs.txt](/third-party-programs.txt).