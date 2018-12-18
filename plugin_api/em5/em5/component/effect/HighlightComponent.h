// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_compositing/component/HighlightComponent.h>

#include <qsf/job/JobProxy.h>
#include <qsf/math/EnumBitmask.h>


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
	*    Highlight component to highlight an entity
	*/
	class EM5_API_EXPORT HighlightComponent : public qsf::compositing::HighlightComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HighlightComponent" unique component ID

		enum HighlightFlags
		{
			FLAG_NONE,					///< No highlighting
			FLAG_SELECTABLE,			///< Entity is selectable highlighting
			FLAG_SELECTED,				///< Entity is selected highlighting
			FLAG_COMMANDTARGET,			///< Entity is target for next command highlighting
			FLAG_CUSTOM					///< The values for the highlighting is set from extern. No automatic update of the material
		};
		typedef qsf::EnumBitmask<uint8, HighlightFlags> HighlightFlagSet;


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
		explicit HighlightComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HighlightComponent();

		/**
		*  @brief
		*    Quick getter for the highlight flags
		*/
		inline bool getHighlightFlags(HighlightComponent::HighlightFlags flag) const;

		/**
		*  @brief
		*    Quick setter for the highlight flags
		*/
		void setHighlightFlags(HighlightComponent::HighlightFlags flag, bool value);

		inline HighlightFlags getCurrentHighlightType() const;
		inline float getGroundSpotLightRadius() const;

		/**
		*  @brief
		*    Show a highlighted circle around the entity on the ground
		*
		*  @param[in] radius
		*    Radius of the spotlight on the ground. Spotlight is removed when the radius is <= 0
		*/
		void setGroundSpotLightRadius(float radius);

		inline float getSpotLightNearClipDistance() const;
		void setSpotLightNearClipDistance(float distance);

		inline float getGroundHeight() const;
		void setGroundHeight(float groundHeight);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void createOrUpdateGroundSpotComponent();
		void updateHighlight();
		void resetHighlightValues();
		void updateJob(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HighlightFlagSet	mHighlightFlags;				///< Flags of this highlighter
		HighlightFlags		mCurrentHighlightType;			///< Current active highlight type
		qsf::JobProxy		mJobProxy;						///< Regular animation job proxy; for updates once a frame
		float				mGroundHeight;
		float				mGroundSpotLightRadius;			///< Radius for ground spot component (radius from there not directly used as it could be set from elsewhere)
		float				mSpotLightNearClipDistance;


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
#include "em5/component/effect/HighlightComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HighlightComponent)
