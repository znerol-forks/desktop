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

#include <QObject>
#include <QSettings>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include <QVector>
#include <QDebug>

#include <utility>

namespace OCC {

struct Emoji
{
    Emoji(QString u, QString s, bool isCustom = false)
        : unicode(std::move(std::move(u)))
        , shortname(std::move(std::move(s)))
        , isCustom(isCustom)
    {
    }
    Emoji() = default;

    friend QDataStream &operator<<(QDataStream &arch, const Emoji &object)
    {
        arch << object.unicode;
        arch << object.shortname;
        return arch;
    }

    friend QDataStream &operator>>(QDataStream &arch, Emoji &object)
    {
        arch >> object.unicode;
        arch >> object.shortname;
        object.isCustom = object.unicode.startsWith("image://");
        return arch;
    }

    QString unicode;
    QString shortname;
    bool isCustom = false;

    Q_GADGET
    Q_PROPERTY(QString unicode MEMBER unicode)
    Q_PROPERTY(QString shortname MEMBER shortname)
    Q_PROPERTY(bool isCustom MEMBER isCustom)
};

class EmojiModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList history READ history NOTIFY historyChanged)

    Q_PROPERTY(QVariantList people MEMBER people CONSTANT)
    Q_PROPERTY(QVariantList nature MEMBER nature CONSTANT)
    Q_PROPERTY(QVariantList food MEMBER food CONSTANT)
    Q_PROPERTY(QVariantList activity MEMBER activity CONSTANT)
    Q_PROPERTY(QVariantList travel MEMBER travel CONSTANT)
    Q_PROPERTY(QVariantList objects MEMBER objects CONSTANT)
    Q_PROPERTY(QVariantList symbols MEMBER symbols CONSTANT)
    Q_PROPERTY(QVariantList flags MEMBER flags CONSTANT)

public:
    static QObject *singletonProvider(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    explicit EmojiModel(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    Q_INVOKABLE QVariantList history();
    Q_INVOKABLE static QVariantList filterModel(const QString &filter);

Q_SIGNALS:
    void historyChanged();

public Q_SLOTS:
    void emojiUsed(const QVariant &modelData);

private:
    static const QVariantList people;
    static const QVariantList nature;
    static const QVariantList food;
    static const QVariantList activity;
    static const QVariantList travel;
    static const QVariantList objects;
    static const QVariantList symbols;
    static const QVariantList flags;

    QSettings _settings;
};

}

Q_DECLARE_METATYPE(OCC::Emoji)
