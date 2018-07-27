// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/reflection/type/CampQsfColor4.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class ManualObject;
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
	*    Meter ruler component class
	*/
	class QSF_API_EXPORT MeterRulerComponent : public RendererComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::MeterRulerComponent" unique component ID


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
		inline MeterRulerComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeterRulerComponent();

		inline const Color4& getColor() const;
		inline void setColor(const Color4& color);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		inline virtual Ogre::SceneNode* getOgreSceneNode() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void rebuildManualObject();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32		 RENDER_QUEUE_ID;
		static const std::string MATERIAL_NAME;	///< Name of OGRE material used for our manual object


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::SceneNode*	mOgreSceneNode;	///< OGRE scene node instance containing mOgreObject
		Ogre::ManualObject*	mOgreObject;	///< OGRE object instance holding meter ruler mesh

		// Properties
		Color4 mColor;	///< Visible color


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
#include "qsf/renderer/utility/MeterRulerComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeterRulerComponent)
