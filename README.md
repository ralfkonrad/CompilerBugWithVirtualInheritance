# Visual Studio 2022 Segmentation Fault with Virtual Inheritance in `-std:c++11` and `-std:c++14`

---

_This contains the source code for issue 
https://developercommunity.visualstudio.com/t/Visual-Studio-2022-Segmentation-Fault-wi/10263539_

---

Using the code underneath causes a segmentation fault with `-std:c++11` and `-std:c++14` but not with `-std:c++17`
and `-std:c++20`.

The essential part is the `virtual` inheritance here `class Parent : public virtual Abstract {};` and the call of the
constructor `explicit ChildWithProblem(const std::shared_ptr<Parent>& parent)`.

It is also tested against `ubuntu-latest` `gcc` compiler without warnings and errors
and also VS 2019 (same behaviour as VS 2022).

```c++
#include <iostream>
#include <memory>

class Abstract {
  public:
    virtual ~Abstract() = default;
};

class Parent : public virtual Abstract {};

class ChildWithProblem : public Parent {
  public:
    ChildWithProblem() = default;
    explicit ChildWithProblem(const std::shared_ptr<Parent>& parent)
    : ChildWithProblem(makeChild(parent)) {}

  private:
    static ChildWithProblem makeChild(const std::shared_ptr<Parent>& parent) {
        std::cout << "\t Casting ChildWithProblem..." << std::endl;
        auto child = std::dynamic_pointer_cast<ChildWithProblem>(parent);
        std::cout << "\t Done..." << std::endl;
        return {};
    }
};

int main() {
    try {
        std::cout << "Running ChildWithProblem..." << std::endl;
        auto childWithProblem = std::make_shared<ChildWithProblem>();
        auto copiedChildWithProblem = ChildWithProblem(childWithProblem);
    } catch (...) {
        std::cout << "We have an error..." << std::endl;
    }
}
```

However, using `-std:c++17` and `-std:c++20` results in a warning using `/W4`:

```
.\compilerbug.cpp(29): warning C4100: '$initVBases': unreferenced formal parameter
```

The error can be avoided by changing the return type
`static ChildWithProblem makeChild(const std::shared_ptr<Parent>& parent)` to
`static std::shared_ptr<ChildWithNoProblem> makeChild(const std::shared_ptr<Parent>& parent)`

```c++
class ChildWithNoProblem : public Parent {
  public:
    ChildWithNoProblem() = default;
    explicit ChildWithNoProblem(const std::shared_ptr<Parent>& parent)
    : ChildWithNoProblem(*makeChild(parent)) {}

  private:
    static std::shared_ptr<ChildWithNoProblem> makeChild(const std::shared_ptr<Parent>& parent) {
        std::cout << "\t Casting ChildWithNoProblem..." << std::endl;
        auto child = std::dynamic_pointer_cast<ChildWithNoProblem>(parent);
        std::cout << "\t Done..." << std::endl;
        return std::make_shared<ChildWithNoProblem>();
    }
};
```

---

### Commandline used to create the segmentation fault:

```
> cl /Bv
Microsoft (R) C/C++ Optimizing Compiler Version 19.34.31937 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

Compiler Passes:
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\cl.exe:        Version 19.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\c1.dll:        Version 19.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\c1xx.dll:      Version 19.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\c2.dll:        Version 19.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\c1xx.dll:      Version 19.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\link.exe:      Version 14.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\mspdb140.dll:  Version 14.34.31937.0
 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.34.31933\bin\HostX64\x64\1033\clui.dll: Version 19.34.31937.0

> cmake --build .\cmake-build-release-visual-studio\ -v
[1/2] C:\PROGRA~1\MIB055~1\2022\PROFES~1\VC\Tools\MSVC\1434~1.319\bin\Hostx64\x64\cl.exe  /nologo /TP   /DWIN32 /D_WINDOWS /GR /EHsc /O2 /Ob2 /DNDEBUG -MD /W4 -std:c++14 /showIncludes /FoCMakeFiles\compilerbug.dir\compilerbug.cpp.obj /FdCMakeFiles\compilerbug.dir\ /FS -c E:\Users\ralf.eckel\cpp\CompilerBugWithVirtualInheritance\compilerbug.cpp
[2/2] cmd.exe /C "cd . && cmake.exe -E vs_link_exe --intdir=CMakeFiles\compilerbug.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x64\mt.exe --manifests  -- C:\PROGRA~1\MIB055~1\2022\PROFES~1\VC\Tools\MSVC\1434~1.319\bin\Hostx64\x64\link.exe /nologo CMakeFiles\compilerbug.dir\compilerbug.cpp.obj  /out:compilerbug.exe /implib:compilerbug.lib /pdb:compilerbug.pdb /version:0.0 /machine:x64 /INCREMENTAL:NO /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib && cd ."

> ctest.exe --extra-verbose
Testing started at 12:17 ...
UpdateCTestConfiguration  from :./cmake-build-release-visual-studio/DartConfiguration.tcl
UpdateCTestConfiguration  from :./cmake-build-release-visual-studio/DartConfiguration.tcl
Test project ./cmake-build-release-visual-studio
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end

1: Test command: .\cmake-build-release-visual-studio\compilerbug.exe
1: Working Directory: ./cmake-build-release-visual-studio
1: Test timeout computed to be: 10000000
1: Running ChildWithNoProblem...
1: 	 Casting ChildWithNoProblem...
1: 	 Done...
1: Running ChildWithProblem...
1: 	 Casting ChildWithProblem...
Exception: SegFault
```
