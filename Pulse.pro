TEMPLATE    =  subdirs

SUBDIRS     +=  Pulse-App \
                Pulse-Service \
                QtService

Pulse-App.file          =   src/App/App.pro
Pulse-App.depends       =   Pulse-Service

Pulse-Service.file      =   src/Service/Service.pro
Pulse-Service.depends   =   QtService

QtService.file          =   libs/qtservice/qtservice.pro
