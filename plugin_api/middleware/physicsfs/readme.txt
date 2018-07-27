PhysicsFS 2.0.3
- License: zlib
- Used by the QSF file system
- Download from: http://icculus.org/physfs/
- Build with VisualStudio 2015 and used the following support options:
	ZIP support: enabled
	7zip support: enabled
	GRP support: disabled
	WAD support: disabled
	HOG support: disabled
	MVL support: disabled
	QPAK support: disabled
  -> We don't need most of them, don't know if there are e.g. legal issues with WAD or QPAK in commercial products
- Static library includes some 7zip and lzma symbols
  -> will cause duplicate symbol linker errors in case we decide to link those libraries ourselves
- Static library no longer includes some zlib symbols
  -> use MSVC solution from physfs-2.0.3-qsf.7z to build it 