// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/FastDebugDraw.h"
#include "qsf/math/BitFlagSet.h"

#include <OGRE/OgreFrameListener.h>

#include <LinearMath/btIDebugDraw.h>

#include <vector>
#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btDynamicsWorld;
class btCollisionObject;


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
	*    Bullet physics debug drawing class (http://www.ogre3d.org/tikiwiki/tiki-index.php?page=BulletDebugDrawer)
	*
	*  @remarks
	*    This code is currently unused, but very helpful when debugging the raw BulletPhysics simulation.
	*/
	class QSF_API_EXPORT PhysicsDebugDraw : public btIDebugDraw, public FastDebugDraw, public Ogre::FrameListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char* DEFAULT_MATERIAL;

		enum Flag
		{
			DRAW_WORLD			=	1<<0,	///< Draw all collision objects and everything else from "btDynamicsWorld::debugDrawWorld()"
			DRAW_CONTACT_POINTS	=	1<<1,	///< Draw contact points
			DRAW_SELECTED		=	1<<2	///< Draw selected collision objects
		};
		typedef BitFlagSet<uint8, Flag> Flags;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit PhysicsDebugDraw(qsf::Map& map, btDynamicsWorld* bulletDynamicsWorld);
		virtual ~PhysicsDebugDraw();
		Flags getFlags() const;
		void setFlags(Flags flags);
		void selectBulletCollisionObject(const btCollisionObject& bulletCollisionObject);
		void unselectBulletCollisionObject(const btCollisionObject& bulletCollisionObject);


	//[-------------------------------------------------------]
	//[ Public virtual btIDebugDraw methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		virtual void drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar) override;
		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
		virtual void reportErrorWarning(const char* warningString) override;
		virtual void draw3dText(const btVector3& location, const char* textString) override;
		virtual void setDebugMode(int debugMode) override;
		virtual int getDebugMode() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::FrameListener methods         ]
	//[-------------------------------------------------------]
	protected:
		virtual bool frameStarted(const Ogre::FrameEvent& evt) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct ContactPoint
		{
			Ogre::Vector3		mFrom;
			Ogre::Vector3		mTo;
			Ogre::ColourValue	mColor;
			size_t				mDieTime;
		};

		typedef std::vector<ContactPoint> ContactPointVector;
		typedef std::unordered_set<const btCollisionObject*> SelectedBulletCollisionObjects;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Flags							mFlags;
		btDynamicsWorld*				mBulletDynamicsWorld;
		DebugDrawModes					mDebugModes;
		ContactPointVector*				mContactPoints;
		ContactPointVector				mContactPoints1;
		ContactPointVector				mContactPoints2;
		SelectedBulletCollisionObjects	mSelectedBulletCollisionObjects;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
