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
  if not exist release-vc12-x86 md release-vc12-x86
  pushd release-vc12-x86
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS120COMNTOOLS%\..\..\vc\vcvarsall" x86 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Release -G "NMake Makefiles" ..\..
  popd

echo ============================================================================
  if not exist debug-vc12-x86 md debug-vc12-x86
  pushd debug-vc12-x86
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS120COMNTOOLS%\..\..\vc\vcvarsall" x86 >> resetenv.cmd
  call resetenv
  cmake -DTS_VS_CONFIG=Debug -G "NMake Makefiles" ..\..
  popd
  
 
  
echo ============================================================================
  if not exist release-vc12-x64 md release-vc12-x64
  pushd release-vc12-x64
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS120COMNTOOLS%\..\..\vc\vcvarsall" amd64 >> resetenv.cmd
  call resetenv
  cmake -G "NMake Makefiles" -DTS_VS_CONFIG=Release ..\..
  popd
  
echo ============================================================================
  if not exist debug-vc12-x64 md debug-vc12-x64
  pushd debug-vc12-x64
  if not exist resetenv.cmd copy ..\resetenv.cmd . & echo call "%VS120COMNTOOLS%\..\..\vc\vcvarsall" amd64 >> resetenv.cmd
  call resetenv
  cmake -G "NMake Makefiles" -DTS_VS_CONFIG=Debug ..\..
  popd

  
echo SETLOCAL ENABLEEXTENSIONS > buildall.cmd
echo for %%%%i in (debug release) do ( >> buildall.cmd
echo    for %%%%j in (vc12) do ( >> buildall.cmd
echo      for %%%%k in (x86 x64) do ( >> buildall.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> buildall.cmd
echo 		call resetenv >> buildall.cmd
echo 		nmake install >> buildall.cmd
echo        if errorlevel 1 ( >> buildall.cmd
echo           popd  >> buildall.cmd
echo		   goto :eof >> buildall.cmd
echo		)  >> buildall.cmd
echo        if not errorlevel 0 ( >> buildall.cmd
echo           popd  >> buildall.cmd
echo		   goto :eof >> buildall.cmd
echo		)  >> buildall.cmd
echo 		popd >> buildall.cmd
echo 	 ) >> buildall.cmd
echo    ) >> buildall.cmd
echo ) >> buildall.cmd

echo SETLOCAL ENABLEEXTENSIONS > buildrelease.cmd
echo    for %%%%j in (vc12) do ( >> buildrelease.cmd
echo      for %%%%k in (x86 x64) do ( >> buildrelease.cmd
echo 		pushd release-%%%%j-%%%%k >> buildrelease.cmd
echo 		call resetenv >> buildrelease.cmd
echo 		nmake install >> buildrelease.cmd
echo        if errorlevel 1 ( >> buildrelease.cmd
echo           popd  >> buildrelease.cmd
echo		   goto :eof >> buildrelease.cmd
echo		)  >> buildrelease.cmd
echo        if not errorlevel 0 ( >> buildrelease.cmd
echo           popd  >> buildrelease.cmd
echo		   goto :eof >> buildrelease.cmd
echo		)  >> buildrelease.cmd
echo 		popd >> buildrelease.cmd
echo 	 ) >> buildrelease.cmd
echo    ) >> buildrelease.cmd

echo SETLOCAL ENABLEEXTENSIONS > builddebug.cmd
echo    for %%%%j in (vc12) do ( >> builddebug.cmd
echo      for %%%%k in (x86 x64) do ( >> builddebug.cmd
echo 		pushd debug-%%%%j-%%%%k >> builddebug.cmd
echo 		call resetenv >> builddebug.cmd
echo 		nmake install >> builddebug.cmd
echo        if errorlevel 1 ( >> builddebug.cmd
echo           popd  >> builddebug.cmd
echo		   goto :eof >> builddebug.cmd
echo		)  >> builddebug.cmd
echo        if not errorlevel 0 ( >> builddebug.cmd
echo           popd  >> builddebug.cmd
echo		   goto :eof >> builddebug.cmd
echo		)  >> builddebug.cmd
echo 		popd >> builddebug.cmd
echo 	 ) >> builddebug.cmd
echo    ) >> builddebug.cmd

