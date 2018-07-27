- libRocket also updates invisible elements. This leads to e.g. textures being requested from the renderer although the element is not visible.
- Changed
	"
	void Element::Update()
	{
		ReleaseElements(deleted_children);
		active_children = children;
		for (size_t i = 0; i < active_children.size(); i++)
			active_children[i]->Update();

		// Force a definition reload, if necessary.
		style->GetDefinition();

		scroll->Update();
		OnUpdate();
	}
	"
	to
	"
	void Element::Update()
	{
		if (isVisible())
		{
			ReleaseElements(deleted_children);
			active_children = children;
			for (size_t i = 0; i < active_children.size(); i++)
				active_children[i]->Update();

			// Force a definition reload, if necessary.
			style->GetDefinition();

			scroll->Update();
			OnUpdate();
		}
	}
	"
- Changed
	"
	bool Context::Render()
	{
		RenderInterface* render_interface = GetRenderInterface();
		if (render_interface == NULL)
			return false;

		// Update the layout for all documents in the root. This is done now as events during the
		// update may have caused elements to require an update.
		for (int i = 0; i < root->GetNumChildren(); ++i)
			root->GetChild(i)->UpdateLayout();
	"
	to
	"
	bool Context::Render()
	{
		RenderInterface* render_interface = GetRenderInterface();
		if (render_interface == NULL)
			return false;

		// Update the layout for all documents in the root. This is done now as events during the
		// update may have caused elements to require an update.
		for (int i = 0; i < root->GetNumChildren(); ++i)
		{
			Element* element = root->GetChild(i);
			if (element->isVisible())
			{
				element->UpdateLayout();
			}
		}
	"
- Changed
	"
	Element* Context::GetElementAtPoint(const Vector2f& point, const Element* ignore_element, Element* element)
	{
		// Update the layout on all documents prior to this call.
		for (int i = 0; i < GetNumDocuments(); ++i)
			GetDocument(i)->UpdateLayout();
	"
	to
	"
	Element* Context::GetElementAtPoint(const Vector2f& point, const Element* ignore_element, Element* element)
	{
		// Update the layout on all documents prior to this call.
		for (int i = 0; i < GetNumDocuments(); ++i)
		{
			Element* element = GetDocument(i);
			if (element->IsVisible())
			{
				element->UpdateLayout();
			}
		}
	"
