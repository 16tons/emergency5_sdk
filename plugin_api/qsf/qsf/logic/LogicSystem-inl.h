// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LogicSystem::LogicSystem(const char* name) :
		mHashedId(StringHash(name)),
		mName(name)
	{
		// Nothing here
	}

	inline LogicSystem::~LogicSystem()
	{
		// Nothing here
	}

	inline uint32 LogicSystem::getId() const
	{
		return mHashedId;
	}

	inline bool LogicSystem::isSimulating() const
	{
		return mInternalFlags.isSet(SIMULATING_MODE);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* LogicSystem::getName() const
	{
		return mName;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
