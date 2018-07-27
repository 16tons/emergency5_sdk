// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/component/Component.h>
#include <qsf/reflection/type/CampQsfColor4.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	class ManualObject;
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
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Grid component class
		*
		*  @note
		*    - Usually, a grid is used to support precise gizmo transform
		*/
		class GridComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::editor::GridComponent" unique component ID


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
			inline GridComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~GridComponent();

			inline float getGridExtents() const;
			inline void setGridExtents(float value);

			inline float getPrimaryLineWidth() const;
			inline void setPrimaryLineWidth(float value);

			inline float getSecondaryLineWidth() const;
			inline void setSecondaryLineWidth(float value);

			inline float getTertiaryLineWidth() const;
			inline void setTertiaryLineWidth(float value);

			inline float getPrimaryLineStep() const;
			inline void setPrimaryLineStep(float value);

			inline float getSecondaryLineStep() const;
			inline void setSecondaryLineStep(float value);

			inline float getTertiaryLineStep() const;
			inline void setTertiaryLineStep(float value);

			inline const Color4& getGridColor() const;
			inline void setGridColor(const Color4& value);


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
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const std::string MATERIAL_NAME;	///< Name of Ogre material used for our manual object


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			inline void setLineWidthByIndex(int index, float value);
			inline void setLineStepByIndex(int index, float value);
			void rebuildManualObject();
			void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ogre::SceneNode*	mOgreSceneNode;	///< OGRE scene node instance containing mOgreObject
			Ogre::ManualObject*	mOgreObject;	///< OGRE object instance holding gizmo mesh
			// Connected to the CAMP reflection system
			float  mGridExtents;	///< Total size of grid
			float  mLineWidths[3];	///< Widths of grid lines of three different thicknesses
			float  mLineSteps[3];	///< Steps where grid lines of different thicknesses will be drawn
			Color4 mGridColor;		///< Color of grid lines


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/grid/GridComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::GridComponent)
