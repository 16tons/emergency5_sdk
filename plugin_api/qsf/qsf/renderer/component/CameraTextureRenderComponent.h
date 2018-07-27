// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/job/JobProxy.h"
#include "qsf/reflection/type/CampQsfTime.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class RenderTexture;
	class Texture;
	template<typename T> class SharedPtr;
	typedef SharedPtr<Texture> TexturePtr;
}
namespace qsf
{
	class CameraComponent;
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
	*    Camera render to texture component class
	*/
	class QSF_API_EXPORT CameraTextureRenderComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::CameraTextureRenderComponent" unique component ID


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
		explicit CameraTextureRenderComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CameraTextureRenderComponent();

		inline uint32 getRenderWidth() const;
		void setRenderWidth(uint32 renderWidth);

		inline uint32 getRenderHeight() const;
		void setRenderHeight(uint32 renderHeight);

		inline const std::string& getTextureName() const;
		void setTextureName(const std::string& textureName);

		inline const Time& getTimeBetweenCalls() const;
		void setTimeBetweenCalls(const Time& timeBetweenCalls);

		/**
		*  @brief
		*    Call this method whenever you need an instance updated texture
		*/
		void instantUpdateTexture();


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
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void registerRenderJobProxy();

		/**
		*  @brief
		*    Render the camera into texture
		*/
		void renderUpdate(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint32					mRenderWidth;
		uint32					mRenderHeight;
		std::string				mTextureName;
		Time					mTimeBetweenCalls;
		// Internal
		Ogre::TexturePtr*		mOgreTexturePtr;
		Ogre::RenderTexture*	mOgreRenderTexture;
		qsf::JobProxy			mRenderJobProxy;	///< Render update job proxy
		CameraComponent*		mCameraComponent;	///< To check if we have a new camera component


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
#include "qsf/renderer/component/CameraTextureRenderComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraTextureRenderComponent)
