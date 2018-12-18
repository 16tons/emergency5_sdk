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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkComponent;
}


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
		*    QSF game hidden component class
		*
		*  @remarks
		*    Entities with this component should not be visible. There are "hidden" from game logic, cause the entity is in a vehicle, building or invisible.
		*/
		class QSF_GAME_API_EXPORT HiddenComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::HiddenComponent" unique component ID

			enum HiddenType
			{
				HIDDENTYPE_DEFAULT,
				HIDDENTYPE_BLOCKED,
				HIDDENTYPE_BLOCKED_BUT_VISIBLE,		///< This state don't deactivate the rendering
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static void setHidden(Entity& entity, bool hide = true, HiddenType hiddenType = HiddenComponent::HIDDENTYPE_DEFAULT);
			static bool isHidden(const Entity& entity);


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
			explicit HiddenComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~HiddenComponent();

			/**
			*  @brief
			*    Return the hidden type
			*/
			HiddenType getHiddenType() const;

			/**
			*  @brief
			*    Set the hidden type
			*/
			void setHiddenType(HiddenType hiddenType);

			/**
			*  @brief
			*    Check whether or not the entity is visible
			*
			*  @remarks
			*    This is "false" in all cases that don't actually make the entity invisible, e.g. when using type "HIDDENTYPE_BLOCKED_BUT_VISIBLE"
			*/
			bool isEntityVisible() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetSimulating(bool simulating) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setVisibility(bool active);
			void setVisibilityRecursiveForChildren(bool show, qsf::LinkComponent& linkComponent);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			HiddenType mHiddenType;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::HiddenComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::HiddenComponent::HiddenType)
