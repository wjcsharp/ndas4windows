@echo OSR DDKBUILD.BAT V5.3 - OSR, Open Systems Resources, Inc.
@echo off
rem /////////////////////////////////////////////////////////////////////////////
rem //
rem //    This sofware is supplied for instructional purposes only.
rem //
rem //    OSR Open Systems Resources, Inc. (OSR) expressly disclaims any warranty
rem //    for this software.  THIS SOFTWARE IS PROVIDED  "AS IS" WITHOUT WARRANTY
rem //    OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, WITHOUT LIMITATION,
rem //    THE IMPLIED WARRANTIES OF MECHANTABILITY OR FITNESS FOR A PARTICULAR
rem //    PURPOSE.  THE ENTIRE RISK ARISING FROM THE USE OF THIS SOFTWARE REMAINS
rem //    WITH YOU.  OSR's entire liability and your exclusive remedy shall not
rem //    exceed the price paid for this material.  In no event shall OSR or its
rem //    suppliers be liable for any damages whatsoever (including, without
rem //    limitation, damages for loss of business profit, business interruption,
rem //    loss of business information, or any other pecuniary loss) arising out
rem //    of the use or inability to use this software, even if OSR has been
rem //    advised of the possibility of such damages.  Because some states/
rem //    jurisdictions do not allow the exclusion or limitation of liability for
rem //    consequential or incidental damages, the above limitation may not apply
rem //    to you.
rem //
rem //    OSR Open Systems Resources, Inc.
rem //    105 Route 101A Suite 19
rem //    Amherst, NH 03031  (603) 595-6500 FAX: (603) 595-6503
rem //    email bugs to: bugs@osr.com
rem //
rem //
rem //    MODULE:
rem //
rem //        ddkbuild.bat 
rem //
rem //    ABSTRACT:
rem //
rem //      This file allows drivers to be build with visual studio and visual studio.net
rem //
rem //    AUTHOR(S):
rem //
rem //        OSR Open Systems Resources, Inc.
rem // 
rem //    REVISION:   V5.3
rem //
rem //      Fix a couple of bugs where the parameter was suppose to be "parameter"
rem //
rem //
rem //    REQUIREMENTS:  Environment variables that must be set.
rem //
rem //		BASEDIR - Automatically set up by NT4 DDK. (e.g. D:\NT4DDK )
rem //      W2KBASE - must be set up by user to point to W2K DDK  (e.g D:\Nt50DDK )
rem //      WXPBASE - must be set up by user to point to WXP DDK  (e.g D:\WINDDK\2600)
rem //      WNETBASE - must be set up by user to point to WNET DDK (e.g D:\WINDDK\3615) 
rem //
rem //
rem //    COMMAND FORMAT:
rem //
rem //		ddkbuild -PLATFORM BUILDTYPE DIRECTORY [FLAGS]
rem //
rem //              PLATFORM is either 
rem //                   WXP, WXP64, WXP2K - builds using WXP DDK
rem //                   W2K, W2K64,  - builds using W2k DDK
rem //                   WNET, WNET64, WNET2K, WNETXP, WNETXP64 - builds using WNET DDK
rem //                   NT4  - build using NT4 DDK (NT4 is the default)
rem //              BUILDTYPE - free, checked, chk or fre
rem //				DIRECTORY is the path to the directory to be build.  It can be "."
rem //      
rem //
rem //	  BROWSE FILES:
rem //	
rem //       This procedure supports the building of BROWSE files to be used by 
rem //       Visual Studio 6 and by Visual Studio.Net  However, the BSCfiles created
rem //       by bscmake for the 2 studios are not compatible. When this command procedure
rem //       runs, it selects the first bscmake.exe found in the path.   So, make
rem //       sure that the correct bscmake.exe is in the path....
rem //
rem //    COMPILERS:
rem //
rem //        If you are building NT4 or Windows 2000 drivers you should really
rem //        be using the VC 6 compiler.   If you are building a WXP driver,
rem //        you should be using the compiler that comes with the DDK.  This 
rem //        procedure should use the correct compiler.
rem //       
rem //    GENERAL COMMENTS:
rem //        This procedure is not written to be elegant!  It is written to work 
rem //        and to be easy to debug.   While we could have reused a bunch of
rem //        code, we decided not to.   
rem //
rem ///////////////////////////////////////////////////////////////////////////////

