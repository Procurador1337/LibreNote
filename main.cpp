#include "mainwindow.h"
#include "appsettingspath.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>


void createDefaultSettings()
{
    QString path = appSettingsPath();

    if (QFile::exists(path))
        return;

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << "# Keyboard shortcuts\n";
    out << "new=Ctrl+N\n";
    out << "open=Ctrl+O\n";
    out << "save=Ctrl+S\n";
    out << "save_as=Ctrl+Shift+S\n";
    out << "quit=Ctrl+Q\n";
    out << "undo=Ctrl+Z\n";
    out << "redo=Ctrl+Shift+Z\n";
    out << "cut=Ctrl+X\n";
    out << "copy=Ctrl+C\n";
    out << "paste=Ctrl+V\n";
    out << "select_all=Ctrl+A\n";
    out << "find=Ctrl+F\n";
    out << "\n";

    out << "# Editor appearance\n";
    out << "background=#1e1e1e\n";
    out << "foreground=#d4d4d4\n";
    out << "font=Monospace\n";
    out << "font_size=14\n";
    out << "tab_size=4\n";
    out << "word_wrap=false\n";

    out << "\n";
    out << "# Window\n";
    out << "window_width=1000\n";
    out << "window_height=700\n";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    createDefaultSettings();

    MainWindow window;
    window.show();

    return app.exec();
}