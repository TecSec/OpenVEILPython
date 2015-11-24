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
  if not exist release-vc14-x86 md release-vc14-x86
  pushd release-vc14-x86
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" x86 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Release -G "NMake Makefiles" ..\..
  popd

echo ============================================================================
  if not exist debug-vc14-x86 md debug-vc14-x86
  pushd debug-vc14-x86
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" x86 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Debug -G "NMake Makefiles" ..\..
  popd
  
 
  
echo ============================================================================
  if not exist release-vc14-x64 md release-vc14-x64
  pushd release-vc14-x64
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" amd64 >> resetenv.cmd
  call resetenv
  cmake -G "NMake Makefiles" -DTS_VS_CONFIG=Release ..\..
  popd
  
echo ============================================================================
  if not exist debug-vc14-x64 md debug-vc14-x64
  pushd debug-vc14-x64
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS140COMNTOOLS%\..\..\vc\vcvarsall" amd64 >> resetenv.cmd
  call resetenv
  cmake -G "NMake Makefiles" -DTS_VS_CONFIG=Debug ..\..
  popd

  
echo SETLOCAL ENABLEEXTENSIONS > buildall-vc14.cmd
echo for %%%%i in (debug release) do ( >> buildall-vc14.cmd
echo    for %%%%j in (vc14) do ( >> buildall-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> buildall-vc14.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> buildall-vc14.cmd
echo 		call resetenv >> buildall-vc14.cmd
echo 		nmake install >> buildall-vc14.cmd
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

echo SETLOCAL ENABLEEXTENSIONS > buildrelease-vc14.cmd
echo    for %%%%j in (vc14) do ( >> buildrelease-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> buildrelease-vc14.cmd
echo 		pushd release-%%%%j-%%%%k >> buildrelease-vc14.cmd
echo 		call resetenv >> buildrelease-vc14.cmd
echo 		nmake install >> buildrelease-vc14.cmd
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

echo SETLOCAL ENABLEEXTENSIONS > builddebug-vc14.cmd
echo    for %%%%j in (vc14) do ( >> builddebug-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> builddebug-vc14.cmd
echo 		pushd debug-%%%%j-%%%%k >> builddebug-vc14.cmd
echo 		call resetenv >> builddebug-vc14.cmd
echo 		nmake install >> builddebug-vc14.cmd
echo        if errorlevel 1 ( >> builddebug-vc14.cmd
echo           popd  >> builddebug-vc14.cmd
echo		   goto :eof >> builddebug-vc14.cmd
echo		)  >> builddebug-vc14.cmd
echo        if not errorlevel 0 ( >> builddebug-vc14.cmd
echo           popd  >> builddebug-vc14.cmd
echo		   goto :eof >> builddebug-vc14.cmd
echo		)  >> builddebug-vc14.cmd
echo 		popd >> builddebug-vc14.cmd
echo 	 ) >> builddebug-vc14.cmd
echo    ) >> builddebug-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > jom_buildall-vc14.cmd
echo for %%%%i in (debug release) do ( >> jom_buildall-vc14.cmd
echo    for %%%%j in (vc14) do ( >> jom_buildall-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> jom_buildall-vc14.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> jom_buildall-vc14.cmd
echo 		call resetenv >> jom_buildall-vc14.cmd
echo 		jom install >> jom_buildall-vc14.cmd
echo        if errorlevel 1 ( >> jom_buildall-vc14.cmd
echo           popd  >> jom_buildall-vc14.cmd
echo		   goto :eof >> jom_buildall-vc14.cmd
echo		)  >> jom_buildall-vc14.cmd
echo        if not errorlevel 0 ( >> jom_buildall-vc14.cmd
echo           popd  >> jom_buildall-vc14.cmd
echo		   goto :eof >> jom_buildall-vc14.cmd
echo		)  >> jom_buildall-vc14.cmd
echo 		popd >> jom_buildall-vc14.cmd
echo 	 ) >> jom_buildall-vc14.cmd
echo    ) >> jom_buildall-vc14.cmd
echo ) >> jom_buildall-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > jom_buildrelease-vc14.cmd
echo    for %%%%j in (vc14) do ( >> jom_buildrelease-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> jom_buildrelease-vc14.cmd
echo 		pushd release-%%%%j-%%%%k >> jom_buildrelease-vc14.cmd
echo 		call resetenv >> jom_buildrelease-vc14.cmd
echo 		jom install >> jom_buildrelease-vc14.cmd
echo        if errorlevel 1 ( >> jom_buildrelease-vc14.cmd
echo           popd  >> jom_buildrelease-vc14.cmd
echo		   goto :eof >> jom_buildrelease-vc14.cmd
echo		)  >> jom_buildrelease-vc14.cmd
echo        if not errorlevel 0 ( >> jom_buildrelease-vc14.cmd
echo           popd  >> jom_buildrelease-vc14.cmd
echo		   goto :eof >> jom_buildrelease-vc14.cmd
echo		)  >> jom_buildrelease-vc14.cmd
echo 		popd >> jom_buildrelease-vc14.cmd
echo 	 ) >> jom_buildrelease-vc14.cmd
echo    ) >> jom_buildrelease-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > jom_builddebug-vc14.cmd
echo    for %%%%j in (vc14) do ( >> jom_builddebug-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> jom_builddebug-vc14.cmd
echo 		pushd debug-%%%%j-%%%%k >> jom_builddebug-vc14.cmd
echo 		call resetenv >> jom_builddebug-vc14.cmd
echo 		jom install >> jom_builddebug-vc14.cmd
echo        if errorlevel 1 ( >> jom_builddebug-vc14.cmd
echo           popd  >> jom_builddebug-vc14.cmd
echo		   goto :eof >> jom_builddebug-vc14.cmd
echo		)  >> jom_builddebug-vc14.cmd
echo        if not errorlevel 0 ( >> jom_builddebug-vc14.cmd
echo           popd  >> jom_builddebug-vc14.cmd
echo		   goto :eof >> jom_builddebug-vc14.cmd
echo		)  >> jom_builddebug-vc14.cmd
echo 		popd >> jom_builddebug-vc14.cmd
echo 	 ) >> jom_builddebug-vc14.cmd
echo    ) >> jom_builddebug-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > cleanall-vc14.cmd
echo for %%%%i in (debug release) do ( >> cleanall-vc14.cmd
echo    for %%%%j in (vc14) do ( >> cleanall-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> cleanall-vc14.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> cleanall-vc14.cmd
echo 		call resetenv >> cleanall-vc14.cmd
echo 		nmake uninstall >> cleanall-vc14.cmd
echo 		nmake clean >> cleanall-vc14.cmd
echo 		popd >> cleanall-vc14.cmd
echo 	 ) >> cleanall-vc14.cmd
echo    ) >> cleanall-vc14.cmd
echo ) >> cleanall-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > cleanrelease-vc14.cmd
echo    for %%%%j in (vc14) do ( >> cleanrelease-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> cleanrelease-vc14.cmd
echo 		pushd release-%%%%j-%%%%k >> cleanrelease-vc14.cmd
echo 		call resetenv >> cleanrelease-vc14.cmd
echo 		nmake uninstall >> cleanrelease-vc14.cmd
echo 		nmake clean >> cleanrelease-vc14.cmd
echo 		popd >> cleanrelease-vc14.cmd
echo 	 ) >> cleanrelease-vc14.cmd
echo    ) >> cleanrelease-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > cleandebug-vc14.cmd
echo    for %%%%j in (vc14) do ( >> cleandebug-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> cleandebug-vc14.cmd
echo 		pushd debug-%%%%j-%%%%k >> cleandebug-vc14.cmd
echo 		call resetenv >> cleandebug-vc14.cmd
echo 		nmake uninstall >> cleandebug-vc14.cmd
echo 		nmake clean >> cleandebug-vc14.cmd
echo 		popd >> cleandebug-vc14.cmd
echo 	 ) >> cleandebug-vc14.cmd
echo    ) >> cleandebug-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > installall-vc14.cmd
echo for %%%%i in (debug release) do ( >> installall-vc14.cmd
echo    for %%%%j in (vc14) do ( >> installall-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> installall-vc14.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> installall-vc14.cmd
echo 		call resetenv >> installall-vc14.cmd
echo 		nmake install >> installall-vc14.cmd
echo        if errorlevel 1 ( >> installall-vc14.cmd
echo           popd  >> installall-vc14.cmd
echo		   goto :eof >> installall-vc14.cmd
echo		)  >> installall-vc14.cmd
echo        if not errorlevel 0 ( >> installall-vc14.cmd
echo           popd  >> installall-vc14.cmd
echo		   goto :eof >> installall-vc14.cmd
echo		)  >> installall-vc14.cmd
echo 		popd >> installall-vc14.cmd
echo 	 ) >> installall-vc14.cmd
echo    ) >> installall-vc14.cmd
echo ) >> installall-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > installrelease-vc14.cmd
echo    for %%%%j in (vc14) do ( >> installrelease-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> installrelease-vc14.cmd
echo 		pushd release-%%%%j-%%%%k >> installrelease-vc14.cmd
echo 		call resetenv >> installrelease-vc14.cmd
echo 		nmake install >> installrelease-vc14.cmd
echo        if errorlevel 1 ( >> installrelease-vc14.cmd
echo           popd  >> installrelease-vc14.cmd
echo		   goto :eof >> installrelease-vc14.cmd
echo		)  >> installrelease-vc14.cmd
echo        if not errorlevel 0 ( >> installrelease-vc14.cmd
echo           popd  >> installrelease-vc14.cmd
echo		   goto :eof >> installrelease-vc14.cmd
echo		)  >> installrelease-vc14.cmd
echo 		popd >> installrelease-vc14.cmd
echo 	 ) >> installrelease-vc14.cmd
echo    ) >> installrelease-vc14.cmd

