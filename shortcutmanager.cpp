#include "shortcutmanager.h"
#include "appsettingspath.h"

#include <QShortcut>
#include <QKeySequence>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QWidget>

ShortcutManager::ShortcutManager(
    QWidget *window,
    QObject *parent
    )
    : QObject(parent),
    m_window(window)
{
}

QString ShortcutManager::settingsPath() const
{
    return appSettingsPath();
}

void ShortcutManager::add(
    const QString &name,
    const QString &defaultKey,
    std::function<void()> action)
{
    QFile file(settingsPath());

    QString key = defaultKey;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();

            if (line.isEmpty() || line.startsWith('#'))
                continue;

            int separator = line.indexOf('=');

            if (separator == -1)
                continue;

            QString settingName =
                line.left(separator).trimmed();

            QString settingValue =
                line.mid(separator + 1).trimmed();

            if (settingName == name)
            {
                key = settingValue;
                break;
            }
        }
    }

    QShortcut *shortcut = new QShortcut(
        QKeySequence(key),
        m_window
        );

    shortcut->setContext(
        Qt::WindowShortcut
        );

    connect(
        shortcut,
        &QShortcut::activated,
        this,
        action
        );
}