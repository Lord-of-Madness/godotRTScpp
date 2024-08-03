Requirements: see godot-cpp install guide

To run this project
1. clone it
2. update submodule godot-cpp (make sure the branch is 4.2)
3. run scons in the folder containing SConstruct.py. Alternatively on Windows run build.bat 

Note: On Linux the METHODADD makro must be adjusted so that "&funcName" is "&godot::className::funcName" since gcc doesn't like pointers to methods.

More detailed information can be found in the installation guide for godot-cpp: https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html
just replace the example project (src folder) with the contents of the Project1 folder 

godot-cpp repo: https://github.com/godotengine/godot-cpp/tree/4.2?tab=readme-ov-file

Output DLL will be placed in malyjir/Zapoctak/godot-rts/DLLs
This can be configured in SConstruct.py

Project is set up for Windows. For building on Linux and Mac see the godot-cpp install guide for differences.