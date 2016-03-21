# OpenVEIL Python Wrapper

Wraps the OpenVEIL connection object to allow Python applications to send commands to a VEIL Server.

**Note:** Currently only Python x86 is supported

## Installation

We use an out of source build process.  The project/make files and output
can be found in the build folder that is created by the bootstrap.

### Requirements

- Windows
  - CMake 3.2+
  - Visual Studio 2013 or Visual Studio 2015
  - [Boost 1.60](http://sourceforge.net/projects/boost/files/boost/1.60.0/)
  - [OpenVEIL](https://github.com/TecSec/OpenVEIL)
- Linux
  - CMake 3.2+
  - GCC 4.8.2+
  - [Boost.Python 1.60](http://sourceforge.net/projects/boost/files/boost/1.60.0/)
  - [OpenVEIL](https://github.com/TecSec/OpenVEIL)

### Windows

#### Boost
1. Download Boost from the link above
2. Extract Boost anywhere
3. Open a Command Prompt and go to the Boost directory
4. `.\bootstrap.bat`
5. `.\b2 toolset=msvc --with-python --build-type=complete install`
6. Boost.Python should now be installed at C:\Boost

#### OpenVEILPythonWrapper
1. Clone the repository
2. Go to the directory of the repository in a command prompt
3. Set the PYTHON_PATH Environment Variable to the python installation location
  - See [here](http://www.computerhope.com/issues/ch000549.htm) for infomation on navigating
  to the Environment Variables window
  - Once there click the "New" button
  - Entere PYTHON_PATH for "Variable name" For example "C:\Program Files (x86)\Python35-32"
4. `cd make\windows`
5. Run the following command to bootstrap x86 and x64(Debug and Release)
  - If Visual Studio 2015:
    - `bootstrap_VS2015.cmd`
  - If Visual Studio 2013:
    - `bootstrap_VS2013.cmd`
6. `cd ..\..\Build`
7. Run the following command to build x86 and x64(Debug and Release)
  - If Visual Studio 2015:
    - `buildall-vc14.cmd`
  - If Visual Studio 2013:
    - `buildall-vc12.cmd`
8. After the build is complete there should be an OpenVEILPython the OpenVEIL_7-0 directory.
Within that directory there is a bin directory which contains the OpenVEIL.pyd and required DLLs for that to import.
  - If Visual Studio 2015:
    - C:\TecSec\OpenVEIL_7-0\vc14\OpenVEILPython
  - If Visual Studio 2013:
    - C:\TecSec\OpenVEIL_7-0\vc12\OpenVEILPython

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
