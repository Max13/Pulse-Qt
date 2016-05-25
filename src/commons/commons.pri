SOURCES     +=  ../commons/helpers.cpp

HEADERS     +=  ../commons/defines.hpp

RESOURCES   +=  ../commons/resources/resources.qrc

OTHER_FILES +=  ../commons/resources/icon.icns \
                ../commons/resources/icon.ico \
                ../commons/resources/icon.svg \
                ../commons/resources/Info.plist
osx {
    ICON                =   ../commons/resources/icon.icns
    QMAKE_INFO_PLIST    =   ../commons/resources/Info.plist
}
win32:RC_ICONS          =   ../commons/resources/icon.ico
