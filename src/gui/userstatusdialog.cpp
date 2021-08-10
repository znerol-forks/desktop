/*
 * Copyright (C) by Felix Weilbach <felix.weilbach@nextcloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include "userstatusdialog.h"
#include "accountfwd.h"
#include "jsonuserstatusjob.h"
#include "userstatusdialogmodel.h"
#include "emojimodel.h"
#include "account.h"

#include <QQuickView>
#include <QLoggingCategory>
#include <QQmlError>
#include <QQmlEngine>
#include <QQuickView>
#include <qqml.h>

Q_LOGGING_CATEGORY(lcUserStatusDialog, "nextcloud.gui.systray")

namespace OCC {
namespace UserStatusDialog {

    static bool logErrors(const QList<QQmlError> &errors)
    {
        bool isError = false;

        for (const auto &error : errors) {
            isError = true;
            qCWarning(lcUserStatusDialog) << error.toString();
        }

        return isError;
    }

    QQuickWindow *create(AccountPtr account)
    {
        const auto userStatusDialogModel = new UserStatusDialogModel(account->userStatusJob());
        const auto view = new QQuickView;
        const auto width = 450;
        const auto height = 600;
        view->setMaximumWidth(width);
        view->setMaximumHeight(height);
        view->setMinimumWidth(width);
        view->setMinimumHeight(height);
        QObject::connect(userStatusDialogModel, &UserStatusDialogModel::finished, view, [view]() {
            view->close();
            view->deleteLater();
        });
        view->engine()->setObjectOwnership(view, QQmlEngine::CppOwnership);
        view->engine()->setObjectOwnership(userStatusDialogModel, QQmlEngine::JavaScriptOwnership);

        view->setInitialProperties({ { "userStatusDialogModel", QVariant::fromValue(userStatusDialogModel) } });
        logErrors(view->errors());
        view->setSource(QUrl("qrc:/qml/src/gui/SetUserStatusView.qml"));
        logErrors(view->errors());

        return view;
    }
}
}
