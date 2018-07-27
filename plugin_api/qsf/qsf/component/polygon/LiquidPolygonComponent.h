// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/polygon/PolygonComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	class SceneManager;
	namespace v1
	{
		class Entity;
	}
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
	*    Liquid polygon component class
	*/
	class QSF_API_EXPORT LiquidPolygonComponent : public PolygonComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::LiquidPolygonComponent" unique component ID
		static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
		static const uint32 MATERIAL;		///< "Material" unique class property ID inside the class


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
		inline explicit LiquidPolygonComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LiquidPolygonComponent();

		inline const AssetProxy& getMaterial() const;
		inline void setMaterial(const AssetProxy& material);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetActive(bool active) override;
		virtual void onSetHidden(bool hidden) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		virtual bool closedContinuousLines() const override;
		virtual void onUpdateNodes() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PolygonComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual const Color4& getPolygonDebugColor() const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateMaterial(Ogre::v1::Entity& ogreEntity) const;
		void updateMesh();
		Ogre::SceneManager* getOgreSceneManager() const;

		/**
		*  @brief
		*    Update the visibility state of the internal OGRE scene node
		*/
		void updateOgreSceneNodeVisibility();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetProxy		  mMaterial;
		Ogre::SceneNode*  mOgreSceneNode;	///< OGRE scene node instance, can be a null pointer
		Ogre::v1::Entity* mOgreEntity;		///< OGRE entity instance, can be a null pointer


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
#include "qsf/component/polygon/LiquidPolygonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LiquidPolygonComponent)
