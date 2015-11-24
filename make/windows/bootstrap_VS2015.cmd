@echo off
rem	Copyright (c) 2015, TecSec, Inc.
rem
rem	Redistribution and use in source and binary forms, with or without
rem	modification, are permitted provided that the following conditions are met:
rem	
rem		* Redistributions of source code must retain the above copyright
rem		  notice, this list of conditions and the following disclaimer.
rem		* Redistributions in binary form must reproduce the above copyright
rem		  notice, this list of conditions and the following disclaimer in the
rem		  documentation and/or other materials provided with the distribution.
rem		* Neither the name of TecSec nor the names of the contributors may be
rem		  used to endorse or promote products derived from this software 
rem		  without specific prior written permission.
rem		 
rem	ALTERNATIVELY, provided that this notice is retained in full, this product
rem	may be distributed under the terms of the GNU General Public License (GPL),
rem	in which case the provisions of the GPL apply INSTEAD OF those given above.
rem		 
rem	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
rem	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
rem	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
rem	DISCLAIMED.  IN NO EVENT SHALL TECSEC BE LIABLE FOR ANY 
rem	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
rem	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
rem	LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
rem	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
rem	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
rem	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
rem

if not exist ..\..\Build md ..\..\Build
if exist ..\..\Build\resetenv.cmd goto okToBootstrap
copy resetenv.cmd.in ..\..\Build\resetenv.cmd.in
copy resetenv.cmd.in ..\..\Build\resetenv.cmd
echo The resetenv.cmd file has not been customized for your development machine.  Please go to the build folder and rename resetenv.cmd.in to resetenv.cmd and update it for your environment.
echo.

:okToBootstrap

pushd ..\..\Build

echo ============================================================================
  if not exist vsdebug-vc14-x86 md vsdebug-vc14-x86
  pushd vsdebug-vc14-x86
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" x86 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Debug -G "Visual Studio 14" ..\..
  popd

echo ============================================================================
  if not exist vsrelease-vc14-x86 md vsrelease-vc14-x86
  pushd vsrelease-vc14-x86
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" x86 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Release -G "Visual Studio 14" ..\..
  popd

 
  
echo ============================================================================
  if not exist vsdebug-vc14-x64 md vsdebug-vc14-x64
  pushd vsdebug-vc14-x64
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" amd64 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Debug -G "Visual Studio 14 Win64" ..\..
  popd
  
echo ============================================================================
  if not exist vsrelease-vc14-x64 md vsrelease-vc14-x64
  pushd vsrelease-vc14-x64
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" amd64 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Release -G "Visual Studio 14 Win64" ..\..
  popd
  
  
echo @echo off > buildall-vc14.cmd
echo SETLOCAL ENABLEEXTENSIONS > buildall-vc14.cmd
echo for %%%%i in (debug release) do ( >> buildall-vc14.cmd
echo    for %%%%j in (vc14) do ( >> buildall-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> buildall-vc14.cmd
echo 		pushd vs%%%%i-%%%%j-%%%%k >> buildall-vc14.cmd
echo        call resetenv.cmd >> buildall-vc14.cmd
echo 		cmake --build . --target install --config %%%%i >> buildall-vc14.cmd
echo        if errorlevel 1 ( >> buildall-vc14.cmd
echo           popd  >> buildall-vc14.cmd
echo		   goto :eof >> buildall-vc14.cmd
echo		)  >> buildall-vc14.cmd
echo        if not errorlevel 0 ( >> buildall-vc14.cmd
echo           popd  >> buildall-vc14.cmd
echo		   goto :eof >> buildall-vc14.cmd
echo		)  >> buildall-vc14.cmd
echo 		popd >> buildall-vc14.cmd
echo 	 ) >> buildall-vc14.cmd
echo    ) >> buildall-vc14.cmd
echo ) >> buildall-vc14.cmd

echo @echo off > buildrelease-vc14.cmd
echo SETLOCAL ENABLEEXTENSIONS > buildrelease-vc14.cmd
echo for %%%%i in (release) do ( >> buildrelease-vc14.cmd
echo    for %%%%j in (vc14) do ( >> buildrelease-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> buildrelease-vc14.cmd
echo 		pushd vs%%%%i-%%%%j-%%%%k >> buildrelease-vc14.cmd
echo        call resetenv.cmd >> buildrelease-vc14.cmd
echo 		cmake --build . --target uninstall --config %%%%i >> buildrelease-vc14.cmd
echo 		cmake --build . --clean-first --target install --config %%%%i >> buildrelease-vc14.cmd
echo        if errorlevel 1 ( >> buildrelease-vc14.cmd
echo           popd  >> buildrelease-vc14.cmd
echo		   goto :eof >> buildrelease-vc14.cmd
echo		)  >> buildrelease-vc14.cmd
echo        if not errorlevel 0 ( >> buildrelease-vc14.cmd
echo           popd  >> buildrelease-vc14.cmd
echo		   goto :eof >> buildrelease-vc14.cmd
echo		)  >> buildrelease-vc14.cmd
echo 		popd >> buildrelease-vc14.cmd
echo 	 ) >> buildrelease-vc14.cmd
echo    ) >> buildrelease-vc14.cmd
echo ) >> buildrelease-vc14.cmd

echo @echo off > cleanall-vc14.cmd
echo SETLOCAL ENABLEEXTENSIONS > cleanall-vc14.cmd
echo for %%%%i in (release debug) do ( >> cleanall-vc14.cmd
echo    for %%%%j in (vc14) do ( >> cleanall-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> cleanall-vc14.cmd
echo 		pushd vs%%%%i-%%%%j-%%%%k >> cleanall-vc14.cmd
echo        call resetenv.cmd >> cleanall-vc14.cmd
echo 		cmake --build . --target uninstall --config %%%%i >> cleanall-vc14.cmd
echo 		cmake --build . --target clean --config %%%%i >> cleanall-vc14.cmd
echo 		popd >> cleanall-vc14.cmd
echo 	 ) >> cleanall-vc14.cmd
echo    ) >> cleanall-vc14.cmd
echo ) >> cleanall-vc14.cmd

echo @echo off > cleanrelease-vc14.cmd
echo SETLOCAL ENABLEEXTENSIONS > cleanrelease-vc14.cmd
echo for %%%%i in (release) do ( >> cleanrelease-vc14.cmd
echo    for %%%%j in (vc14) do ( >> cleanrelease-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> cleanrelease-vc14.cmd
echo 		pushd vs%%%%i-%%%%j-%%%%k >> cleanrelease-vc14.cmd
echo        call resetenv.cmd >> cleanrelease-vc14.cmd
echo 		cmake --build . --target uninstall --config %%%%i >> cleanrelease-vc14.cmd
echo 		cmake --build . --target clean --config %%%%i >> cleanrelease-vc14.cmd
echo 		popd >> cleanrelease-vc14.cmd
echo 	 ) >> cleanrelease-vc14.cmd
echo    ) >> cleanrelease-vc14.cmd
echo ) >> cleanrelease-vc14.cmd

popd
