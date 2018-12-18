// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>
#include <qsf/component/Component.h>
#include <qsf/reflection/type/CampGlmVec3.h>
#include <qsf/reflection/type/CampGlmQuat.h>


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
		*    Use this component to attach the owning entity to a bone of its parent
		*/
		class QSF_GAME_API_EXPORT BoneLinkComponent : public Component, public JobProxyMovableObjectRenderingListener
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::BoneLinkComponent" unique component ID
			static const uint32 BONE_NAME;		///< "BoneName" unique property ID
			static const uint32 LOCAL_POSITION;	///< "LocalPosition" unique property ID
			static const uint32 LOCAL_ROTATION;	///< "LocalRotation" unique property ID


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
			inline explicit BoneLinkComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~BoneLinkComponent();

			/**
			*  @brief
			*    Return the name of the bone to attach to
			*/
			inline const std::string& getBoneName() const;

			/**
			*  @brief
			*    Set the name of the bone to attach to, inside the linked parent
			*/
			void setBoneName(const std::string& boneName);

			/**
			*  @brief
			*    Set/get the offset of the local position
			*/
			inline const glm::vec3& getLocalPosition() const;
			void setLocalPosition(const glm::vec3& localPosition);

			/**
			*  @brief
			*    Set/get the offset on the local rotation / orientation
			*/
			inline const glm::quat& getLocalRotation() const;
			void setLocalRotation(const glm::quat& localRotation);

			/**
			*  @brief
			*    Refreshes the bone link, shouldn't be needed
			*/
			void refresh();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup() override;
			virtual void onShutdown() override;

			inline virtual bool implementsOnComponentPropertyChange() const override;
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual Component* getMovableObjectRenderingListenerComponent() override;	// The component we use to check if we are visible
			virtual void onObjectRenderingStartup() override;
			virtual void onObjectRenderingShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected qsf::BoneLinkComponent methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void refreshBoneLink();
			virtual void destroyBoneLink();


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			std::string	mBoneName;				///< Name of the bone to attach to
			glm::vec3	mLocalPosition;			///< Local position offset relative to the bone
			glm::quat	mLocalRotation;			///< Local rotation offset relative to the bone
			uint64		mOldParentPrototypeId;	///< ID of the prototype for the last created link

			// Internal
			bool mObjectRendered;


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
#include "qsf_game/component/base/BoneLinkComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::BoneLinkComponent)
