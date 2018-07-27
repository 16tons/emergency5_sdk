// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline Dialog::~Dialog()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		inline void Dialog::readSettings(QSettings&)
		{
			// Nothing to do in here
		}

		inline void Dialog::writeSettings(QSettings&)
		{
			// Nothing to do in here
		}

		inline void Dialog::retranslateUi()
		{
			// Nothing to do in here
		}

		inline bool Dialog::preChangeVisibility(bool visible)
		{
			// Nothing special to do in here, just return the given value
			return visible;
		}

		inline void Dialog::changeVisibility(bool visible)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
