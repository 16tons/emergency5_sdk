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
#include <qsf/math/Transform.h>
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	namespace v1
	{
		class Entity;
	}
}
namespace em5
{
	class SimpleBarrierTapeMeshGenerator;
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
	*    Simple warning tape component based on existing implementation
	*
	*  @note
	*    - Pure technical component, no editor support due to time pressure
	*/
	class EM5_API_EXPORT SimpleBarrierTapeComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SimpleBarrierTapeMeshGenerator;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::SimpleBarrierTapeComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static SimpleBarrierTapeComponent& createBarrierTapeBetweenEntities(qsf::Entity& entity1, const std::string& entity1BoneName, qsf::Entity& entity2, const std::string& entity2BoneName);


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
		explicit SimpleBarrierTapeComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SimpleBarrierTapeComponent();

		void initializeWithEntities(qsf::Entity& entity1, const std::string& entity1BoneName, qsf::Entity& entity2, const std::string& entity2BoneName);

		/**
		*  @brief
		*    Get waypoints along the tape from the current position back to the starting pole
		*/
		void getWaypoints(std::vector<glm::vec3>& outWaypoints);

		/**
		*  @brief
		*    Set transparency
		*    1: No Transparency  0: Full Transparency
		*/
		void setTransparency(float transparency);

		/**
		*  @brief
		*    Reset transparency
		*/
		void resetTransparency();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		struct Section
		{
			qsf::Transform					mStartTransform;
			qsf::Transform					mEndTransform;
			mutable float					mLength;
			std::string						mOgreMeshName;
			SimpleBarrierTapeMeshGenerator* mMeshGenerator;
			Ogre::SceneNode*				mOgreSceneNode;
			Ogre::v1::Entity*				mOgreEntity;

			Section() : mLength(0.0f), mMeshGenerator(nullptr), mOgreSceneNode(nullptr), mOgreEntity(nullptr) {}
		};
		typedef std::vector<Section> SectionArray;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateAnimation(const qsf::JobArguments& jobArguments);

		void addSection();
		void removeSection();
		void updateSectionMesh(uint32 sectionIndex);

		void setTransparency(Section& section);
		void resetTransparency(Section& section);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Update jobs
		qsf::JobProxy			  mAnimationJobProxy;

		// Start entity
		qsf::WeakPtr<qsf::Entity> mParentEntity1;
		qsf::WeakPtr<qsf::Entity> mEntity1;
		std::string				  mBoneName1;

		// End entity
		qsf::WeakPtr<qsf::Entity> mParentEntity2;
		qsf::WeakPtr<qsf::Entity> mEntity2;
		std::string				  mBoneName2;

		// Sections
		SectionArray			mSections;
		float					mTransparency;

		// Debug
		qsf::DebugDrawProxy		  mDebugDrawProxy;


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
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SimpleBarrierTapeComponent)
