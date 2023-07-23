import sys
import subprocess
import re

cmake = r'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
msbuild = r'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBuild.exe'

proc_run = subprocess.run([cmake, 'out/build/x64-Debug/'], capture_output=True, text=True)
output = proc_run.stdout.strip()
# print(output)

proc_run = subprocess.run([msbuild, 'out/build/x64-Debug/pomelo.sln', '-t:Clean'], capture_output=True, text=True)
output = proc_run.stdout.strip()
# print(output)

proc_run = subprocess.run([msbuild, 'out/build/x64-Debug/pomelo.sln'], capture_output=True, text=True)
output = proc_run.stdout.strip()
# print(output)

EXCLUDE_LIST = [
    '"S:\\projects\\pomelo\\desktop\\out\\build\\x64-Debug\\'
    'C4514',
    'C4820',
    'C5045',
    'LNK4098',
    '\\glm',
    'pomelotest.vcxproj',
    'winbase.h',
    'winioctl.h',
]

i = 0
for line in output.split("\n"):
    if not line.strip():
        continue

    exclude = False
    for e in EXCLUDE_LIST:
        if e in line:
            exclude = True
            break

    if exclude:
        continue

    i += 1
    print(f'{i}: {line}')
