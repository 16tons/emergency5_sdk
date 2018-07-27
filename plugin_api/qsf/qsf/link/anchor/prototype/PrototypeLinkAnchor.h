// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/anchor/LinkAnchor.h"
#include "qsf/link/anchor/prototype/PrototypeLinkAnchorAdapter.h"
#include "qsf/reflection/object/ObjectProxy.h"
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/math/EnumBitmask.h"

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class Entity;
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
	*    Prototype link anchor class
	*/
	class QSF_API_EXPORT PrototypeLinkAnchor : public LinkAnchor
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PrototypeLinkAnchor(const Prototype& prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrototypeLinkAnchor();

		/**
		*  @brief
		*    Return the owning prototype
		*/
		Prototype& getPrototype() const;

		/**
		*  @brief
		*    Return the owning entity
		*/
		Entity& getEntity() const;

		/**
		*  @brief
		*    Enable debug visualization
		*/
		void enableDebug(bool forwardLinks, bool backwardLinks);

		/**
		*  @brief
		*    Update debug visualization
		*/
		void updateDebug();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::LinkAnchor methods             ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStructuralChange() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onObjectChange(Object* object);
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);
		void updateDebugInternal();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef EnumBitmask<uint8, link::Direction> LinkDirectionBitmask;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ObjectProxy			 mObjectProxy;		///< Holds a pointer to the prototype

		// TODO(fw): Debug output variables are just overhead, we don't make use of this feature anyway
#ifndef RETAIL
		// Debug output
		DebugDrawProxy		 mDebugDrawProxy;
		LinkDirectionBitmask mDebugEnabled;
		boost::signals2::connection mRegisteredAtPreCompositorWorkspaceUpdate;
#endif


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PrototypeLinkAnchor)
