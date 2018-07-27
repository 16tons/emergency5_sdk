// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>

#include <vector>


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
	*    EMERGENCY 5 bury component class; given to the entity how burys other entities
	*/
	class EM5_API_EXPORT BuryComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::BuryComponent" unique component ID


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
		explicit inline BuryComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BuryComponent();

		/**
		*  @brief
		*    Get if the entity is visible or invisible for the player. Don't mix with the "isHidden" function from qsf::Component
		*/
		inline bool getIsHidden() const;
		inline void setIsHidden(bool isHidden);

		inline bool getDraggable() const;
		inline void setDraggable(bool draggable);

		inline bool getCutableBySaw() const;
		inline void setCutableBySaw(bool cutableBySaw);

		bool isBlocked() const;
		void setBlocked(bool blocked);

		void unlinkAndShowBuriedEntities();

		void buryEntity(qsf::Entity& entity);
		void showCuttableBySawIcon(bool enable);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;	///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void showBuriedEntities();
		void hideBuriedEntities();
		void setBuriedEntityHiddenType(qsf::Entity& buriedEntity) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			mIsHidden;
		bool			mDraggable;		///< "true" if the firefighter can drag/pull the object by hand
		bool			mCutableBySaw;	///< "true" if the firefighter can cut and destroy the object by saw
		bool			mIsBlocked;		///< "true" if this object can't be interacted with right now (maybe someone has to switch out an ebox before or similar)
		EntityIdArray	mBuriedEntities;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, BuryComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface BuriedEntitiesArray;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/objects/BuryComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuryComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::BuryComponent::EntityIdArrayInterface)
