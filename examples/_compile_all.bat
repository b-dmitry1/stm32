@echo off

rem Set your CPU name here
set "cpu=f401"


echo Target CPU: "%cpu%"

for /d %%d in (*) do (
  echo %%d
  cd %%d
  call _compile.bat
  if errorlevel 1 goto :end
  cd ..
)

:end
