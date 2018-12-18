// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/fire/component/HydrantComponent.h"
#include "em5/fire/component/simplefirehose/SimpleFireHoseComponent.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>


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
	*    Fireman logic
	*/
	class EM5_API_EXPORT FiremanLogic : public CarryPersonLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::FiremanLogic" game logic type ID

		static const std::string ASSET_ABC_SUIT;
		static const std::string ASSET_DUST_MASK;

		enum Clothing
		{
			CLOTHING_DEFAULT,
			CLOTHING_ABC_SUIT,
			CLOTHING_DUST_MASK
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		FiremanLogic();

		/**
		*  @brief
		*    Return pulling person entity ID
		*
		*  @return
		*    ID of the pulling entity
		*/
		// TODO(mz): Pulling persons belongs in its own logic
		uint64 getPullingPersonId() const;

		/**
		*  @brief
		*    Set pulling person entity ID
		*
		*  @param[in] id
		*    ID of the pulling entity
		*/
		// TODO(mz): Pulling persons belongs in its own logic
		void setPullingPersonId(uint64 id);

		/**
		*  @brief
		*    Check if the fireman is connected to a hydrant
		*
		*  @return
		*    Returns "true" when the fireman is connected to a hydrant; "false" otherwise
		*/
		bool isConnectedToHydrant() const;

		/**
		*  @brief
		*    Return the hydrant entity
		*
		*  @return
		*    The hydrant entity, or a null pointer if there is none connected
		*/
		qsf::Entity* getConnectedHydrant() const;

		/**
		*  @brief
		*    Connect to a hydrant
		*/
		void connectToHydrant(qsf::Entity& hydrant);

		/**
		*  @brief
		*    Disconnect from a hydrant
		*/
		void disconnectFromHydrant();

		/**
		*  @brief
		*    Return the connected fire hose component
		*
		*  @return
		*    The fire hose component entity, or a null pointer if there is none connected
		*/
		SimpleFireHoseComponent* getFireHose() const;

		void createFireHose();
		void destroyFireHose();

		/**
		*  @brief
		*    Performs the automatism, internally called after all checks are passed
		*
		*  @note
		*    Called from "ExtinguishFireAction"
		*/
		void performAutomatism();

		/**
		*  @brief
		* 	 Direct swapping of the mesh to the ABC suit; no actions
		*/
		Clothing getActiveClothing() const;

		/**
		*  @brief
		* 	Set another clothing mesh
		*/
		void setActiveClothing(Clothing clothing, bool changeMesh = true);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetDebug(bool debug) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);
		void updateAnimation(const qsf::JobArguments& jobArguments);
		void postInitialize(const qsf::JobArguments& jobArguments);

		void onReceiveHeatEnergy(const qsf::MessageParameters& parameters);
		void updateHydrantVisualisation();
		void performExtinguisherAutomatism();
		void performExtinguishWithHoseAutomatism();
		void performPullObjectFromPersonAutomatism();
		void performRescueCutterAutomatism();
		void performCuttingblockingTreesAutomatism();
		void performDecontaminatePersonAutomatism();

		void onPersonInjured(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity>				mPullingPersonEntity;
		qsf::Time								mTimeWaiting;

		// Hydrant, fire hose
		qsf::WeakPtr<HydrantComponent>			mConnectedHydrant;	///< The connected hydrant component, can be invalid
		qsf::WeakPtr<SimpleFireHoseComponent>	mConnectedFireHose;	///< The connected fire hose, can be invalid

		// Debug proxies for debug draw
		qsf::DebugDrawProxy	mDebugDrawProxyRange;	///< Only for debug
		qsf::DebugDrawProxy	mDebugDrawProxyHose;	///< Only for debug

		// Job proxies for regular update
		qsf::JobProxy	mAnimationJobProxy;		///< Regular animation job proxy
		qsf::JobProxy	mSimulationJobProxy;	///< Regular simulation job proxy
		qsf::JobProxy	mFirstUpdateJobProxy;	///< Simulation job proxy to be called just once after initialization

		// Contamination
		Clothing			mActiveClothing;			///< Which clothing mesh is the firefighter wearing
		qsf::AssetProxy		mAssetDefaultSuit;			///< Suit when not wearing an ABC suit
		qsf::MessageProxy	mMessageProxyInjured;		///< Get message when fireman gets injured
		qsf::MessageProxy	mHeatEnergyProxy;			///< Squad receives heat energy
		qsf::MessageProxy	mContaminationAreaProxy;	///< Squad receives contamination
		qsf::Time			mTimeRemoveDustMask;		///< Increase timer when not receiving fire energy and idling while wearing an Dust mask


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FiremanLogic)
