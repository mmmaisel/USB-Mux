# USB-Mux

![USB-Mux Rendering](https://github.com/mmmaisel/USB-Mux/blob/master/hw/USB-Mux.png?raw=true)

USB-Mux is an Open-Source and Open-Hardware full speed 2x2 USB multiplexer.

It supports a single connection between one of the inputs to one of the outputs.
Additionally, it can disconnect the outputs completely. Power flows from the
selected input to the selected output. Inactive output ports are powerless.

## Usage

The multiplexer is controlled over USB via a virtual terminal. The virtual
terminal will prompt "Mux:" and accepts a single number between 0 and 4 followed
by a newline to select the multiplexer position.
The meaning of the different numbers is printed on the PCB.

To connect to the virtual terminal on Linux you can use the following command:
`picocom /dev/ttyACM0`

## Programming

You can program the CPU on the USB-Mux PCB with OpenOCD and an ST-Nucleo board
as programmer directly from the makefile with `make flash`.

## License

USB-Mux KiCAD hardware design files in the "hw" directory are licenses under the
CERN-OHL-S v2, or (at your option) any later version.

The USB-Mux firmware in the "src" directory is licensed under the
GNU GPLv3, or (at your option) any later version.
