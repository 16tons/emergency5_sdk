// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/worldModel/dynamicUpdate/DynamicCollisionConverterComponent.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class PersonComponent;
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
	*    Specific variant of the QSF AI component that tracks for an entity whether it should be treated as a static or a dynamic obstacle.
	*    The EM5 specific rules are based on the definition that some entities should move out of the way when approached.
	*    So these shouldn't count as obstacles even if they are standing still atm.
	*    Although this reasoning motivated the creation of a EM5 specific variant, until the move out of the way logic is not fully implemented, the behavior is actually the same.
	*/
	class DynamicCollisionConverterComponent : public qsf::ai::DynamicCollisionConverterComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::DynamicCollisionConverterComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		DynamicCollisionConverterComponent(qsf::Prototype* prototype);


	//[-------------------------------------------------------]
	//[ Public overridden methods                             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Overloaded startup to initialize weak pointers to other components.
		*/
		virtual bool onStartup() override;

		/**
		*  @brief
		*    Overloaded version that considers that civil persons usually try to avoid themselves.
		*/
		virtual bool shouldBeTreatedAsStatic() const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// specific methods for some types of entities that require specific rules
		//@{
		bool shouldCivilPersonBeTreatedAsStatic() const;
		bool shouldCivilCarBeTreatedAsStatic() const;
		//@}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<PersonComponent> mPersonComponent;	//< Optional pointer to the person component of the entity. May legally be invalid (nullptr)


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::DynamicCollisionConverterComponent)
