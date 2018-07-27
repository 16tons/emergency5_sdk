Bullet 2.82 (svn r2704)
- License: "zlib"
- Download from: http://bulletphysics.org/ (or use the zip provided here)
- Apply the memory leak fix from http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=8006 ("Physics Simulation Forum • View topic - Memory leaking in Visual Studio 11.pdf")
 ... which basically means uncommenting the line #define BT_NO_PROFILE 1 at the top of LinearMath/btQuickprof.h

- Call CMake to create the Visual Studio 2015 (VC 14) setup for 32 and 64 bit separately in a different target directory
- After pressing configure, set
 - USE_MSVC_INCREMENTAL_LINKING to true
 - USE_MSVC_RUNTIME_LIBRARY_DLL to true
- Now press Generate
- Repeat the following for x86 and x64 solutions:
- Open the Solution in the target directory and build for Debug and Release
 You can manually only build the three projects LinearMath, BulletCOllision and BulletDynamics instead of the whole solution. 
- Fetch the following from the lib\Debug and lib\Release directories
 LinearMath.lib
 LinearMath_Debug.lib
 LinearMath_Debug.pdb
 BulletCollision.lib
 BulletCollision_Debug.lib
 bulletcollision_debug.pdb
 BulletDynamics.lib
 BulletDynamics_Debug.lib
 bulletdynamics_debug.pdb
- ... and copy them to the bullet lib directory in the QSF external subfolder for the respective solution (x86 / x64)