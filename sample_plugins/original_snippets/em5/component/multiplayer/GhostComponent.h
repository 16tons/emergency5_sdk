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
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>


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
	*    Ghost component
	*/
	class EM5_API_EXPORT GhostComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::GhostComponent" unique component ID
		static const uint32 RENDER_QUEUE_ID;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline GhostComponent(qsf::Prototype* prototype);

		inline uint32 getPlayerId() const;
		void setPlayerId(uint32 playerId);

		/**
		*  @brief
		*    Returns if the entity is a ghost (doesn't belongs to the local player)
		*/
		inline bool isGhost() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGhostValues(qsf::Entity& entity);
		void resetGhostValues(qsf::Entity& entity);
		void synchronizeInternalAndRendererDataJob(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint16		  mApplyDelay;
		uint32		  mPlayerId;
		uint32		  mLocalPlayerId;
		float		  mAlphaValue;
		bool		  mInternalIsGhost;
		bool		  mRendererIsGhost;
		qsf::JobProxy mSynchronizeJobProxy;


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
#include "em5/component/multiplayer/GhostComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GhostComponent)
