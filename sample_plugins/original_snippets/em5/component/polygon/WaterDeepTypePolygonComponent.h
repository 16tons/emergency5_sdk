// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/component/polygon/PolygonComponent.h>


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
	*    AI water polygon component class
	*/
	class WaterDeepTypePolygonComponent : public qsf::PolygonComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::WaterDeepTypePolygonComponent" unique component ID
		static const uint32 WATER_DEEP_TYPE;	///< "WaterDeepType" unique class property ID inside the class

		enum WaterDeepType
		{
			SHALLOW_WATER,
			MEDIUM_DEEP_WATER,
			DEEP_WATER
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
		inline explicit WaterDeepTypePolygonComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WaterDeepTypePolygonComponent();

		void setWaterDeepType(WaterDeepType waterDeepType);
		inline WaterDeepType getWaterDeepType() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true;}
		virtual void onComponentPropertyChange(const qsf::Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PolygonComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual const qsf::Color4& getPolygonDebugColor() const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Updates the values of the Collision component depending of the current water deep type
		void updateCollisionComponentValues() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		WaterDeepType	mWaterDeepType;


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
#include "em5/component/polygon/WaterDeepTypePolygonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::WaterDeepTypePolygonComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::WaterDeepTypePolygonComponent::WaterDeepType)
