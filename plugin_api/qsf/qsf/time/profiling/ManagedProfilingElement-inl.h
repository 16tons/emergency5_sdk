// Copyright (C) 2012-2017 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline uint32 ManagedProfilingElement::getId() const
		{
			return mName.getHash();
		}

		inline const std::string& ManagedProfilingElement::getName() const
		{
			return mName.getName();
		}

		inline uint32 ManagedProfilingElement::getParentId() const
		{
			return mParentName.getHash();
		}

		inline const std::string& ManagedProfilingElement::getParentName() const
		{
			return mParentName.getName();
		}


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf
#endif