set scriptDebug=off
setlocal ENABLEEXTENSIONS

@echo %scriptDebug%

rem //
rem // Check for NT 4 Build
rem //
if /I %1 NEQ -NT4   goto NoNT4Base

@echo NT4 BUILD using NT4 DDK

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set mode=
for %%f in (free FREE fre FRE) do if %%f == %1 set mode=free
for %%f in (checked CHECKED chk CHK) do if %%f == %1 set mode=checked
if "%mode%" =="" goto ErrBadMode

if "%2" == "" goto ErrNoDir

if not exist %2 goto ErrNoDir

pushd .
call %BASEDIR%\bin\setenv.bat %BASEDIR% %mode% "%MSDEVDIR%"
popd

@echo %scriptDebug%

goto RegularBuild

:NoNT4Base

rem //
rem // Check for WNET Windows 2000 Build using WNET DDK
rem //
if /I %1 NEQ -WNET2K goto NoWNET2KBase

@echo W2K BUILD using WNET DDK

set BASEDIR=%WNETBASE%

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set mode=
for %%f in (free FREE fre FRE) do if %%f == %1 set mode=f
for %%f in (checked CHECKED chk CHK) do if %%f == %1 set mode=c
if "%mode%" =="" goto ErrBadMode

if "%2" == "" goto ErrNoDir

if not exist %2 goto ErrNoDir

pushd .
call %BASEDIR%\bin\setenv.bat %BASEDIR% W2K %mode% 
popd

@echo %scriptDebug%

goto RegularBuild

:NoWNET2KBase

rem //
rem // Check for WXP Build using WNET DDK
rem //
if /I %1 NEQ -WNETXP goto NoWNETXPBase

@echo WXP BUILD using WNET DDK

set BASEDIR=%WNETBASE%

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set mode=
for %%f in (free FREE fre FRE) do if %%f == %1 set mode=fre
for %%f in (checked CHECKED chk CHK) do if %%f == %1 set mode=chk
if "%mode%" =="" goto ErrBadMode


if "%2" == "" goto ErrNoDir

if not exist %2 goto ErrNoDir

pushd .
call %BASEDIR%\bin\setenv.bat %BASEDIR% %mode% WXP 
popd

@echo %scriptDebug%

goto RegularBuild

:NoWNETXPBase

rem //
rem // Check for WXP 64 bit Build using WNET DDK
rem //
if /I %1 NEQ -WNETXP64 goto NoWNETXP64Base

@echo WXP 64 BIT BUILD using WNET DDK

set BASEDIR=%WNETBASE%

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set mode=
for %%f in (free FREE fre FRE) do if %%f == %1 set mode=fre
for %%f in (checked CHECKED chk CHK) do if %%f == %1 set mode=chk
if "%mode%" =="" goto ErrBadMode

if "%2" == "" goto ErrNoDir

if not exist %2 goto ErrNoDir

pushd .
call %BASEDIR%\bin\setenv.bat %BASEDIR% %mode% 64 WXP 
popd

@echo %scriptDebug%

goto RegularBuild

:NoWNETXP64Base

rem //
rem // Check for WNET 64 bit Build using WNET DDK
rem //
if /I %1 NEQ -WNET64 goto NoWNET64Base

@echo WNET 64 BIT BUILD using WNET DDK

set BASEDIR=%WNETBASE%

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set mode=
for %%f in (free FREE fre FRE) do if %%f == %1 set mode=fre
for %%f in (checked CHECKED chk CHK) do if %%f == %1 set mode=chk
if "%mode%" =="" goto ErrBadMode

if "%2" == "" goto ErrNoDir

if not exist %2 goto ErrNoDir

pushd .
call %BASEDIR%\bin\setenv.bat %BASEDIR% %mode% 64 WNET 
popd

@echo %scriptDebug%

goto RegularBuild

:NoWNET64Base

rem //
rem // Check for WNET 32 BIT BUILD using WNET DDK
rem //
if /I %1 NEQ -WNET goto NoWNETBase

@echo WNET 32 BIT BUILD using WNET DDK

