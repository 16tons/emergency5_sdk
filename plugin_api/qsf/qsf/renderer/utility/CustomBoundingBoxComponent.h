// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"


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
	*    Custom bounding box component class
	*/
	class CustomBoundingBoxComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::CustomBoundingBoxComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit CustomBoundingBoxComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CustomBoundingBoxComponent();

		/**
		*  @brief
		*    Gets the local minimum corner position in object space coordinates of the custom bounding box
		*/
		inline const glm::vec3& getLocalMinimumCorner() const;

		/**
		*  @brief
		*    Set the local minimum corner of the custom bounding box
		*
		*  @param[in] localMinimumCorner
		*    Minimum corner position in object space coordinates
		*/
		void setLocalMinimumCorner(const glm::vec3& localMinimumCorner);

		/**
		*  @brief
		*    Gets the local maximum corner position in object space coordinates of the custom bounding box
		*/
		inline const glm::vec3& getLocalMaximumCorner() const;

		/**
		*  @brief
		*    Set the local maximum corner of the custom bounding box
		*
		*  @param[in] localMaximumCorner
		*    Maximum corner position in object space coordinates
		*/
		void setLocalMaximumCorner(const glm::vec3& localMaximumCorner);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateCustomBoundingBox();
		void resetCustomBoundingBox();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3 mLocalMinimumCorner;
		glm::vec3 mLocalMaximumCorner;


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
#include "qsf/renderer/utility/CustomBoundingBoxComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CustomBoundingBoxComponent)
