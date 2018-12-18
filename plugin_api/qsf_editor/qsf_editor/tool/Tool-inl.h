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
		inline Tool::~Tool()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		inline void Tool::readSettings(QSettings&)
		{
			// Nothing to do in here
		}

		inline void Tool::writeSettings(QSettings&)
		{
			// Nothing to do in here
		}

		inline bool Tool::onStartup(ToolboxView&)
		{
			// Nothing to do in here

			// Done
			return true;
		}

		inline void Tool::retranslateUi(ToolboxView&)
		{
			// Nothing to do in here
		}

		inline void Tool::onShutdown(ToolboxView&)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
