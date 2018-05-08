/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Hualet Wang <mr.asianwang@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDBusConnectionInterface>

#include <DApplication>
#include <DLog>

#include "dbus_manager_adaptor.h"
#include "manager_object.h"
#include "dmenuapplication.h"

#define MENU_SERVICE_NAME "com.deepin.menu"
#define MENU_SERVICE_PATH "/com/deepin/menu"

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DMenuApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-menu");
    app.setApplicationVersion("2.0");

#if DTK_VERSION >= DTK_VERSION_CHECK(2, 0, 9, 0)
    app.setOOMScoreAdj(500);
#endif

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    ManagerObject managerObject;
    ManagerAdaptor manager(&managerObject);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.interface()->registerService(MENU_SERVICE_NAME,
                                            QDBusConnectionInterface::ReplaceExistingService,
                                            QDBusConnectionInterface::AllowReplacement);
    connection.registerObject(MENU_SERVICE_PATH, &managerObject);
    DMenuApplication::connect(connection.interface(), SIGNAL(serviceUnregistered(QString)),
                              &app, SLOT(quitApplication(QString)));

    return app.exec();
}
