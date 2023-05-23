@echo off

for /d %%d in (*) do (
  echo %%d
  cd %%d
  call _compile.bat
  if errorlevel 1 goto :end
  cd ..
)

:end
