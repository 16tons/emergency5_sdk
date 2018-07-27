// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/BaseModel.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
	class UserObject;
}
namespace qsf
{
	class ClassSystem;
	class ParameterGroup;
	class BoostSignalComponent;
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
		*    CAMP properties Qt model
		*
		*  @remarks
		*    This is a generic Qt model which shows the properties of any CAMP reflection class. There's build in support for QSF
		*    components as well as QSF settings group and QSF debug group so that the Qt model will automatically update the Qt view in
		*    case of a property change.
		*/
		class GenericPropertiesModel : public BaseModel
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] parent
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*  @param[in] campUserObject
			*    CAMP user object to list the properties from, must be valid, the instance the user object is wrapping up must stay valid as long as this properties model exist (not the "camp::UserObject"-instance itself)
			*/
			GenericPropertiesModel(QObject* parent, const camp::UserObject& campUserObject);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GenericPropertiesModel();

			void setInspectedObject(const camp::UserObject& campUserObject);


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex parent(const QModelIndex& child) const override;
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
			virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
			virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
			virtual QStringList mimeTypes() const override;
			virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::GenericPropertiesModel methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    When a property value is changed, this method is called so the handling can be tracked in the
			*    child model class. This is basically a thin wrapper around the setData() method.
			*/
			virtual void onPropertyValueChanged(const camp::UserObject& campUserObject, const camp::Property& campProperty, const std::string& newPropertyValue) = 0;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			ClassSystem& mClassSystem;	///< QSF class system instance, do not destroy the instance


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Get "qsf::BoostSignalComponent" instance
			*
			*  @return
			*    "qsf::BoostSignalComponent" instance, can be a null pointer, do not destroy the returned instance
			*/
			BoostSignalComponent* getBoostSignal() const;

			/**
			*  @brief
			*    Connect to Boost signals
			*/
			void connectToBoostSignals();

			/**
			*  @brief
			*    Disconnect to Boost signals
			*/
			void disconnectFromBoostSignals();

			/**
			*  @brief
			*    Called in case a component property value was changed
			*
			*  @param[in] component
			*    Component instance which had a property value change
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*
			*  @note
			*    - The unique class property identifier is generated by "qsf::StringHash" by using the class property name
			*      (e.g. qsf::StringHash("Position") generating the hash value 3091144222)
			*/
			void onComponentPropertyChange(const Component& component, uint32 propertyId);

			/**
			*  @brief
			*    Called as soon as there's a property change inside the debug group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onDebugGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

			/**
			*  @brief
			*    Called as soon as there's a property change inside an QSF object
			*
			*  @param[in] object
			*    QSF object instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onPropertyChanged(const Object& object, uint32 propertyId);

			/**
			*  @brief
			*    Setup the base model data
			*/
			void setupModelData(TreeItem<IndexData>& parentItem);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			camp::UserObject*  mCampUserObject;		///< CAMP user object to list the properties from, always valid
			const camp::Class* mCampClass;			///< CAMP class instance received from the used CAMP user object, always valid
			QModelIndex		   mParentQModelIndex;	///< Parent Qt model index


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
