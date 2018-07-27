After pasting something from the clipboard, the clipboard was no longer usable.

- Clipboard wasn't correct used and wasn't able to work with unicode strings
	"Rocket::Controls::Clipboard::Set()" never call GlobalLock() nor  GlobalUnlock() and used the wrong string format and string length to allocate memory
	Changed
	"
		void Clipboard::Set(const Core::WString& _content)
		{
			#if defined ROCKET_PLATFORM_WIN32
			if (GetHWND())
			{
				if (!OpenClipboard(GetHWND()))
					return;

				EmptyClipboard();

				Rocket::Core::String win32_content;
				_content.ToUTF8(win32_content);

				HGLOBAL clipboard_data = GlobalAlloc(GMEM_FIXED, win32_content.Length() + 1);
				strcpy_s((char*) clipboard_data, win32_content.Length() + 1, win32_content.CString());

				if (SetClipboardData(CF_TEXT, clipboard_data) == NULL)
				{
					CloseClipboard();
					GlobalFree(clipboard_data);
				}
				else
					CloseClipboard();
			}
			else
				content = _content;
			#else
			content = _content;
			#endif
		}
	"
	to
	"
		void Clipboard::Set(const Core::WString& _content)
		{
			#if defined ROCKET_PLATFORM_WIN32
			if (GetHWND())
			{
				if (!OpenClipboard(GetHWND()))
					return;

				EmptyClipboard();


				HGLOBAL clipboard_data = GlobalAlloc(NULL, (_content.Length() + 1) * 2);

				WCHAR* pData = (WCHAR*)GlobalLock(clipboard_data);
				for (int i = 0; i < _content.Length(); ++i)
				{
					pData[i] = _content[i];
				}

				pData[_content.Length()] = 0;

				if (SetClipboardData(CF_UNICODETEXT, clipboard_data) == NULL)
				{
					GlobalUnlock(clipboard_data);
				}

				CloseClipboard();
			}
			else
				content = _content;
			#else
			content = _content;
			#endif
		}
	"