echo SETLOCAL ENABLEEXTENSIONS > jom_buildall.cmd
echo for %%%%i in (debug release) do ( >> jom_buildall.cmd
echo    for %%%%j in (vc12) do ( >> jom_buildall.cmd
echo      for %%%%k in (x86 x64) do ( >> jom_buildall.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> jom_buildall.cmd
echo 		call resetenv >> jom_buildall.cmd
echo 		jom install >> jom_buildall.cmd
echo        if errorlevel 1 ( >> jom_buildall.cmd
echo           popd  >> jom_buildall.cmd
echo		   goto :eof >> jom_buildall.cmd
echo		)  >> jom_buildall.cmd
echo        if not errorlevel 0 ( >> jom_buildall.cmd
echo           popd  >> jom_buildall.cmd
echo		   goto :eof >> jom_buildall.cmd
echo		)  >> jom_buildall.cmd
echo 		popd >> jom_buildall.cmd
echo 	 ) >> jom_buildall.cmd
echo    ) >> jom_buildall.cmd
echo ) >> jom_buildall.cmd

echo SETLOCAL ENABLEEXTENSIONS > jom_buildrelease.cmd
echo    for %%%%j in (vc12) do ( >> jom_buildrelease.cmd
echo      for %%%%k in (x86 x64) do ( >> jom_buildrelease.cmd
echo 		pushd release-%%%%j-%%%%k >> jom_buildrelease.cmd
echo 		call resetenv >> jom_buildrelease.cmd
echo 		jom install >> jom_buildrelease.cmd
echo        if errorlevel 1 ( >> jom_buildrelease.cmd
echo           popd  >> jom_buildrelease.cmd
echo		   goto :eof >> jom_buildrelease.cmd
echo		)  >> jom_buildrelease.cmd
echo        if not errorlevel 0 ( >> jom_buildrelease.cmd
echo           popd  >> jom_buildrelease.cmd
echo		   goto :eof >> jom_buildrelease.cmd
echo		)  >> jom_buildrelease.cmd
echo 		popd >> jom_buildrelease.cmd
echo 	 ) >> jom_buildrelease.cmd
echo    ) >> jom_buildrelease.cmd

echo SETLOCAL ENABLEEXTENSIONS > jom_builddebug.cmd
echo    for %%%%j in (vc12) do ( >> jom_builddebug.cmd
echo      for %%%%k in (x86 x64) do ( >> jom_builddebug.cmd
echo 		pushd debug-%%%%j-%%%%k >> jom_builddebug.cmd
echo 		call resetenv >> jom_builddebug.cmd
echo 		jom install >> jom_builddebug.cmd
echo        if errorlevel 1 ( >> jom_builddebug.cmd
echo           popd  >> jom_builddebug.cmd
echo		   goto :eof >> jom_builddebug.cmd
echo		)  >> jom_builddebug.cmd
echo        if not errorlevel 0 ( >> jom_builddebug.cmd
echo           popd  >> jom_builddebug.cmd
echo		   goto :eof >> jom_builddebug.cmd
echo		)  >> jom_builddebug.cmd
echo 		popd >> jom_builddebug.cmd
echo 	 ) >> jom_builddebug.cmd
echo    ) >> jom_builddebug.cmd

echo SETLOCAL ENABLEEXTENSIONS > cleanall.cmd
echo for %%%%i in (debug release) do ( >> cleanall.cmd
echo    for %%%%j in (vc12) do ( >> cleanall.cmd
echo      for %%%%k in (x86 x64) do ( >> cleanall.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> cleanall.cmd
echo 		call resetenv >> cleanall.cmd
echo 		nmake uninstall >> cleanall.cmd
echo 		nmake clean >> cleanall.cmd
echo 		popd >> cleanall.cmd
echo 	 ) >> cleanall.cmd
echo    ) >> cleanall.cmd
echo ) >> cleanall.cmd

