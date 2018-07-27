// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Element;
		class ElementText;
		class ElementDocument;
	}
}
namespace Ogre
{
	class Camera;
	namespace v1
	{
		class Entity;
		class OldBone;
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
	*    Skeleton bone debug text class
	*
	*  @note
	*    - This skeleton debug class is based on http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Skeleton+Debugger
	*/
	class SkeletonBoneDebugText : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		SkeletonBoneDebugText(const std::string& name, const Ogre::v1::OldBone& ogreBone, const Ogre::v1::Entity& ogreEntity);
		virtual ~SkeletonBoneDebugText();

		void setVisible(bool visible) const;
		void setText(const std::string& text) const;
		void update(const Ogre::Camera& ogreCamera, Ogre::Viewport& ogreViewport) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		Rocket::Core::ElementDocument& getOrCreateRocketElementDocument();
		Rocket::Core::Element* createRocketElement(const std::string& type);
		Rocket::Core::Element* createRocketElementWithParent(Rocket::Core::Element* parentElement, const std::string& type);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Ogre::v1::OldBone& mOgreBone;
		const Ogre::v1::Entity&	 mOgreEntity;

		Rocket::Core::ElementDocument*	mRocketElementDocument;
		Rocket::Core::Element*			mContainerElement;
		Rocket::Core::Element*			mPositionElement;
		Rocket::Core::ElementText*		mTextElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
