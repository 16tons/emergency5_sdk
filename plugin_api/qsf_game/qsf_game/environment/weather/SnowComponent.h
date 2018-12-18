// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/component/Component.h>
#include <qsf/renderer/map/MovableObjectRenderingListener.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Snow component class
		*/
		class QSF_GAME_API_EXPORT SnowComponent : public Component, public MovableObjectRenderingListener
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::SnowComponent" unique component ID
			static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
			static const uint32 SNOW_AMOUNT;	///< "SnowAmount" unique class property ID inside the class


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
			inline explicit SnowComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~SnowComponent();

			/**
			*  @brief
			*    Return the snow amount
			*
			*  @return
			*    Snow amount
			*/
			inline float getSnowAmount() const;

			/**
			*  @brief
			*    Set the snow amount
			*
			*  @param[in] snowAmount
			*    Snow amount
			*/
			void setSnowAmount(float snowAmount);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual Component* getMovableObjectRenderingListenerComponent() override;
			virtual void onObjectRenderingStartup() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			inline virtual bool implementsOnComponentPropertyChange() const override;
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
			virtual void onSetActive(bool active) override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateMaterial(bool enable = true);
			void updateInternalSnowAmount() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			float mSnowAmount;	///< Snow amount
			// Internal
			bool mMaterialChanged;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/environment/weather/SnowComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::SnowComponent)
