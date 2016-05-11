TEMPLATE    =  subdirs

SUBDIRS     +=  Pulse \
                QtService

Pulse.file      =   src/Pulse/Pulse.pro
Pulse.depends   =   QtService
QtService.file  =   libs/qtservice/qtservice.pro