echo SETLOCAL ENABLEEXTENSIONS > cleanrelease.cmd
echo    for %%%%j in (vc12) do ( >> cleanrelease.cmd
echo      for %%%%k in (x86 x64) do ( >> cleanrelease.cmd
echo 		pushd release-%%%%j-%%%%k >> cleanrelease.cmd
echo 		call resetenv >> cleanrelease.cmd
echo 		nmake uninstall >> cleanrelease.cmd
echo 		nmake clean >> cleanrelease.cmd
echo 		popd >> cleanrelease.cmd
echo 	 ) >> cleanrelease.cmd
echo    ) >> cleanrelease.cmd

echo SETLOCAL ENABLEEXTENSIONS > cleandebug.cmd
echo    for %%%%j in (vc12) do ( >> cleandebug.cmd
echo      for %%%%k in (x86 x64) do ( >> cleandebug.cmd
echo 		pushd debug-%%%%j-%%%%k >> cleandebug.cmd
echo 		call resetenv >> cleandebug.cmd
echo 		nmake uninstall >> cleandebug.cmd
echo 		nmake clean >> cleandebug.cmd
echo 		popd >> cleandebug.cmd
echo 	 ) >> cleandebug.cmd
echo    ) >> cleandebug.cmd

echo SETLOCAL ENABLEEXTENSIONS > installall.cmd
echo for %%%%i in (debug release) do ( >> installall.cmd
echo    for %%%%j in (vc12) do ( >> installall.cmd
echo      for %%%%k in (x86 x64) do ( >> installall.cmd
echo 		pushd %%%%i-%%%%j-%%%%k >> installall.cmd
echo 		call resetenv >> installall.cmd
echo 		nmake install >> installall.cmd
echo        if errorlevel 1 ( >> installall.cmd
echo           popd  >> installall.cmd
echo		   goto :eof >> installall.cmd
echo		)  >> installall.cmd
echo        if not errorlevel 0 ( >> installall.cmd
echo           popd  >> installall.cmd
echo		   goto :eof >> installall.cmd
echo		)  >> installall.cmd
echo 		popd >> installall.cmd
echo 	 ) >> installall.cmd
echo    ) >> installall.cmd
echo ) >> installall.cmd

echo SETLOCAL ENABLEEXTENSIONS > installrelease.cmd
echo    for %%%%j in (vc12) do ( >> installrelease.cmd
echo      for %%%%k in (x86 x64) do ( >> installrelease.cmd
echo 		pushd release-%%%%j-%%%%k >> installrelease.cmd
echo 		call resetenv >> installrelease.cmd
echo 		nmake install >> installrelease.cmd
echo        if errorlevel 1 ( >> installrelease.cmd
echo           popd  >> installrelease.cmd
echo		   goto :eof >> installrelease.cmd
echo		)  >> installrelease.cmd
echo        if not errorlevel 0 ( >> installrelease.cmd
echo           popd  >> installrelease.cmd
echo		   goto :eof >> installrelease.cmd
echo		)  >> installrelease.cmd
echo 		popd >> installrelease.cmd
echo 	 ) >> installrelease.cmd
echo    ) >> installrelease.cmd

echo SETLOCAL ENABLEEXTENSIONS > installdebug.cmd
echo    for %%%%j in (vc12) do ( >> installdebug.cmd
echo      for %%%%k in (x86 x64) do ( >> installdebug.cmd
echo 		pushd debug-%%%%j-%%%%k >> installdebug.cmd
echo 		call resetenv >> installdebug.cmd
echo 		nmake install >> installdebug.cmd
echo        if errorlevel 1 ( >> installdebug.cmd
echo           popd  >> installdebug.cmd
echo		   goto :eof >> installdebug.cmd
echo		)  >> installdebug.cmd
echo        if not errorlevel 0 ( >> installdebug.cmd
echo           popd  >> installdebug.cmd
echo		   goto :eof >> installdebug.cmd
echo		)  >> installdebug.cmd
echo 		popd >> installdebug.cmd
echo 	 ) >> installdebug.cmd
echo    ) >> installdebug.cmd

  
for %%i in (debug release) do (
  for %%j in (vc12) do (
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
