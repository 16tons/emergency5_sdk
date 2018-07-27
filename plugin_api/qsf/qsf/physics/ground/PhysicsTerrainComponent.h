// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/PhysicsComponent.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btRigidBody;
class btCollisionShape;


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
	*    Physics terrain component class
	*/
	class QSF_API_EXPORT PhysicsTerrainComponent : public PhysicsComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsTerrainComponent" unique component ID


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
		inline explicit PhysicsTerrainComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsTerrainComponent();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onCollisionFilterGroupOrCollisionMaskChanged() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool buildGroundMapTiles();
		void releaseGroundMapTiles();
		void rebuildGroundMapTiles();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct GroundMapTile
		{
			btRigidBody*		mBulletRigidBody;
			btCollisionShape*	mBulletCollisionShape;
			std::vector<float>	mHeightData;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<GroundMapTile> mGroundMapTiles;


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
#include "qsf/physics/ground/PhysicsTerrainComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsTerrainComponent)
