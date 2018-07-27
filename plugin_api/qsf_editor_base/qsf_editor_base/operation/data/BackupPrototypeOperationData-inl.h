// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline BackupPrototypeOperationData::BackupPrototypeOperationData()
			{
				// Nothing to do in here
			}

			inline BackupPrototypeOperationData::BackupPrototypeOperationData(const Prototype& prototype)
			{
				// Backup the prototype
				backupPrototype(prototype);
			}

			inline BackupPrototypeOperationData::~BackupPrototypeOperationData()
			{
				// Nothing to do in here
			}

			inline const boost::property_tree::ptree& BackupPrototypeOperationData::getPrototypeBackup() const
			{
				return mPrototypeBackup;
			}


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			inline uint32 BackupPrototypeOperationData::getId() const
			{
				return OPERATION_DATA_ID;
			}

			inline size_t BackupPrototypeOperationData::getMemoryConsumption() const
			{
				// TODO(co) Implement me
				return OperationData::getMemoryConsumption();
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
