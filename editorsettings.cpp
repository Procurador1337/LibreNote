#include "editorsettings.h"
#include "appsettingspath.h"

#include <QFile>
#include <QTextStream>
#include <QPlainTextEdit>
#include <QFont>
#include <QFontMetricsF>
#include <QPalette>
#include <QColor>
#include <QMainWindow>
#include <QStringList>


EditorSettings::EditorSettings()
{
}

QString EditorSettings::settingsPath() const
{
    return appSettingsPath();
}

QString EditorSettings::value(
    const QString &name,
    const QString &defaultValue
    ) const
{
    QFile file(settingsPath());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return defaultValue;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty() || line.startsWith('#'))
            continue;

        int separator = line.indexOf('=');

        if (separator == -1)
            continue;

        QString key =
            line.left(separator).trimmed();

        if (key != name)
            continue;

        return line.mid(separator + 1).trimmed();
    }

    return defaultValue;
}

QString EditorSettings::background() const
{
    return value(
        "background",
        "#1e1e1e"
        );
}

QString EditorSettings::foreground() const
{
    return value(
        "foreground",
        "#d4d4d4"
        );
}

QString EditorSettings::font() const
{
    return value(
        "font",
        "Monospace"
        );
}

int EditorSettings::fontSize() const
{
    return value(
               "font_size",
               "14"
               ).toInt();
}

int EditorSettings::tabSize() const
{
    return value(
               "tab_size",
               "4"
               ).toInt();
}

bool EditorSettings::wordWrap() const
{
    return value(
               "word_wrap",
               "false"
               ).toLower() == "true";
}

void EditorSettings::applyTo(QPlainTextEdit *editor) const
{
    if (!editor)
        return;

    QFont editorFont(font());

    editorFont.setPointSize(
        fontSize()
        );

    editor->setFont(editorFont);

    QPalette palette = editor->palette();

    palette.setColor(
        QPalette::Base,
        QColor(background())
        );

    palette.setColor(
        QPalette::Text,
        QColor(foreground())
        );

    editor->setPalette(palette);

    qreal characterWidth =
        QFontMetricsF(editorFont)
            .horizontalAdvance(' ');

    editor->setTabStopDistance(
        characterWidth * tabSize()
        );

    editor->setLineWrapMode(
        wordWrap()
            ? QPlainTextEdit::WidgetWidth
            : QPlainTextEdit::NoWrap
        );
}

int EditorSettings::windowWidth() const
{
    return value(
               "window_width",
               "1000"
               ).toInt();
}

int EditorSettings::windowHeight() const
{
    return value(
               "window_height",
               "700"
               ).toInt();
}

void EditorSettings::applyWindowSize(QMainWindow *window) const
{
    if (!window)
        return;

    window->resize(
        windowWidth(),
        windowHeight()
        );
}

void EditorSettings::saveWindowSize(QMainWindow *window)
{
    if (!window)
        return;

    QFile file(settingsPath());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList lines;

    QTextStream in(&file);

    while (!in.atEnd())
        lines.append(in.readLine());

    file.close();

    for (QString &line : lines)
    {
        if (line.startsWith("window_width="))
            line = "window_width=" +
                   QString::number(window->width());

        if (line.startsWith("window_height="))
            line = "window_height=" +
                   QString::number(window->height());
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (const QString &line : lines)
        out << line << '\n';
}
