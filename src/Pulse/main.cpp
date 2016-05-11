#include <QString>
#include "MyService.hpp"

int main(int argc, char *argv[])
{
    MyService   svc(argc, argv, QStringLiteral("Pulse"));
    return svc.exec();
}
