// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EntityHelper::EntityHelper(Entity& entity) :
		mEntity(entity)
	{
		// Nothing to do in here
	}

	inline EntityHelper::~EntityHelper()
	{
		// Nothing to do in here
	}

	inline Entity& EntityHelper::getEntity() const
	{
		return mEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
