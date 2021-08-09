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

#pragma once

#include "common/result.h"

#include <userstatusjob.h>
#include <datetimeprovider.h>

#include <QObject>
#include <QMetaType>
#include <QtNumeric>

#include <cstddef>
#include <memory>
#include <vector>

namespace OCC {

class UserStatusDialogModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userStatusMessage READ userStatusMessage NOTIFY userStatusChanged)
    Q_PROPERTY(QString userStatusEmoji READ userStatusEmoji WRITE setUserStatusEmoji NOTIFY userStatusChanged)
    Q_PROPERTY(OCC::UserStatus::OnlineStatus onlineStatus READ onlineStatus WRITE setOnlineStatus NOTIFY onlineStatusChanged)
    Q_PROPERTY(int predefinedStatusesCount READ predefinedStatusesCount NOTIFY predefinedStatusesChanged)
    Q_PROPERTY(QStringList clearAtStages READ clearAtStages CONSTANT)
    Q_PROPERTY(QString clearAt READ clearAt NOTIFY clearAtChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    explicit UserStatusDialogModel(QObject *parent = nullptr);

    explicit UserStatusDialogModel(std::shared_ptr<UserStatusJob> userStatusJob,
        QObject *parent = nullptr);

    explicit UserStatusDialogModel(std::shared_ptr<UserStatusJob> userStatusJob,
        std::unique_ptr<DateTimeProvider> dateTimeProvider,
        QObject *parent = nullptr);

    explicit UserStatusDialogModel(const UserStatus &userStatus,
        std::unique_ptr<DateTimeProvider> dateTimeProvider,
        QObject *parent = nullptr);

    explicit UserStatusDialogModel(const UserStatus &userStatus,
        QObject *parent = nullptr);

    ~UserStatusDialogModel();

    UserStatus::OnlineStatus onlineStatus() const;
    Q_INVOKABLE void setOnlineStatus(OCC::UserStatus::OnlineStatus status);
    QString userStatusMessage() const;
    Q_INVOKABLE void setUserStatusMessage(const QString &message);
    void setUserStatusEmoji(const QString &emoji);
    QString userStatusEmoji() const;

    Q_INVOKABLE void setUserStatus();
    Q_INVOKABLE void clearUserStatus();

    int predefinedStatusesCount() const;
    Q_INVOKABLE UserStatus predefinedStatus(int index) const;
    Q_INVOKABLE QString predefinedStatusClearAt(int index) const;
    Q_INVOKABLE void setPredefinedStatus(int index);

    QStringList clearAtStages() const;
    QString clearAt() const;
    Q_INVOKABLE void setClearAt(int index);

    QString errorMessage() const;

signals:
    void showError();
    void errorMessageChanged();
    void userStatusChanged();
    void onlineStatusChanged();
    void clearAtChanged();
    void predefinedStatusesChanged();
    void finished();

private:
    enum class ClearStageType {
        DontClear,
        HalfHour,
        OneHour,
        FourHour,
        Today,
        Week
    };

    void init();
    void onUserStatusFetched(const UserStatus &userStatus);
    void onPredefinedStatusesFetched(const std::vector<UserStatus> &statuses);
    void onUserStatusSet();
    void onMessageCleared();
    void onError(UserStatusJob::Error error);

    QString clearAtStageToString(ClearStageType stage) const;
    QString clearAtReadable(const Optional<ClearAt> &clearAt) const;
    QString timeDifferenceToString(int differenceSecs) const;
    Optional<ClearAt> clearStageTypeToDateTime(ClearStageType type);
    void setError(const QString &reason);

    std::shared_ptr<UserStatusJob> _userStatusJob {};
    std::vector<UserStatus> _predefinedStatuses;
    UserStatus _userStatus;
    std::unique_ptr<DateTimeProvider> _dateTimeProvider;

    QString _errorMessage;

    std::vector<ClearStageType> _clearStages = {
        ClearStageType::DontClear,
        ClearStageType::HalfHour,
        ClearStageType::OneHour,
        ClearStageType::FourHour,
        ClearStageType::Today,
        ClearStageType::Week
    };
};
}

Q_DECLARE_METATYPE(OCC::UserStatusDialogModel *);
