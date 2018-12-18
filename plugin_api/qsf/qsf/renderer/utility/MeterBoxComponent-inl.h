// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MeterBoxComponent::MeterBoxComponent(Prototype* prototype) :
		RendererComponent(prototype),
		mOgreSceneNode(nullptr),
		mOgreManualObject(nullptr),
		mOgreDummyMovableObject(nullptr),
		mColor(0.75f, 0.75f, 0.75f, 1.0f)
	{
		// Handle the OGRE manual object as overlay by setting the proper render queue group
		setRenderQueueId(RENDER_QUEUE_ID); // Don't use "qsf::RENDER_QUEUE_OVERLAY"
	}

	inline MeterBoxComponent::~MeterBoxComponent()
	{
		// Nothing to do in here
	}

	inline const Color4& MeterBoxComponent::getColor() const
	{
		return mColor;
	}

	inline void MeterBoxComponent::setColor(const Color4& color)
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

	inline bool MeterBoxComponent::getCenterAtPivot() const
	{
		return mFlags.isSet(CENTER_AT_PIVOT);
	}

	inline bool MeterBoxComponent::getShowDirection() const
	{
		return mFlags.isSet(SHOW_DIRECTION);
	}

	inline void MeterBoxComponent::setShowDirection(bool showDirection)
	{
		// Something changed?
		if (mFlags.isSet(SHOW_DIRECTION) != showDirection)
		{
			mFlags.set(SHOW_DIRECTION, showDirection);

			// Update render geometry
			if (isRunning())
			{
				rebuildManualObject();
			}
		}
	}

	inline bool MeterBoxComponent::isVisibleEvenAfterClipping() const
	{
		return (nullptr != mOgreManualObject);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	inline bool MeterBoxComponent::isGridManaged() const
	{
		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	Component* MeterBoxComponent::getMovableObjectRenderingListenerComponent()
	{
		return this;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
