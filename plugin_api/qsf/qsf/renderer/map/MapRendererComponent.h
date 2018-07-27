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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneManager;
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
	*    Map renderer component interface
	*
	*  @note
	*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
	*/
	class QSF_API_EXPORT MapRendererComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::MapRendererComponent" unique component ID
		static const uint32 NUMBER_OF_CELLS_PER_EDGE;	///< "CellsPerEdge" unique component ID


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
		inline explicit MapRendererComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MapRendererComponent();

		/**
		*  @brief
		*    Return the number of grid cells along a grid edge
		*
		*  @return
		*    Number of grid cells along a grid edge, e.g. 16
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint32 getCellsPerEdge() const;

		/**
		*  @brief
		*    Set the number of grid cells along a grid edge
		*
		*  @param[in] cellsPerEdge
		*    Number of grid cells along a grid edge, e.g. 16
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setCellsPerEdge(uint32 cellsPerEdge);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the OGRE scene manager instance
		*
		*  @return
		*    The OGRE scene manager instance, can be a null pointer, do no destroy the returned instance
		*/
		Ogre::SceneManager* getOgreSceneManager() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetDebug(bool debug) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set the 2D grid axis aligned box
		*/
		void set2dGridAxisAlignedBox();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint32 mCellsPerEdge;	///< Number of grid cells along a grid edge


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
#include "qsf/renderer/map/MapRendererComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MapRendererComponent)
