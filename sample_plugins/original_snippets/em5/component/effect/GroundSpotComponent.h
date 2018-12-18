// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>


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
	class EM5_API_EXPORT GroundSpotComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::GroundSpotComponent" unique component ID


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
		explicit GroundSpotComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GroundSpotComponent();

		inline float getRadius() const;

		/**
		*  @brief
		*    Show a highlighted circle around the entity on the ground
		*
		*  @param[in] radius
		*    Radius of the spotlight on the ground. Spotlight is removed when the radius is <= 0
		*/
		void setRadius(float radius);

		inline qsf::Entity* getGroundSpotLightEntity() const;

		inline float getGroundSpotLightHeight() const;
		inline void setGroundSpotLightHeight(float groundSpotLightHeight);

		void setClipFactors(float nearClipFactor, float farClipFactor);

		/**
		*  @brief
		*    Show a highlighted circle around the entity on the ground
		*
		*  @param[in] assetId
		*    Name of the texture to use on the spotlight.
		*    By default "qsf/texture/default/missing" is used.
		*	 Using an empty name is valid in which case there is only a spotlight without texture.
		*/
		void setSpotLightTexture(const std::string& assetId);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;
		virtual void onSetActive(bool active) override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	protected:
		qsf::Entity* createSpotLightEntity(const std::string& textureAssetName);
		void setupLightForEntity(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void createGroundLight();
		void destroyGroundLight();
		void setupGroundLight();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		float					  mGroundSpotLightRadius;	///< Radius of highlighted circle on the ground
		qsf::WeakPtr<qsf::Entity> mGroundSpotLightEntity;	///< Spotlight used to highlight a circle on the ground
		float					  mGroundSpotLightHeight;	///< Height of spotlight above entity
		float					  mNearClipFactor;
		float					  mFarClipFactor;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string			mSpotLightTexture;				///< Name of the asset for the texture used on the spotlight


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
#include "em5/component/effect/GroundSpotComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GroundSpotComponent)
