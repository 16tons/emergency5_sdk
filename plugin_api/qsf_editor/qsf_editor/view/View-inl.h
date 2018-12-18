// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline View::~View()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline bool View::wasHiddenDueClose() const
		{
			return mHiddenDueClose;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		inline void View::retranslateUi()
		{
			// Nothing to do in here
		}

		inline bool View::preChangeVisibility(bool visible)
		{
			// Nothing special to do in here, just return the given value
			return visible;
		}

		inline void View::changeVisibility(bool visible)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