set BASEDIR=%WNETBASE%

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set mode=
for %%f in (free FREE fre FRE) do if %%f == %1 set mode=fre
for %%f in (checked CHECKED chk CHK) do if %%f == %1 set mode=chk
if "%mode%" =="" goto ErrBadMode

if "%2" == "" goto ErrNoDir

if not exist %2 goto ErrNoDir

pushd .
call %BASEDIR%\bin\setenv.bat %BASEDIR% %mode%
popd

@echo %scriptDebug%

goto RegularBuild

:NoWNETBase

rem //
rem // Check for WXP 64 BIT BUILD using WXP DDK
rem //
if /I %1 NEQ -WXP64 goto NoWxp64Base

@echo WXP 64 BIT BUILD using WXP DDK

set BASEDIR=%WXPBASE%

shift

if "%BASEDIR%"=="" goto ErrNoBASEDIR

set path=%BASEDIR%\bin;%path%

set
ѓ               §!p                                                 р                              h 1	 џџh 1	 џџx 1	 џџx 1	 џџ                         1	 џџ  1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџ№1	 џџ        9            НKoP    Mб    XnP    Ђs#    XnP    Ђs#                        `              X1	 џџX1	 џџ                                1	 џџ1	 џџ1	 џџ1	 џџЈ1	 џџЈ1	 џџИ1	 џџИ1	 џџ       N               fџџџџ        А 1	 џџ                                     1	 џџ 1	 џџ       81	 џџ81	 џџ                         Ћџџџџк       4; џџ        1	 џџ1	 џџ                        Ј1	 џџЈ1	 џџ        СV
м                                            Й#P    a     1	 џџ 1	 џџ    џџџџ                               81	 џџ81	 џџ                        y  y          
ѓ               ў!p                                                 р                              и1	 џџи1	 џџш1	 џџш1	 џџ                       1	 џџ1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџ`1	 џџ        :            НKoP    Mб    XnP    Ђs#    XnP    Ђs#                        `              Ш1	 џџШ1	 џџ                                ј1	 џџј1	 џџ1	 џџ1	 џџ1	 џџ1	 џџ(1	 џџ(1	 џџ                      fџџџџ         1	 џџ                                    1	 џџ1	 џџ       Ј1	 џџЈ1	 џџ                         Ћџџџџк       4; џџ        №1	 џџ№1	 џџ                        1	 џџ1	 џџ        ТV
м                                            Й#P    a    p1	 џџp1	 џџ    џџџџ                               Ј1	 џџЈ1	 џџ                        y  y    1	 џџ
ѓ               џ!p                                                 р                              H1	 џџH1	 џџX1	 џџX1	 џџ                       1	 џџ1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџа1	 џџ        ;            НKoP    Mб    XnP    Ђs#    XnP    Ђs#                        `              81	 џџ81	 џџ                                h1	 џџh1	 џџx1	 џџx1	 џџ1	 џџ1	 џџ1	 џџ1	 џџ                     fџџџџ        1	 џџ                                     	1	 џџ 	1	 џџ       	1	 џџ	1	 џџ                         Ћџџџџк       4; џџ        `	1	 џџ`	1	 џџ                        	1	 џџ	1	 џџ        УV
м                                            Й#P    a    р	1	 џџр	1	 џџ    џџџџ                               
1	 џџ
1	 џџ                        y  y  p1	 џџ
ѓ                $p       % p       ] p                         р                             И
1	 џџИ
1	 џџШ
1	 џџШ
1	 џџ 0                      №
1	 џџ№
1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџ@1	 џџ        <            НKoP    Mб    XnP    Ђs#    XnP    Ђs#                  0      `              Ј1	 џџЈ1	 џџ                                и1	 џџи1	 џџш1	 џџш1	 џџј1	 џџј1	 џџ1	 џџ1	 џџ       С              fџџџџ         1	 џџ                                    p1	 џџp1	 џџ       1	 џџ1	 џџ                         Ћџџџџк       4; џџ        а1	 џџа1	 џџ                        ј1	 џџј1	 џџ        ФV
м                                            Й#P    a    P1	 џџP1	 џџ    џџџџ                               1	 џџ1	 џџ                        y  y  р1	 џџ
ѓ               $p                                                 р                              (1	 џџ(1	 џџ81	 џџ81	 џџ                       `1	 џџ`1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџА1	 џџ        =            НKoP    Mб    XnP    Ђjц    XnP    Ђjц                        `              1	 џџ1	 џџ                                H1	 џџH1	 џџX1	 џџX1	 џџh1	 џџh1	 џџx1	 џџx1	 џџ       -               fџџџџ        p1	 џџ                                    р1	 џџр1	 џџ       ј1	 џџј1	 џџ                         Ћџџџџк       4; џџ        @1	 џџ@1	 џџ                        h1	 џџh1	 џџ        ХV
м                                            Й#P    a    Р1	 џџР1	 џџ    џџџџ                               ј1	 џџј1	 џџ                        y  y  P
1	 џџ
ѓ               ј!p                                                 р                              1	 џџ1	 џџЈ1	 џџЈ1	 џџ                       а1	 џџа1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџ 1	 џџ        3            НKoP    Mб    XnP    Ђs#    XnP    Ђs#                        `              1	 џџ1	 џџ                                И1	 џџИ1	 џџШ1	 џџШ1	 џџи1	 џџи1	 џџш1	 џџш1	 џџ       o               fџџџџ        р1	 џџ                                    P1	 џџP1	 џџ       h1	 џџh1	 џџ                         Ћџџџџк       4; џџ        А1	 џџА1	 џџ                        и1	 џџи1	 џџ        ЛV
м                                            Й#P    a    01	 џџ01	 џџ    џџџџ                               h1	 џџh1	 џџ                        y  y  Р1	 џџ
ѓ           '$p   p       a p        p       № p             р                             1	 џџ1	 џџ1	 џџ1	 џџ P                      @1	 џџ@1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџ1	 џџ        4            НKoP    Mб    XnP    Ђ    XnP    Ђ          0        P      `              ј1	 џџј1	 џџ                                (1	 џџ(1	 џџ81	 џџ81	 џџH1	 џџH1	 џџX1	 џџX1	 џџ       г              fџџџџ        P1	 џџ                                    Р1	 џџР1	 џџ       и1	 џџи1	 џџ                         Ћџџџџк       4; џџ         1	 џџ 1	 џџ                        H1	 џџH1	 џџ        МV
м                                            Й#P    a     1	 џџ 1	 џџ    џџџџ                               и1	 џџи1	 џџ                        y  y  01	 џџ
ѓ               і!p                                                 р                              x1	 џџx1	 џџ1	 џџ1	 џџ                       А1	 џџА1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџ 1	 џџ        1            НKoP    HCv&    й O            ю#P    ч4!                        `              h1	 џџh1	 џџ                                1	 џџ1	 џџЈ1	 џџЈ1	 џџИ1	 џџИ1	 џџШ1	 џџШ1	 џџ                      fџџџџ        Р1	 џџ                                    01	 џџ01	 џџ       H1	 џџH1	 џџ                         Ћџџџџк       4; џџ        1	 џџ1	 џџ                        И1	 џџИ1	 џџ        ЙV
м                                            Й#P    a    1	 џџ1	 џџ    џџџџ                               H1	 џџH1	 џџ                        y  y   1	 џџ
ѓ               у#H                                                                               ш1	 џџш1	 џџј1	 џџј1	 џџ                        1	 џџ 1	 џџэA             џџџџџџџџџџџџџџџџРpџџџџ р6 џџp1	 џџ        qї            НKoP    HCv&    оzЂO            ю#P    циѕ                        `              и1	 џџи1	 џџ                                1	 џџ1	 џџ1	 џџ1	 џџ(1	 џџ(1	 џџ81	 џџ81	 џџ                      fџџџџ        01	 џџ                                     1	 џџ 1	 џџ       И1	 џџИ1	 џџ                         Ћџџџџк       4; џџ         1	 џџ 1	 џџ                        (1	 џџ(1	 џџ        Z
м                            у#H            Ў#P    зИ/6    1	 џџ1	 џџ    џџџџ                               И1	 џџИ1	 џџ                      y  y  1	 џџ
ѓ               $p                                                 р                              X1	 џџX1	 џџh1	 џџh1	 џџ               