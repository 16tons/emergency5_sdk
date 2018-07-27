// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/**
		*  @brief
		*    Return the list of systems
		*/
		inline const std::vector<System*>& Plugin::getSystems() const
		{
			return mSystems;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline const char* Plugin::getName() const
		{
			return "QSF AI";
		}

		inline const char* Plugin::getVendor() const
		{
			return "Promotion Software GmbH";
		}

		inline const char* Plugin::getDescription() const
		{
			return "Quadriga Simulation Framework (QSF) Artificial Intelligence (AI)";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
