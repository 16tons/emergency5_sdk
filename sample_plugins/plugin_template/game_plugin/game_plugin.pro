# This QMake project is only used to feed "lupdate.exe" which is used for internationalization
SOURCES      =	"game_plugin/plugin/*.cpp" \
				"game_plugin/plugin/metaclasses/*.cpp"
INCLUDEPATH +=	.
TRANSLATIONS =	"data/localization/de/game_plugin.ts" \
				"data/localization/en/game_plugin.ts"
