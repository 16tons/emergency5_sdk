// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/treatmentlogic/BaseTreatmentLogic.h"


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
	*    EMERGENCY 5 generic splint treatment logic
	*/
	class EM5_API_EXPORT SplintTreatmentLogic : public BaseTreatmentLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::SplintTreatmentLogic" game logic type ID

		// To identify the different splint types
		enum SplintType
		{
			SPLINTTYPE_ARM,
			SPLINTTYPE_LEG,
			SPLINTTYPE_BODY
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SplintTreatmentLogic();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SplintTreatmentLogic();

		/**
		*  @brief
		*    Initialize logic
		*/
		void init(uint64 targetEntityId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	protected:
		// Job update
		virtual void updateSimulation(const qsf::JobArguments& jobArguments) override;
		virtual	void startTreatment();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void detectSplintType();
		bool canTargetReceiveSplint();
		std::string getBandageNameForTarget();
		void attachSplint();
		void setVisualVitalSignToTarget();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mCanAttachSplint;
		SplintType	mSplintType;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SplintTreatmentLogic)