echo SETLOCAL ENABLEEXTENSIONS > installdebug-vc14.cmd
echo    for %%%%j in (vc14) do ( >> installdebug-vc14.cmd
echo      for %%%%k in (x86 x64) do ( >> installdebug-vc14.cmd
echo 		pushd debug-%%%%j-%%%%k >> installdebug-vc14.cmd
echo 		call resetenv >> installdebug-vc14.cmd
echo 		nmake install >> installdebug-vc14.cmd
echo        if errorlevel 1 ( >> installdebug-vc14.cmd
echo           popd  >> installdebug-vc14.cmd
echo		   goto :eof >> installdebug-vc14.cmd
echo		)  >> installdebug-vc14.cmd
echo        if not errorlevel 0 ( >> installdebug-vc14.cmd
echo           popd  >> installdebug-vc14.cmd
echo		   goto :eof >> installdebug-vc14.cmd
echo		)  >> installdebug-vc14.cmd
echo 		popd >> installdebug-vc14.cmd
echo 	 ) >> installdebug-vc14.cmd
echo    ) >> installdebug-vc14.cmd

  
for %%i in (debug release) do (
  for %%j in (vc14) do (
    for %%k in (x86 x64) do (
      echo @echo off > %%i-%%j-%%k\build.cmd
      echo call resetenv >> %%i-%%j-%%k\build.cmd
      echo nmake install >> %%i-%%j-%%k\build.cmd

      echo @echo off > %%i-%%j-%%k\clean.cmd
      echo call resetenv >> %%i-%%j-%%k\clean.cmd
      echo nmake uninstall >> %%i-%%j-%%k\clean.cmd
      echo nmake clean >> %%i-%%j-%%k\clean.cmd
	  
      echo @echo off > %%i-%%j-%%k\jom_build.cmd
      echo call resetenv >> %%i-%%j-%%k\jom_build.cmd
      echo jom install >> %%i-%%j-%%k\jom_build.cmd
    )
  )
)
  
popd
