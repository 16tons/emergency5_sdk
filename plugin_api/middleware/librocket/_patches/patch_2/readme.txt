Changed "rocket\Source\Controls\ElementDataGridRow.cpp" to resolve an issue when a table row is removed,
but the following row's table_relative_index does not get recalculated, although it should be.
This bug can lead to null pointer memory access afterwards, e.g. when removing the following row later on.
