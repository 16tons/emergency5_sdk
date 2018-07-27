# This QMake project is only used to feed "lupdate.exe" which is used for internationalization
SOURCES      =	"editor_plugin/*.cpp" \
				"editor_plugin/plugin/*.cpp" \
				"editor_plugin/menu/*.cpp" \
				"editor_plugin/tool/*.cpp" \
				"editor_plugin/tool/*.ui" \
				"editor_plugin/view/*.cpp" \
				"editor_plugin/view/*.ui" \
				"editor_plugin/batchprocess/*.cpp" \
				"editor_plugin/map/test/*.cpp"
INCLUDEPATH +=	.
TRANSLATIONS =	"data/localization/de/editor_plugin.ts" \
				"data/localization/en/editor_plugin.ts"
