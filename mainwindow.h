#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editorsettings.h"
#include "shortcutmanager.h"


class QPlainTextEdit;
class QCloseEvent;
class QContextMenuEvent;
class ShortcutManager;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadFile(const QString &path); 

protected:
    void closeEvent(QCloseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPlainTextEdit *editor;
    ShortcutManager *shortcuts;
    EditorSettings *editorSettings;
    QLineEdit *findBar;
    QString currentFile;

    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    bool maybeSave();
    void findText();
    void closeFindBar();
    void findNext();
    void updateTitle();


    bool eventFilter(
        QObject *watched,
        QEvent *event
        ) override;
};

#endif
