// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    AI level of detail component
	*
	*  @remarks
	*    This component stores the values that govern whether an entity uses the QSF AI level of detail system.
	*    It is queried by other AI components and the level of detail to use is calculated based on its state.
	*    If some entity doesn't use this component it is expected to be always simulated at full detail.
	*    This is for example the case for player controlled entities.
	*    The currently planned rules are that an entity uses the full detail if ...
	*    - it is visible to a player
	*    - it is part of an event or an event is nearby
	*    The component acts as a service component to other components update and calculates data that it doesn't use itself.
	*    It therefore promotes the property change to interested other components.
	*    It is noncopyable like all components by inheritance.
	*
	*  @todo
	*    - TODO(vs) It would be more optimal regarding memory consumption if we could store the state in a vector<bool> for all entities
	*               and more optimal for performance if we could store the state with the update call.
	*    - TODO(co) Please review "ACTIVATION_PROPERTY_ID"/"UseFullDetail": Why does it have to be so complicated? Can't it just follow the simple property convention used everywhere where one can guess all the names?
	*/
	class AiLevelOfDetailComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "em5::AiLevelOfDetailComponent" unique component ID
		static const uint32 ACTIVATION_PROPERTY_ID;	///< Unique CAMP ID of the boolean activation property


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline AiLevelOfDetailComponent(qsf::Prototype* prototype);

		// Query whether to use full or reduced detail for the simulation
		inline bool useFullDetail() const;

		// Job system interface
		void update(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mUseFullDetail;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/base/AiLevelOfDetailComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::AiLevelOfDetailComponent)
