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
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/** EMERGENCY 5 Inhabitant component.
	* Attached to (civilian) entities that were spawned by an InhabitantSpawnComponent.
	* This component takes care that the entity tries to reach its destination
	* if that destination is valid and the entity is not injured.
	* In that cases this component takes care that the entity tries to return to
	* its home (= the entity with the InhabitantSpawnComponent that spawned this entity).
	*/
	class EM5_API_EXPORT InhabitantComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "em5::InhabitantComponent" unique component ID
		static const uint32 ACTION_PRIORITY;		///< "ActionPriority" unique class property ID inside the class
		static const uint32 DESTINATION_ENTITY_ID;	///< "DestinationEntityId" unique class property ID inside the class

		enum InhabitantMode
		{
			GOTO_DESTINATION=0,
			GOTO_HOME,
			UNDEFINED
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit InhabitantComponent(qsf::Prototype* prototype);
		virtual ~InhabitantComponent();

		virtual void serialize(qsf::BinarySerializer& serializer) override;

		// Get / set the priority of the actions pushed
		//@{
		uint32 getActionPriority() const;
		void setActionPriority(uint32 priority);
		//@}

		// Get / set the home's entity id
		//@{
		uint64 getHomeEntityId() const;
		void setHomeEntityId(uint64 entityId);
		//@}

		// Get / set the destination's entity id
		//@{
		uint64 getDestinationEntityId() const;
		void setDestinationEntityId(uint64 entityId);
		//@}

		// Get / set the inhabitant's current mode
		//@{
		InhabitantMode getInhabitantMode() const;
		void setInhabitantMode(InhabitantMode mode);
		//@}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Command the entity to go to the supplied target-entity.
		bool goToEntity(uint64 targetEntityId);

		// Main processing method. Triggers actions depending on mode and situation.
		void updateJob(const qsf::JobArguments& jobArguments);

		// Implemented as unregistering the component and registering again with the time delay defined as parameter, so that the update-interval can be changed easily on demand.
		void setUpdateFrequency(qsf::Time updateInterval);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mActionPriority;			///< The priority of the actions pushed
		uint64 mDestinationEntityId;

		uint64		   mHomeEntityId;
		InhabitantMode mInhabitantMode;
		qsf::JobProxy  mUpdateJobProxy;


	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/person/InhabitantComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::InhabitantComponent)
