- Build did not start under Windows 7 due to unresolved external symbol "CreateFile2"
- Solution as proposed at "http://stackoverflow.com/questions/17102770/using-the-windows-8-sdk-to-compile-for-windows-7"
- Found and fixed another bug. Changed
	"
	static BOOL MySetFilePointerEx(HANDLE hFile, LARGE_INTEGER pos, LARGE_INTEGER *newPos,  DWORD dwMoveMethod)
	{
	#ifdef IOWIN32_USING_WINRT_API
		return SetFilePointerEx(hFile, pos, newPos, dwMoveMethod);
	#else
		LONG lHigh = pos.HighPart;
		DWORD dwNewPos = SetFilePointer(hFile, pos.LowPart, &lHigh, FILE_CURRENT);
	"
	to
	"
	static BOOL MySetFilePointerEx(HANDLE hFile, LARGE_INTEGER pos, LARGE_INTEGER *newPos,  DWORD dwMoveMethod)
	{
	#ifdef IOWIN32_USING_WINRT_API
		return SetFilePointerEx(hFile, pos, newPos, dwMoveMethod);
	#else
		LONG lHigh = pos.HighPart;
		DWORD dwNewPos = SetFilePointer(hFile, pos.LowPart, &lHigh, dwMoveMethod);
	"
