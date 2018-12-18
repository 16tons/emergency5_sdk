// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline MaterialAccessComponent::MaterialAccessComponent(Prototype* prototype) :
			Component(prototype)
		{
			// Nothing here
		}

		inline MaterialAccessComponent::~MaterialAccessComponent()
		{
			// Nothing to do in here
		}

		inline const std::string& MaterialAccessComponent::getPropertyName() const
		{
			return mPropertyName;
		}

		inline const std::string& MaterialAccessComponent::getPropertyValue() const
		{
			return mPropertyValue;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
