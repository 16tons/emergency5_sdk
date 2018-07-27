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
#include <qsf/message/MessageProxy.h>
#include <qsf/time/Time.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
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
	*    EMERGENCY 5. The person magnet will make every (alife, healthy) civilian in it's area follow him.
	*/
	class EM5_API_EXPORT PersonMagnetComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::PersonMagnetComponent" unique component ID


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
		explicit PersonMagnetComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PersonMagnetComponent();

		/**
		*  @brief
		*    Enable the magnet visualization when the unit is selected by the player
		*/
		void enableVisualization(float visualizationRadius);
		/**
		*  @brief
		*    Disable the magnet visualization when the unit is selected by the player
		*/
		void disableVisualization();
		bool isVisualizationEnabled() const;
		float getVisualizationRadius() const;

		void enableMagnet(float influenceRadius);
		void disableMagnet();
		bool isMagnetEnabled() const;
		float getInfluenceRadius() const;

		void setCarrierEntityId(uint64 entityId);
		uint64 getCarrierEntityId() const;

		void setMaxFollowers(uint32 maxFollowers);
		uint32 getMaxFollowers() const;

		/**
		*  @brief
		*    Check how far away the follow should be from the magnet entity
		*/
		glm::vec3 getFollowOffset(uint64 followerEntityId, bool localCoordinates, bool recalculateOffset);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments& jobArguments);
		void cancelMagnet(const qsf::MessageParameters& parameters);
		void onChangeSelection(const qsf::MessageParameters& parameters);

		/**
		* Implemented as unregistering the component and registering again with the time delay defined as parameter
		*/
		void setUpdateFrequency(qsf::Time updateInterval);

		glm::vec3 updateFollowerOffset(const qsf::TransformComponent& magnetTransform, const qsf::TransformComponent& followerTransform) const;

		void switchGroundSpot(bool activate);
		void removeFollowers();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				mMaxFollowers;
		float				mInfluenceRadius;
		float				mPlaceDistMin;
		float				mPlaceDistMax;
		float				mTalkPause;
		qsf::JobProxy		mUpdateJobProxy;
		qsf::MessageProxy	mOnInjuredProxy;
		qsf::MessageProxy	mOnContaminatedProxy;
		qsf::MessageProxy	mOnEnterVehicleProxy;
		qsf::MessageProxy	mOnSelectionChangeProxy;
		boost::container::flat_map<uint64, glm::vec3>	mFollowerOffsets;	///< Key is follower entity ID, value is relative offsets they should keep. Map also helps to remember followers (so if you ever kick out offsets still use a set to keep follower entity-ID's).
		qsf::Time			mTimeLastAnnouncement;							///< Last time a megaphone announcement was made
		uint64				mMagnetCarrier;
		float				mMaxForwardSpeedBackup;
		float				mVisualizationRadius;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonMagnetComponent)
