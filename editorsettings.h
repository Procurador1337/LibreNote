#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QString>

class QPlainTextEdit;
class QMainWindow;

class EditorSettings
{
public:
    EditorSettings();

    void applyTo(QPlainTextEdit *editor) const;

    void applyWindowSize(QMainWindow *window) const;
    void saveWindowSize(QMainWindow *window);

private:
    QString settingsPath() const;

    QString value(
        const QString &name,
        const QString &defaultValue
        ) const;

    QString background() const;
    QString foreground() const;
    QString font() const;

    int fontSize() const;
    int tabSize() const;

    bool wordWrap() const;

    int windowWidth() const;
    int windowHeight() const;
};

#endif