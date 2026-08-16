#include "mainwindow.h"
#include "shortcutmanager.h"
#include "editorsettings.h"
#include "appsettingspath.h"

#include <QPlainTextEdit>
#include <QMenu>
#include <QTextDocument>
#include <QContextMenuEvent>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QInputDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QWidget>
#include <QTextCursor>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QEvent>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    editor(new QPlainTextEdit(this)),
    shortcuts(new ShortcutManager(this, this)),
    editorSettings(new EditorSettings()),
    findBar(new QLineEdit(this))

{

    QWidget *centralWidget = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(editor);
    layout->addWidget(findBar);

    setCentralWidget(centralWidget);

    findBar->setPlaceholderText("Find...");
    findBar->hide();
    findBar->installEventFilter(this);

    editorSettings->applyWindowSize(this);

    connect(
        findBar,
        &QLineEdit::textChanged,
        this,
        [this](const QString &text)
        {
            if (text.isEmpty())
                return;

            findNext();
        }
        );

    connect(
        findBar,
        &QLineEdit::returnPressed,
        this,
        [this]()
        {
            findNext();
        }
        );

    editorSettings->applyTo(editor);

    connect(
        editor->document(),
        &QTextDocument::modificationChanged,
        this,
        [this](bool) {
            updateTitle();
        }
        );

    shortcuts->add(
        "new",
        "Ctrl+N",
        [this]() { newFile(); }
        );

    shortcuts->add(
        "open",
        "Ctrl+O",
        [this]() { openFile(); }
        );

    shortcuts->add(
        "save",
        "Ctrl+S",
        [this]() { saveFile(); }
        );

    shortcuts->add(
        "save_as",
        "Ctrl+Shift+S",
        [this]() { saveFileAs(); }
        );

    shortcuts->add(
        "quit",
        "Ctrl+Q",
        [this]() { close(); }
        );

    shortcuts->add(
        "undo",
        "Ctrl+Z",
        [this]() { editor->undo(); }
        );

    shortcuts->add(
        "redo",
        "Ctrl+Shift+Z",
        [this]() { editor->redo(); }
        );

    shortcuts->add(
        "cut",
        "Ctrl+X",
        [this]() { editor->cut(); }
        );

    shortcuts->add(
        "copy",
        "Ctrl+C",
        [this]() { editor->copy(); }
        );

    shortcuts->add(
        "paste",
        "Ctrl+V",
        [this]() { editor->paste(); }
        );

    shortcuts->add(
        "select_all",
        "Ctrl+A",
        [this]() { editor->selectAll(); }
        );

    shortcuts->add(
        "find",
        "Ctrl+F",
        [this]() { findText(); }
        );

    updateTitle();
}


MainWindow::~MainWindow()
{
}


void MainWindow::updateTitle()
{
    QString title;

    if (currentFile.isEmpty())
        title = "Untitled";
    else
        title = QFileInfo(currentFile).fileName();

    if (editor->document()->isModified())
        title += " *";

    setWindowTitle(title);
}


void MainWindow::newFile()
{
    if (!maybeSave())
        return;

    editor->clear();
    currentFile.clear();

    editor->document()->setModified(false);

    updateTitle();
}


void MainWindow::openFile()
{
    if (!maybeSave())
        return;

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File"
        );

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(
            this,
            "Open Failed",
            "Could not open the file."
            );

        return;
    }

    QTextStream stream(&file);

    editor->setPlainText(stream.readAll());

    currentFile = fileName;

    editor->document()->setModified(false);

    updateTitle();
}


bool MainWindow::saveFile()
{
    if (currentFile.isEmpty())
        return saveFileAs();

    QFile file(currentFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(
            this,
            "Save Failed",
            "Could not save the file."
            );

        return false;
    }

    QTextStream stream(&file);

    stream << editor->toPlainText();

    editor->document()->setModified(false);

    updateTitle();

    return true;
}


bool MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save File"
        );

    if (fileName.isEmpty())
        return false;

    currentFile = fileName;

    return saveFile();
}


bool MainWindow::maybeSave()
{
    if (!editor->document()->isModified())
        return true;

    QMessageBox::StandardButton result =
        QMessageBox::warning(
            this,
            "Unsaved Changes",
            "The document has unsaved changes.",
            QMessageBox::Save |
                QMessageBox::Discard |
                QMessageBox::Cancel
            );

    if (result == QMessageBox::Save)
        return saveFile();

    if (result == QMessageBox::Cancel)
        return false;

    return true;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!maybeSave())
    {
        event->ignore();
        return;
    }

    editorSettings->saveWindowSize(this);

    event->accept();
}


void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(
        "New",
        this,
        &MainWindow::newFile
        );

    menu.addAction(
        "Open...",
        this,
        &MainWindow::openFile
        );

    menu.addAction(
        "Save",
        this,
        &MainWindow::saveFile
        );

    menu.addAction(
        "Save As...",
        this,
        &MainWindow::saveFileAs
        );

    menu.addSeparator();

    menu.addAction(
        "Undo",
        editor,
        &QPlainTextEdit::undo
        );

    menu.addAction(
        "Redo",
        editor,
        &QPlainTextEdit::redo
        );

    menu.addSeparator();

    menu.addAction(
        "Cut",
        editor,
        &QPlainTextEdit::cut
        );

    menu.addAction(
        "Copy",
        editor,
        &QPlainTextEdit::copy
        );

    menu.addAction(
        "Paste",
        editor,
        &QPlainTextEdit::paste
        );

    menu.addAction(
        "Select All",
        editor,
        &QPlainTextEdit::selectAll
        );

    menu.addSeparator();

    menu.addAction(
        "Quit",
        this,
        &QWidget::close
        );

    menu.exec(event->globalPos());
}

void MainWindow::findText()
{
    findBar->show();
    findBar->setFocus();

    if (!editor->textCursor().selectedText().isEmpty())
    {
        findBar->setText(
            editor->textCursor().selectedText()
            );

        findBar->selectAll();
    }
}

void MainWindow::findNext()
{
    QString text = findBar->text();

    if (text.isEmpty())
        return;

    if (editor->find(text))
        return;

    QTextCursor cursor = editor->textCursor();

    cursor.movePosition(
        QTextCursor::Start
        );

    editor->setTextCursor(cursor);

    editor->find(text);
}

void MainWindow::closeFindBar()
{
    findBar->clear();
    findBar->hide();
    editor->setFocus();
}

bool MainWindow::eventFilter(
    QObject *watched,
    QEvent *event
    )
{
    if (
        watched == findBar &&
        event->type() == QEvent::KeyPress
        )
    {
        QKeyEvent *keyEvent =
            static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Escape)
        {
            closeFindBar();
            return true;
        }
    }

    return QMainWindow::eventFilter(
        watched,
        event
        );
}
