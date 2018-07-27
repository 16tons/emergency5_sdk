// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline VehicleComponent::VehicleComponent(Prototype* prototype) :
			Component(prototype),
			PrototypeContainer(*prototype)
		{
			// Nothing to do in here
		}

		inline VehicleComponent::~VehicleComponent()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
