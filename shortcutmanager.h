#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <QObject>
#include <QString>
#include <functional>

class QWidget;

class ShortcutManager : public QObject
{
    Q_OBJECT

public:
    explicit ShortcutManager(
        QWidget *window,
        QObject *parent = nullptr
        );

    void add(
        const QString &name,
        const QString &defaultKey,
        std::function<void()> action
        );

private:
    QWidget *m_window;

    QString settingsPath() const;
};

#endif