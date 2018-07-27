// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MeterRulerComponent::MeterRulerComponent(Prototype* prototype) :
		RendererComponent(prototype),
		mOgreSceneNode(nullptr),
		mOgreObject(nullptr),
		mColor(0.75f, 0.75f, 0.75f, 1.0f)
	{
		// Handle the OGRE manual object as overlay by setting the proper render queue group
		setRenderQueueId(RENDER_QUEUE_ID); // Don't use "qsf::RENDER_QUEUE_OVERLAY"
	}

	inline MeterRulerComponent::~MeterRulerComponent()
	{
		// Nothing to do in here
	}

	inline const Color4& MeterRulerComponent::getColor() const
	{
		return mColor;
	}

	inline void MeterRulerComponent::setColor(const Color4& color)
	{
		// Something changed?
		if (mColor != color)
		{
			mColor = color;

			// Update render geometry
			if (isRunning())
			{
				rebuildManualObject();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	inline bool MeterRulerComponent::isGridManaged() const
	{
		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	inline Ogre::SceneNode* MeterRulerComponent::getOgreSceneNode() const
	{
		return mOgreSceneNode;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
