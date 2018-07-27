- Used the wrong String format
	Rocket::Controls::WidgetTextInput::CopySelection() used the wrong String format so the length, witch was used to copy the selected part from the string, wasn't correct for unicode strings
	Changed
	"
		void WidgetTextInput::CopySelection()
		{
			const Core::String& value = GetElement()->GetAttribute< Rocket::Core::String >("value", "");
			Clipboard::Set(Core::String(value.Substring(selection_begin_index, selection_length)));
		}
	"
	to
	"
		void WidgetTextInput::CopySelection()
		{
			const Core::WString value = GetElement()->GetAttribute< Rocket::Core::String >("value", "");
			Clipboard::Set(value.Substring(selection_begin_index, selection_length));
		}
	"
	
	
	
	