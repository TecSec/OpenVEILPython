# OpenVEIL Python Wrapper

Wraps OpenVEIL objects to allow Python applications to communicate with a VEIL Server.

**Note:** Currently only Python x86 is supported

## Installation

We use an out of source build process.  The project/make files and output
can be found in the build folder that is created by the bootstrap.

### Requirements

- Windows
  - CMake 3.2+
  - Visual Studio 2015
  - [OpenVEIL](https://github.com/TecSec/OpenVEIL)
- Linux
  - CMake 3.2+
  - GCC 4.8.2+
  - [OpenVEIL](https://github.com/TecSec/OpenVEIL)

### Windows

1. Clone the repository
2. Go to the directory of the repository in a command prompt as Administrator
3. Set the PYTHON_PATH Environment Variable to the python installation location
  - See [here](http://www.computerhope.com/issues/ch000549.htm) for infomation on navigating
  to the Environment Variables window
  - Once there click the "New" button
  - Enter PYTHON_PATH for "Variable name" For example "C:\Program Files (x86)\Python35-32"
4. `cd make\windows`
5. `bootstrap_VS2015.cmd`
6. `cd ..\..\Build`
7. To build release: `buildrelease-vc14.cmd` or to build release and debug (the python debug binaries need to be installed):  `buildall-vc14.cmd`
8. After the build is complete there should be an OpenVEILPython the OpenVEIL_7-0 directory. The default location is C:\TecSec\OpenVEIL_7-0\vc14\OpenVEILPython 
Within that directory there is a bin directory which contains the OpenVEIL.pyd and required DLLs for that to import.

### Linux

TODO: Describe the installation process

## Usage

TODO: Write usage instructions

## Documentation

Documentation is not available yet.  Sorry.

## Contributing

To become a contributor to this project please look at the document called
**Contribution Agreement.pdf**

## License

See **LICENSE** file
