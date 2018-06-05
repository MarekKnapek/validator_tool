echo on

:call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat"

if exist ..\build rmdir /S /Q ..\build
mkdir ..\build

mkdir ..\build\debug_dynamic
cl.exe /nologo /EHsc /DUNITY_BUILD /DTYPE_ERASER_WANT_TESTS /DDEBUG /D_DEBUG /GA /GL- /GS /Gs0 /guard:cf /Gw- /MDd /Od /permissive- /RTCsu /sdl /std:c++latest /W3 /guard:cf /Fe..\build\debug_dynamic\validator_tool.exe validator_tool_unity.cpp /link /DYNAMICBASE /guard:cf
del /F /Q validator_tool_unity.obj

mkdir ..\build\debug_static
cl.exe /nologo /EHsc /DUNITY_BUILD /DTYPE_ERASER_WANT_TESTS /DDEBUG /D_DEBUG /GA /GL- /GS /Gs0 /guard:cf /Gw- /MTd /Od /permissive- /RTCsu /sdl /std:c++latest /W3 /guard:cf /Fe..\build\debug_static\validator_tool.exe validator_tool_unity.cpp /link /DYNAMICBASE /guard:cf
del /F /Q validator_tool_unity.obj

mkdir ..\build\release_dynamic
cl.exe /nologo /EHsc /DUNITY_BUILD /DTYPE_ERASER_WANT_TESTS /DNDEBUG /D_NDEBUG /GA /GL /GS- /Gs4096 /guard:cf- /Gw /MD /O2 /permissive- /std:c++latest /W3 /Fe..\build\release_dynamic\validator_tool.exe validator_tool_unity.cpp /link /DYNAMICBASE
del /F /Q validator_tool_unity.obj

mkdir ..\build\release_static
cl.exe /nologo /EHsc /DUNITY_BUILD /DTYPE_ERASER_WANT_TESTS /DNDEBUG /D_NDEBUG /GA /GL /GS- /Gs4096 /guard:cf- /Gw /MT /O2 /permissive- /std:c++latest /W3 /Fe..\build\release_static\validator_tool.exe validator_tool_unity.cpp /link /DYNAMICBASE
del /F /Q validator_tool_unity.obj

..\build\debug_dynamic\validator_tool.exe /tests & echo %errorlevel%
..\build\debug_static\validator_tool.exe /tests & echo %errorlevel%
..\build\debug_static\validator_tool.exe /tests & echo %errorlevel%
..\build\release_dynamic\validator_tool.exe /tests & echo %errorlevel%
