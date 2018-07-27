// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/EditMode.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QPoint;
QT_END_NAMESPACE
namespace qsf
{
	class Prototype;
	class Component;
	class PrototypeManager;
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
		*    Prefab instancing edit mode class
		*
		*  @note
		*    - Has support for prefab instancing and entity cloning (an entity itself can be used as a prototype)
		*/
		class QSF_EDITOR_API_EXPORT PrefabInstancingEditMode : public EditMode
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PrefabInstancingEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit PrefabInstancingEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PrefabInstancingEditMode();

			/**
			*  @brief
			*    Return the instance of the prototype to copy
			*
			*  @return
			*    The instance of the prototype to copy ("mOriginalPrototypeId" or "mOriginalEntityId"), can be a null pointer, do not destroy the instance
			*/
			Prototype* getOriginalPrototype() const;

			/**
			*  @brief
			*    Copy the given prototype, create an preview object with it and change to prefab instancing edit mode
			*
			*  @param[in] prototype
			*    Reference to the prototype instance we want to copy as preview object
			*/
			void setModePlacePrototype(Prototype& prototype);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::PrefabInstancingEditMode methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Create entity instance at the given mouse position inside the render view
			*
			*  @param[in] prototype
			*    Prototype to instantiate
			*  @param[in] mousePosition
			*    Mouse position inside the render view to use for determine the world space position
			*/
			virtual void createEntityAt(Prototype& prototype, const QPoint& mousePosition);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;
			virtual void enterEvent(QEvent& qEvent) override;
			virtual void leaveEvent(QEvent& qEvent) override;
			virtual void dragEnterEvent(QDragEnterEvent& qDragEnterEvent) override;
			virtual void dragMoveEvent(QDragMoveEvent& qDragMoveEvent) override;
			virtual void dragLeaveEvent(QDragLeaveEvent& qDragLeaveEvent) override;
			virtual void dropEvent(QDropEvent& qDropEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onTriggeredSelectMoveModeQAction(bool checked);
			void onTriggeredSelectRotateModeQAction(bool checked);
			void onTriggeredSelectScaleModeQAction(bool checked);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Find the first component hit by a ray generated by using a given mouse position
			*
			*  @param[in]  mousePosition
			*    Mouse position inside the render view to use for determine the world space position
			*  @param[out] position
			*    Receives on success the found world space position, on failure the content is unknown
			*
			*  @return
			*    The first hit component, can be a null pointer, do not destroy the returned instance, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
			*
			*  @note
			*    - Ignores the preview entity while performing the picking
			*/
			Component* getFirstHitComponent(const QPoint& mousePosition, glm::vec3& position) const;

			/**
			*  @brief
			*    Create the entity instance used during the drag'n'drop process
			*
			*  @param[in] prototype
			*    Used prototype
			*  @param[in] mousePosition
			*    Mouse position inside the render view to use for determine the world space position
			*/
			void createDragDropPreviewEntity(Prototype& prototype, const QPoint& mousePosition);

			/**
			*  @brief
			*    Update the entity instance used during the drag'n'drop process, if there's one
			*
			*  @param[in] mousePosition
			*    Mouse position inside the render view to use for determine the world space position
			*
			*  @note
			*    - Creates a drag'n'drop preview entity if required
			*    - Destroys the drag'n'drop preview entity if required (invalid world space position)
			*/
			void updateDragDropPreviewEntity(const QPoint& mousePosition);

			/**
			*  @brief
			*    Destroy the entity instance used during the drag'n'drop process, if there's one
			*/
			void destroyDragDropPreviewEntity();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PrototypeManager* mPrototypeManager;		///< Prototype manager instance the prototype to instantiate is in, can be a null pointer, do not destroy the instance
			uint64			  mOriginalPrototypeId;		///< ID of the prototype we want to copy, "qsf::getUninitialized<uint64>()" if invalid, if valid "mOriginalEntityId" is considered to be invalid
			uint64			  mOriginalEntityId;		///< ID of the entity we want to copy, "qsf::getUninitialized<uint64>()" if invalid, if valid "mOriginalPrototypeId" is considered to be invalid
			uint64			  mDragDropPreviewEntityId;	///< ID of the entity instance used during the drag'n'drop process, "qsf::getUninitialized<uint64>()" if invalid
			uint64			  mInstancedEntityId;		///< The ID of the previously instanced entity, "qsf::getUninitialized<uint64>()" if invalid


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PrefabInstancingEditMode)
