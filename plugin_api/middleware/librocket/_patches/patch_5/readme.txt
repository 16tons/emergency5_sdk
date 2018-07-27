- libRocket doesn't reset properly the active (psudo) state of an element when with the right mouse button inside an element is clicked
- Changed
	"
	void Element::ProcessEvent(Event& event)
  {
    if (event == MOUSEDOWN && IsPointWithinElement(Vector2f(event.GetParameter< float >("mouse_x", 0), event.GetParameter< float >("mouse_y", 0))))
      SetPseudoClass("active", true);

    if (event == MOUSESCROLL)
	"
	to
	"
	void Element::ProcessEvent(Event& event)
  {
    if (event == MOUSEDOWN || event == MOUSEUP)
    {
      // Update active state on mouse down and up event
      bool active = false;
      if (IsPointWithinElement(Vector2f(event.GetParameter< float >("mouse_x", 0), event.GetParameter< float >("mouse_y", 0))))
      {
        active = event == MOUSEDOWN;
      }
      SetPseudoClass("active", active);
    }
    else if (event == MOUSESCROLL)
	"
- "Element::RemoveEventListener": Added "all" event
