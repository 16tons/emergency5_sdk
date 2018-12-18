// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GuiContext& GuiDocument::getGuiContext() const
	{
		return *mGuiContext;
	}

	inline Rocket::Core::ElementDocument* GuiDocument::getRocketCoreElementDocument() const
	{
		return mRocketCoreElementDocument;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	inline void GuiDocument::processEvent(const std::string&, Rocket::Core::Event&)
	{
		// Default implementation is empty
	}

	inline void GuiDocument::processViewportDimensionsChanged(int, int)
	{
		// Default implementation is empty
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
