mkdir extbin

cd extbin
mkdir Win-Debug
mkdir Win-Release
mkdir Win-RelWithDebInfo
 
cd ..
mkdir OGLGAME

cd OGLGAME
mkdir bin

cd bin
mkdir Win-Debug
mkdir Win-Release
mkdir Win-RelWithDebInfo

cd ../../

cd ext/openal
cmake -S . -B debug-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER="gcc.exe" -DCMAKE_CXX_COMPILER="g++.exe" -GNinja
cmake --build debug-build --config Debug
copy /y debug-build/OpenAL32.lib ../../extbin/Win-Debug/OpenAL32.lib
copy /y debug-build/OpenAL32.dll ../../OGLGAME/bin/Win-Debug/OpenAL32.dll
copy /y debug-build/OpenAL32.pdb ../../OGLGAME/bin/Win-Debug/OpenAL32.pdb

cmake -S . -B release-build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER="gcc.exe" -DCMAKE_CXX_COMPILER="g++.exe" -GNinja
cmake --build release-build --config Release
copy /y release-build/OpenAL32.lib ../../extbin/Win-Release/OpenAL32.lib
copy /y release-build/OpenAL32.dll ../../OGLGAME/bin/Win-Release/OpenAL32.dll

cmake -S . -B reldbg-build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER="gcc.exe" -DCMAKE_CXX_COMPILER="g++.exe" -GNinja
cmake --build reldbg-build --config RelWithDebInfo
copy /y reldbg-build/OpenAL32.lib ../../extbin/Win-RelWithDebInfo/OpenAL32.lib
copy /y reldbg-build/OpenAL32.dll ../../OGLGAME/bin/Win-RelWithDebInfo/OpenAL32.dll
copy /y reldbg-build/OpenAL32.pdb ../../OGLGAME/bin/Win-RelWithDebInfo/

REM cleanup
rmdir debug-build /s /q
rmdir release-build /s /q
rmdir reldbg-build /s /q


PAUSE