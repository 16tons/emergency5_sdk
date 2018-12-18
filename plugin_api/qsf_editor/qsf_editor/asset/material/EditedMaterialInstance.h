// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/material/BaseMaterialPropertyListing.h"
#include "qsf_editor/asset/AssetStorageManagerTypes.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <QtWidgets/QUndoCommand>

#include <unordered_map>
#include <condition_variable>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QUndoStack;
QT_END_NAMESPACE
namespace qsf
{
	class Material;
	class MaterialManager;
	namespace editor
	{
		class AssetEditHelper;
		class EditedMaterialInstance;
		class MaterialEditorManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{

		struct EditorMaterialPropertyValue
		{
			enum InheritanceStatus
			{
				Inherited_Changed,		///< Shown in bold font: Properties from the parent material which were overwritten
				Normal,					///< Shown in normal font: Properties not existing in the parent material
				Inherited_Unchanged,	///< Shown in grey font: Properties from the parent material which were not overwritten
				None					///< There is no property source (e.g. a pass, which does not have a name)
			};

			std::string		  value;
			InheritanceStatus inheritance;
		};
		typedef std::unordered_map<std::string, EditorMaterialPropertyValue> EditorMaterialPropertyValues;

		struct EditorMaterialProperty
		{
			typedef EditorMaterialPropertyValue::InheritanceStatus Source;

			EditorMaterialProperty() :
				mSource(EditorMaterialPropertyValue::None),
				mBaseMaterialProperty(nullptr)
			{}

			EditorMaterialProperty(const std::string& value, const BaseMaterialProperty* baseMaterialProperty, Source source = Source::Normal) :
				mValue(value),
				mSource(source),
				mBaseMaterialProperty(baseMaterialProperty)
			{}

			std::string					mValue;
			Source						mSource;
			const BaseMaterialProperty* mBaseMaterialProperty;
		};
		typedef std::map<std::string, EditorMaterialProperty> EditorMaterialProperties;

		struct MaterialInfo
		{
			std::string				 mParent;
			EditorMaterialProperties mProperties;
		};

		// Material actions

		class MaterialAction : public QUndoCommand
		{
		public:
			MaterialAction() :
				mMaterialInstance(nullptr)
			{}

			virtual ~MaterialAction() {}

			inline void setMaterialInstance(EditedMaterialInstance* materialInstance)
			{
				mMaterialInstance = materialInstance;
			}

			virtual void redo() = 0;
			virtual void undo() = 0;

		protected:
			EditedMaterialInstance* mMaterialInstance;
		};

		// Properties

		class MaterialActionSetProperty : public MaterialAction
		{
		public:
			MaterialActionSetProperty(const std::string& key, const std::string& value) :
				mKey(key),
				mValue(value),
				mIsNewProperty(false)
			{}

			virtual void redo() override;
			virtual void undo() override;

		private:
			std::string mKey;
			std::string mValue;
			bool		mIsNewProperty;
			std::string mOldValue;
		};

		class MaterialActionDeleteProperty : public MaterialAction
		{
		public:
			explicit MaterialActionDeleteProperty(const std::string& key) :
				mKey(key)
			{}

			virtual void redo() override;
			virtual void undo() override;

		private:
			std::string mKey;
			std::string mOldValue;
		};


		//[-------------------------------------------------------]
		//[ Class                                                 ]
		//[-------------------------------------------------------]
		class EditedMaterialInstance : public QObject
		{
			friend MaterialEditorManager;


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			inline const std::string& getName() const;
			inline const std::string& getBaseMaterialName() const;
			inline bool isValid() const;
			inline bool isOpen() const;
			inline bool isBad() const;
			inline bool isDeleted() const;
			inline bool needsTextureCompilation() const;
			bool isModified() const;
			bool getMaterialInfo(MaterialInfo& materialInfo);
			bool hasMaterialProperty(const std::string& key);
			std::string getMaterialProperty(const std::string& key);
			void setMaterialProperty(const std::string& key, const std::string& value);
			void deleteMaterialProperty(const std::string& key);
			bool saveMaterial(bool forceCompile);
			void push(MaterialAction* action);
			void close();

			//[-------------------------------------------------------]
			//[ Internal                                              ]
			//[-------------------------------------------------------]
			MaterialManager& getMaterialManager();
			QUndoStack& getQUndoStack() const;
			Material* getMaterial();
			const BaseMaterialPropertyListing* getBaseMaterialPropertyListing();
			void getMaterialDependencies(boost::container::flat_set<GlobalAssetId>& dependencies);
			std::string getAbsoluteMaterialDatasourcePath() const;
			void markDeleted(bool isDeleted = true);
			/** Do not pass "true" to this method from within any qsf job until qsf::editor::AssetClient has been adjusted to run in a separate thread! */
			bool tryOpenForEdit(bool wait = false);

			void onMaterialAssetReady();
			void actuallyPerformTryOpenForEditProcedure();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			explicit EditedMaterialInstance(const std::string& materialName);
			virtual ~EditedMaterialInstance();

			void reset();

			bool _hasMaterialProperty(const std::string& key);
			std::string _getMaterialProperty(const std::string& key);

			bool loadSourceMaterial(const BaseMaterialPropertyListing& baseMaterialPropertyListing);
			void reconstructSourceMaterial();


		private Q_SLOTS:
			void onAssetTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void onAssetTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct MaterialBackup;
			struct FrozenProperty
			{
				std::string name;
				std::string value;
				std::string defaultValue;
				bool isDeleted;
			};


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string						   mName;
			std::string						   mBaseMaterialName;
			GlobalAssetId					   mGlobalAssetId;
			MaterialManager*				   mMaterialManager;
			Material*						   mMaterial;
			QUndoStack*						   mQUndoStack;
			bool							   mIsDeleted;
			bool							   mIsOpenForEdit;
			bool							   mIsTextureModified;
			bool							   mNeedsTextureCompilation;
			bool							   mIsUsingCompiledTextures;
			bool							   mIsUsingFrozenProperties;
			bool							   mHasStartedOpenForEditCallChain;
			bool							   mHasOpenForEditFailed;
			const BaseMaterialPropertyListing* mBaseMaterialPropertyListing;
			std::map<uint32, FrozenProperty>   mFrozenPropertyMap;				///< Names and displayed values of properties which are not propagated to the material during editing
			MaterialBackup*					   mMaterialBackup;
			std::mutex						   mMutex;
			std::condition_variable			   mConditionVariable;
			std::shared_ptr<AssetEditHelper>   mAssetEditHelper;	// Can be a null pointer


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


#include "qsf_editor/asset/material/EditedMaterialInstance-inl.h"
