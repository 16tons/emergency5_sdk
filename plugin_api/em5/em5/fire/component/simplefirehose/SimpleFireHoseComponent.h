// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/FastPodAllocator.h>
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
	class SimpleFireHoseMeshGenerator;
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
	*    Simple fire hose component based on existing implementation
	*
	*  @note
	*    - Pure technical component, no editor support due to time pressure
	*/
	class EM5_API_EXPORT SimpleFireHoseComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SimpleFireHoseMeshGenerator;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::SimpleFireHoseComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static SimpleFireHoseComponent& createFirehoseBetweenEntities(qsf::Entity& hydrant, const std::string& hydrantBoneName, qsf::Entity& nozzle, const std::string& nozzleBoneName);


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
		inline explicit SimpleFireHoseComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SimpleFireHoseComponent();

		void initializeWithEntities(qsf::Entity& connector, const std::string& connectorBoneName, qsf::Entity& nozzle, const std::string& nozzleBoneName);

		/**
		*  @brief
		*    Get waypoints along the fire hose from the current position back to the hydrant
		*/
		void getWaypoints(std::vector<glm::vec3>& outWaypoints);

		bool isGhost() const;


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


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		struct Section : public qsf::FastPodAllocator<Section>
		{
			qsf::Transform				 mStartTransform;
			qsf::Transform				 mEndTransform;
			mutable float				 mLength;
			std::string					 mOgreMeshName;
			std::string					 mOgreEntityName;
			SimpleFireHoseMeshGenerator* mMeshGenerator;
			Ogre::SceneNode*			 mOgreSceneNode;
			Ogre::v1::Entity*			 mOgreEntity;

			Section() :
				mLength(0.0f),
				mMeshGenerator(nullptr),
				mOgreSceneNode(nullptr),
				mOgreEntity(nullptr)
			{}
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
		void updateTransform(qsf::Transform& transform, qsf::Entity& targetEntity, const std::string& targetBoneName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Update jobs
		qsf::JobProxy			  mAnimationJobProxy;

		// Start entity: connector on hydrant
		qsf::WeakPtr<qsf::Entity> mHydrantEntity;
		qsf::WeakPtr<qsf::Entity> mConnectorEntity;
		std::string				  mConnectorBoneName;

		// End entity: nozzle carried by fireman
		qsf::WeakPtr<qsf::Entity> mFiremanEntity;
		qsf::WeakPtr<qsf::Entity> mNozzleEntity;
		std::string				  mNozzleBoneName;

		float	mMaxRangeHydrant;

		// Sections
		SectionArray			  mSections;

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
#include "em5/fire/component/simplefirehose/SimpleFireHoseComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SimpleFireHoseComponent)
