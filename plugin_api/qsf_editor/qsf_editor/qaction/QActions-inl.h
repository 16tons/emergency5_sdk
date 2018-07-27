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
		inline QActions::~QActions()
		{
			// Nothing to do in here
		}

		inline const std::vector<QAction*>& QActions::getQActionList() const
		{
			return mQActionList;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		inline bool QActions::onStartup()
		{
			// Nothing to do in here

			// Done
			return true;
		}

		inline void QActions::retranslateUi()
		{
			// Nothing to do in here
		}

		inline void QActions::onShutdown()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
