// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/gui/hud/overlay/OverlayWindowManager.h"

#include <qsf/component/Component.h>
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MeshComponent;
}
namespace Rocket
{
	namespace Core
	{
		class ElementDocument;
	}
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
	*    EMERGENCY 5 overlay component base class
	*/
	class EM5_API_EXPORT OverlayComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class OverlayManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::OverlayComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OverlayComponent();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*  @param[in] overlayWindowManager
		*    Overlay window manager to use
		*/
		explicit OverlayComponent(qsf::Prototype* prototype, OverlayWindowManager& overlayWindowManager);

		void checkEmptyStateChanged();
		void makeDirty();

		void avoidDelayedDestruction();
		void triggerDelayedDestruction();

		void onPreCompositorWorkspaceUpdate(const Ogre::Camera& ogreCamera);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::OverlayComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Check whether or not there is any content to show inside the overlay
		*/
		inline virtual bool isContentEmpty() const;

		inline virtual void updateContent(Rocket::Core::ElementDocument& document);

		virtual const glm::vec3& getContentWorldSpaceOffset() const = 0;
		virtual const glm::vec2& getContentWindowSize() const = 0;
		virtual const glm::vec2& getContentWindowOffset() const = 0;

		inline virtual void updateAnimation(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera) override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::WeakPtr<OverlayWindowManager> mOverlayWindowManager;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool calculateScreenPosition(const qsf::MeshComponent& meshComponent, const Ogre::Camera& ogreCamera, glm::vec2& screenPosition) const;
		void onDelayedDestruction(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct DoubleLinkedListEntry
		{
			bool mIsRegistered;
			OverlayComponent* mPrevious;
			OverlayComponent* mNext;
			DoubleLinkedListEntry() : mIsRegistered(false), mPrevious(nullptr), mNext(nullptr) {}
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		  mLastIsContentEmptyState;
		qsf::JobProxy mJobProxy;			///< Regular animation job proxy, for updates once a frame
		qsf::JobProxy mDestructionJobProxy;	///< Job proxy triggering once for delayed destruction of this component
		bool		  mScreenRendered;		///< Indicates if a screen rendering was done (render state = IRS_NONE)
		glm::vec2	  mScreenPosition;

		// Registration in manager
		qsf::WeakPtr<qsf::MeshComponent> mVisibleMeshComponent;
		DoubleLinkedListEntry mVisibleOverlayComponentsEntry;


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
#include "em5/component/overlay/OverlayComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::OverlayComponent)
