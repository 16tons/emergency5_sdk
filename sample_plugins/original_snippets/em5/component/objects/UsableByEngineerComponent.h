// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>


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
	*    EMERGENCY 5 usable by engineer component
	*/
	class EM5_API_EXPORT UsableByEngineerComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::UsableByEngineerComponent" unique component ID

		enum Placement
		{
			HIGH,
			LOW
		};


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
		explicit UsableByEngineerComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UsableByEngineerComponent();

		/**
		*  @brief
		*    Initialization from event code
		*/
		void initWithUseTime(qsf::Time useTime);

		/**
		*  @brief
		*    Engineer needs different animations (standing or kneeling) depending on whether the object is placed high or low
		*/
		Placement getPlacement() const;
		void setPlacement(Placement placement);

		/**
		*  @brief
		*    Time it takes to use this object once (wait-time for engineer)
		*/
		qsf::Time getUseTime() const;
		void setUseTime(qsf::Time useTime);

		// Legacy: Only for CAMP property compatibility reasons
		float getUseTimeInSeconds() const;
		void setUseTimeInSeconds(float useTime);

		/**
		*  @brief
		*    Has it been used already
		*/
		bool getUsed() const;
		void setUsed(bool wasUsed);

		/**
		*  @brief
		*    Is it in use at the moment
		*/
		bool getIsInUse() const;
		void setIsInUse(bool isInUse);

		/**
		*  @brief
		*    Reset state of this component
		*/
		void cleanup();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Placement mPlacement;
		qsf::Time mUseTime;		///< Use time
		bool mUsed;				///< Has it been used already
		bool mIsInUse;			///< Is it in use at the moment


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::UsableByEngineerComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::UsableByEngineerComponent::Placement)
