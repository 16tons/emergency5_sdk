// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/view/FireMaterialsView.h"
#include "em5_editor/menu/FireMaterialsMenu.h"
#include "em5_editor/model/FireMaterialsModel.h"
#include "em5_editor/model/FireMaterialsFilterModel.h"
#include "ui_FireMaterialsView.h"	// Automatically created by Qt's uic (output directory is "tmp\qt\uic\em5_editor" within the hand configured Visual Studio files, another directory when using CMake)

#include <em5/plugin/AssetTypes.h>
#include <em5/fire/FireSystem.h>
#include <em5/fire/FireMaterialManager.h>
#include <em5/EM5Helper.h>

#include <qsf_editor/delegate/ObjectPropertiesDelegate.h>
#include <qsf_editor/asset/AssetEditHelper.h>

#include <qsf/QsfHelper.h>
#include <qsf/asset/Asset.h>
#include <qsf/asset/AssetSystem.h>
#include <qsf/asset/project/AssetPackage.h>
#include <qsf/reflection/CampHelper.h>

#include <QtWidgets/qinputdialog.h>
#include <QtWidgets/qmessagebox.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const uint32 FireMaterialsView::PLUGINABLE_ID = qsf::StringHash("em5::editor::FireMaterialsView");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireMaterialsView::FireMaterialsView(qsf::editor::ViewManager* viewManager, QWidget* qWidgetParent) :
			View(viewManager, qWidgetParent),
			mUiFireMaterialsView(nullptr),
			mFireMaterialsModel(nullptr),
			mFilterModel(nullptr),
			mAssetEditHelper(nullptr)
		{
			// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
			addViewAndTabify(reinterpret_cast<QMainWindow&>(*qWidgetParent), Qt::BottomDockWidgetArea);
		}

		FireMaterialsView::~FireMaterialsView()
		{
			// Destroy the UI fire materials instance
			if (nullptr != mUiFireMaterialsView)
			{
				delete mUiFireMaterialsView;
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		void FireMaterialsView::retranslateUi()
		{
			mUiFireMaterialsView->retranslateUi(this);
		}

		void FireMaterialsView::changeVisibility(bool visible)
		{
			// Lazy evaluation: If the view is shown the first time, create its content
			if (visible)
			{
				if (nullptr == mUiFireMaterialsView)
				{
					// Create and setup the view content
					// ... and here the magic happens...
					mUiFireMaterialsView = new Ui::FireMaterialsView();
					{
						// Create widget which is the container for the widgets created by the automatically generated class from the ui file
						QWidget* qWidget = new QWidget(this);
						mUiFireMaterialsView->setupUi(qWidget);
						setWidget(qWidget);
					}

					{ // Configurate table view
						QTableView* qTableView = mUiFireMaterialsView->tableViewMaterials;
						qTableView->setSelectionMode(QAbstractItemView::SingleSelection);
						qTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
						qTableView->setSortingEnabled(true);

						// Custom context menu required
						qTableView->setContextMenuPolicy(Qt::CustomContextMenu);
					}

					// Connect signals and slots
					connect(mUiFireMaterialsView->pushButtonEdit, SIGNAL(toggled(bool)), this, SLOT(onChangeEditMode(bool)));
					connect(mUiFireMaterialsView->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(onClickedAdd(bool)));
					connect(mUiFireMaterialsView->pushButtonDuplicate, SIGNAL(clicked(bool)), this, SLOT(onClickedDuplicate(bool)));
					connect(mUiFireMaterialsView->pushButtonDelete, SIGNAL(clicked(bool)), this, SLOT(onClickedDelete(bool)));
				}

				// Initial no editing is allowed
				mUiFireMaterialsView->tableViewMaterials->setEditTriggers(QAbstractItemView::NoEditTriggers);
				enableGui(true);
				enableEditButtons(false);

				// Rebuild GUI
				rebuildGui();
				onSelectionChanged();
			}
			else
			{
				if (mUiFireMaterialsView->pushButtonEdit->isChecked())
					mUiFireMaterialsView->pushButtonEdit->setChecked(false);
			}
		}


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		void FireMaterialsView::onClickedAdd(bool)
		{
			bool ok = false;
			const QString inputDialogText = QInputDialog::getText(this, tr("INPUT_DIALOG_CREATE_NEW_FIRE_MATERIAL_HEADER"), tr("INPUT_DIALOG_CREATE_NEW_FIRE_MATERIAL_DESCRIPTION"), QLineEdit::Normal, "", &ok);
			if (ok && !inputDialogText.isEmpty())
			{
				// To avoid duplicates we create a unique name
				std::string fireMaterialName = inputDialogText.toUtf8().constData();
				fireMaterialName = EM5_FIRE.getFireMaterialManager().findUniqueNameForMaterial(fireMaterialName);

				// Create and register a new fire material instance
				FireMaterial* fireMaterial = new FireMaterial(qsf::StringHash(fireMaterialName));
				fireMaterial->setName(fireMaterialName);

				// Register new fire material instance
				EM5_FIRE.getFireMaterialManager().insertElement(fireMaterial->getFireMaterialId(), *fireMaterial);

				// Rebuild the view
				rebuildGui();

				// Select the new material
				updateSelection(*fireMaterial);
			}
		}

		void FireMaterialsView::onClickedDuplicate(bool)
		{
			// Get the selected index
			const QModelIndex qModelIndex = mUiFireMaterialsView->tableViewMaterials->currentIndex();
			qsf::editor::BaseModel::IndexData* indexData = static_cast<qsf::editor::BaseModel*>(mUiFireMaterialsView->tableViewMaterials->model())->getIndexDataByQModelIndex(mFilterModel->mapToSource(qModelIndex));
			if (nullptr != indexData && static_cast<FireMaterialsModel::IndexType>(indexData->indexType) == FireMaterialsModel::INDEXTYPE_FIREMATERIAL)
			{
				FireMaterial* sourceFireMaterial = static_cast<FireMaterial*>(indexData->object);

				// Take the original name and append a suffix because else the name and the ID will be identical which is not what we want
				std::string fireMaterialName = sourceFireMaterial->getName();

				// To avoid duplicates we create a unique name
				fireMaterialName = EM5_FIRE.getFireMaterialManager().findUniqueNameForMaterial(fireMaterialName);

				// Create new fire material instance
				FireMaterial* newFireMaterial = new FireMaterial(qsf::StringHash(fireMaterialName));

				// Copy values from selected material
				qsf::CampHelper::copyCampUserObjectProperties(camp::UserObject(*newFireMaterial), sourceFireMaterial);

				// Override the old name with a unique one
				newFireMaterial->setFireMaterialId(qsf::StringHash(fireMaterialName));
				newFireMaterial->setName(fireMaterialName);

				// Register new fire material instance
				EM5_FIRE.getFireMaterialManager().insertElement(newFireMaterial->getFireMaterialId(), *newFireMaterial);

				// Rebuild the view
				rebuildGui();

				// Set the duplicate as the selected material
				updateSelection(*newFireMaterial);
			}
		}

		void FireMaterialsView::onClickedDelete(bool)
		{
			const QModelIndex qModelIndex = mUiFireMaterialsView->tableViewMaterials->currentIndex();
			qsf::editor::BaseModel::IndexData* indexData = static_cast<qsf::editor::BaseModel*>(mUiFireMaterialsView->tableViewMaterials->model())->getIndexDataByQModelIndex(mFilterModel->mapToSource(qModelIndex));
			if (nullptr != indexData && static_cast<FireMaterialsModel::IndexType>(indexData->indexType) == FireMaterialsModel::INDEXTYPE_FIREMATERIAL)
			{
				const FireMaterial* fireMaterialToDelete = static_cast<FireMaterial*>(indexData->object);
				QSF_CHECK(nullptr != fireMaterialToDelete, "EM5 editor: The fire material instance is invalid", return);

				// Delete the selected element
				EM5_FIRE.getFireMaterialManager().destroyElement(fireMaterialToDelete->getFireMaterialId());

				// Rebuild the view
				rebuildGui();
				onSelectionChanged();
			}
		}

		void FireMaterialsView::onSelectionChanged()
		{
			const bool enabled = mUiFireMaterialsView->tableViewMaterials->currentIndex().isValid();
			mUiFireMaterialsView->pushButtonDelete->setEnabled(enabled);
			mUiFireMaterialsView->pushButtonDuplicate->setEnabled(enabled);
		}

		void FireMaterialsView::onCustomContextMenuRequested(const QPoint& qPoint)
		{
			FireMaterialsMenu(mUiFireMaterialsView->tableViewMaterials->currentIndex().isValid(), *this).exec(mUiFireMaterialsView->tableViewMaterials->mapToGlobal(qPoint));
		}

		void FireMaterialsView::onChangeEditMode(bool checked)
		{
			if (checked)
			{
				// Disable the GUI as long as we don't have the fire material asset locks
				enableGui(false);

				// Try to get the locks for the assets
				QSF_ASSERT(nullptr == mAssetEditHelper, "EM5 editor fire materials view: There's already an asset edit helper instance, this can't be", QSF_REACT_NONE);
				mAssetEditHelper = std::shared_ptr<qsf::editor::AssetEditHelper>(new qsf::editor::AssetEditHelper());
				std::vector<qsf::Asset*> assets;
				QSF_ASSET.getAssetsOfType(AssetTypes::EM5_FIREMATERIAL, assets);
				for (qsf::Asset* asset : assets)
				{
					qsf::Asset* editedAsset = mAssetEditHelper->editAsset(asset->getGlobalAssetId());
					if (nullptr == editedAsset)
					{
						QSF_ERROR("EM5 editor fire materials view failed to open the asset for editing: " << asset->getGlobalAssetId(), QSF_REACT_NONE);
					}
				}

				// The ready callback gets always called when all lock operations are done. We have to check in the callback if the locking was successful (isGood state of the helper)
				mAssetEditHelper->callWhenReady(boost::bind(&FireMaterialsView::onAssetReady, this));
			}
			else
			{
				// Disable the GUI until the done callback is called
				enableGui(false);

				// Disable editing
				mUiFireMaterialsView->tableViewMaterials->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
				disconnect(mUiFireMaterialsView->tableViewMaterials, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenuRequested(const QPoint&)));
				disconnect(mUiFireMaterialsView->tableViewMaterials, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onSelectionChanged()));
				enableEditButtons(false);

				if (nullptr != mAssetEditHelper)
				{
					// Ask for save all changes to local file
					FireMaterialManager& fireMaterialManager = EM5_FIRE.getFireMaterialManager();
					if (fireMaterialManager.getContentHasChanged())
					{
						fireMaterialManager.saveAssets();
					}

					// Unmount the intermediate asset package now because we don't need the mount anymore before submitting the changes
					mAssetEditHelper->getIntermediateAssetPackage()->unmount();

					// Finalize the fire material asset editing
					std::vector<qsf::Asset*> assets;
					QSF_ASSET.getAssetsOfType(AssetTypes::EM5_FIREMATERIAL, assets);
					for (qsf::Asset* asset : assets)
					{
						mAssetEditHelper->setAssetUploadData(asset->getGlobalAssetId(), true, true);
					}

					mAssetEditHelper->submit();
					mAssetEditHelper->callWhenFinishedUploading(boost::bind(&FireMaterialsView::onSubmitDone, this));
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void FireMaterialsView::rebuildGui()
		{
			QTableView* tableViewMaterials = mUiFireMaterialsView->tableViewMaterials;

			if (nullptr == mFireMaterialsModel)
			{
				mFireMaterialsModel = new FireMaterialsModel(tableViewMaterials, EM5_FIRE);
			}
			else
			{
				mFireMaterialsModel->refresh();
			}

			if (nullptr == mFilterModel)
			{
				mFilterModel = new FireMaterialsFilterModel(this);
				mFilterModel->setSourceModel(mFireMaterialsModel);
				tableViewMaterials->setModel(mFilterModel);
			}

			// Make sure the table view is resized correctly
			tableViewMaterials->setVisible(false);
			tableViewMaterials->resizeRowsToContents();
			tableViewMaterials->resizeColumnsToContents();
			tableViewMaterials->setVisible(true);

			// Setup the delegate
			tableViewMaterials->setItemDelegate(new qsf::editor::ObjectPropertiesDelegate(tableViewMaterials));

			// Restore sorting
			const int sortColumn = tableViewMaterials->horizontalHeader()->sortIndicatorSection();
			const Qt::SortOrder sortOrder = tableViewMaterials->horizontalHeader()->sortIndicatorOrder();
			mFilterModel->sort(sortColumn, sortOrder);
		}

		void FireMaterialsView::enableGui(bool enabled)
		{
			enableEditButtons(enabled);
			mUiFireMaterialsView->tableViewMaterials->setEnabled(enabled);
			mUiFireMaterialsView->pushButtonEdit->setEnabled(enabled);
		}

		void FireMaterialsView::enableEditButtons(bool enabled)
		{
			mUiFireMaterialsView->pushButtonAdd->setEnabled(enabled);
			mUiFireMaterialsView->pushButtonDuplicate->setEnabled(enabled);
			mUiFireMaterialsView->pushButtonDelete->setEnabled(enabled);
		}

		void FireMaterialsView::updateSelection(const FireMaterial& fireMaterial)
		{
			// Take the name of the new material
			const QString matchName = QString::fromUtf8(fireMaterial.getName().c_str());

			// Find the model index of the new material
			QTableView* tableViewMaterials = mUiFireMaterialsView->tableViewMaterials;
			const QModelIndex qModelIndexStart = tableViewMaterials->model()->index(0, 0, QModelIndex());
			const QModelIndexList qModelIndexList = tableViewMaterials->model()->match(qModelIndexStart, Qt::DisplayRole, QVariant(matchName), 1, Qt::MatchExactly);
			if (!qModelIndexList.empty())
			{
				tableViewMaterials->setCurrentIndex(qModelIndexList[0]);
			}

			onSelectionChanged();
		}

		void FireMaterialsView::onAssetReady()
		{
			// Check if we got for all registered assets the locks
			if (mAssetEditHelper->isGood())
			{
				// Yeah we have the locks now we can enable editing of the assets
				enableGui(true);
				// Let the logic handle enable/disable state for additional edit buttons
				onSelectionChanged();

				// Enable editing
				mUiFireMaterialsView->tableViewMaterials->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked | QAbstractItemView::EditTrigger::EditKeyPressed);
				connect(mUiFireMaterialsView->tableViewMaterials, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenuRequested(const QPoint&)));
				connect(mUiFireMaterialsView->tableViewMaterials, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onSelectionChanged()));

				EM5_FIRE.getFireMaterialManager().setIsEditing(true, -1);

				mFireMaterialsModel->enableEditing(true);

				// Mount the intermediate asset package
				// -> now the file savings goes into the intermediate asset package which gets submitted
				mAssetEditHelper->getIntermediateAssetPackage()->mount();
			}
			else
			{
				// Oh no! Someone else have at least one asset locked
				// Destroy the helper because we don't need it anymore
				mAssetEditHelper = nullptr;

				// Reset check state of the edit button
				mUiFireMaterialsView->pushButtonEdit->setChecked(false);

				// Enable ui but let the edit buttons still disabled
				enableGui(true);
				enableEditButtons(false);
			}
		}

		void FireMaterialsView::onSubmitDone()
		{
			mAssetEditHelper = nullptr;

			// Notify the model that editing is disabled
			mFireMaterialsModel->enableEditing(false);

			// Get the new asset revision and inform the fire material manager about it so he can ignore changes to this asset revision
			// TODO(sw) this works currently because we have only one asset for fire materials -> rework when we have more then one asset for fire materials
			std::vector<qsf::Asset*> assets;
			QSF_ASSET.getAssetsOfType(AssetTypes::EM5_FIREMATERIAL, assets);

			qsf::AssetRevision assetRevision = -1;
			if (!assets.empty())
				assetRevision = assets[0]->getRevision();

			// Inform the fire material manager about the finished editing and the changed asset revision
			EM5_FIRE.getFireMaterialManager().setIsEditing(false, assetRevision);

			// Enable ui but let the edit buttons still disabled
			enableGui(true);
			enableEditButtons(false);

		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
