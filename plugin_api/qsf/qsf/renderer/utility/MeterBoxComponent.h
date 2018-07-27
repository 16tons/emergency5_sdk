// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"
#include "qsf/reflection/type/CampQsfColor4.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class ManualObject;
	class MovableObject;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Meter box component class
	*/
	class QSF_API_EXPORT MeterBoxComponent : public RendererComponent, public JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32		 COMPONENT_ID;	///< "qsf::MeterBoxComponent" unique component ID
		static const std::string MATERIAL_NAME;	///< Name of OGRE material used for our manual object


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
		inline MeterBoxComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeterBoxComponent();

		inline const Color4& getColor() const;
		inline void setColor(const Color4& color);

		inline bool getCenterAtPivot() const;
		void setCenterAtPivot(bool centerAtPivot);

		inline bool getShowDirection() const;
		inline void setShowDirection(bool showDirection);

		inline bool isVisibleEvenAfterClipping() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		virtual Ogre::SceneNode* getOgreSceneNode() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MeterBoxComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateActualVisiblity(bool isVisible);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void setOgreSceneNodeVisibility(bool visible) override;
		virtual void updateOgreObjectCastShadows() const override;
		virtual void updateOgreObjectRenderQueue(uint32 renderQueueId) override;
		inline virtual bool isGridManaged() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		inline virtual Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void initializeMeterBox();
		void releaseMeterBox();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Ogre::SceneNode*	 mOgreSceneNode;
		Ogre::ManualObject*	 mOgreManualObject;
		Ogre::MovableObject* mOgreDummyMovableObject;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void rebuildManualObject();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 RENDER_QUEUE_ID;

		enum Flags
		{
			CENTER_AT_PIVOT	= 1<<0,	///< Center at pivot
			SHOW_DIRECTION	= 1<<1,	///< Show direction
			LARGE_SCALE		= 1<<2	///< Is this considered to be a large scale box?
		};
		typedef BitFlagSet<uint8, Flags> FlagSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Color4  mColor;	///< Visible color
		FlagSet mFlags;	///< Internal flags


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/utility/MeterBoxComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeterBoxComponent)
