/***************************************************************************
                          qconsole.h  -  description
                             -------------------
    begin                : mar mar 15 2005
    copyright            : (C) 2005 by Houssem BDIOUI
    email                : houssem.bdioui@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QCONSOLE_H
#define QCONSOLE_H

#include "interceptor.h"
#include <qstringlist.h>
#if QT_VERSION >= 0x040000
#include <q3textedit.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <Q3PopupMenu>
#define QTEXTEDIT_CLASSNAME Q3TextEdit
#define QPOPUPMENU_CLASSNAME Q3PopupMenu
#else
#include <qtextedit.h>
#define QTEXTEDIT_CLASSNAME QTextEdit
#define QPOPUPMENU_CLASSNAME QPopupMenu
#endif

/**An abstract Qt console
 *@author Houssem BDIOUI
 */

class QConsole : protected  QTEXTEDIT_CLASSNAME
{
    Q_OBJECT
public:
    //constructor
    QConsole(QWidget *parent = NULL, const char *name = NULL, bool initInterceptor = true);
    //set the prompt of the console
    void setPrompt(QString prompt, bool display = true);
    //execCommand(QString) executes the command and displays back its result
    void execCommand(QString command, bool writeCommand = true, bool showPrompt = true);
    //saves a file script
    int saveScript(QString fileName);
    //loads a file script
    int loadScript(QString fileName);
    //clear & reset the console (useful sometimes)
    void clear();
    void reset();
    //cosmetic methods !
    void setCmdColor(QColor c) {cmdColor = c;};
    void setErrColor(QColor c) {errColor = c;};
    void setOutColor(QColor c) {outColor = c;};
    void setCompletionColor(QColor c) {completionColor = c;};
    void setFont(QFont f) {setCurrentFont(f);};

private:
    // Redefined virtual methods
    void contentsMouseReleaseEvent(QMouseEvent *e);
    void contentsMousePressEvent(QMouseEvent *e);
    void contentsMouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent * e);
    void paste();
    //Just to disable the popup menu
    QPOPUPMENU_CLASSNAME * createPopupMenu (const QPoint & pos);
    //Return false if the command is incomplete (e.g. unmatched braces)
    virtual bool isCommandComplete(QString command);
    //Get the command to validate
    QString getCurrentCommand();
    //Replace current command with a new one
    void replaceCurrentCommand(QString newCommand);
    //Test wether the cursor is in the edition zone
    bool isInEditionZone();
    //displays redirected stdout/stderr
    void stdReceived(QTextIStream *s);

//protected attributes
protected:
    //colors
    QColor cmdColor, errColor, outColor, completionColor;
    // Old cursor position
    int oldPara, oldIndex;
    // New cursor position !
    int newPara, newIndex;
    // cached prompt length
    int promptLength;
    // The prompt string
    QString prompt;
    // The commands history
    QStringList history;
    //Contains the commands that has succeeded
    QStringList recordedScript;
    // Current history index (needed because afaik QStringList does not have such an index)
    uint historyIndex;
    //Stdout interceptor
    Interceptor *stdoutInterceptor;
    //Stderr interceptor
    Interceptor *stderrInterceptor;
    //Holds the paragraph number of the prompt (useful for multi-line command handling)
    int promptParagraph;

protected:
    //execute a validated command (should be reimplemented and called at the end)
    //the return value of the function is the string result
    //res must hold back the return value of the command (0: passed; else: error)
    virtual QString interpretCommand(QString command, int *res);
    //give suggestions to autocomplete a command (should be reimplemented)
    //the return value of the function is the string list of all suggestions
    virtual QStringList autocompleteCommand(QString cmd);

// Redefined virtual slots
private slots:
    //Correctly handle the cursor when moved
    void moveCursor(CursorAction action, bool select);
    //Reimplemented method
    void removeSelectedText(int selNum = 0);
    //Redirect keyboard actions
    void doKeyboardAction (KeyboardAction action);
    //displays the prompt
    void displayPrompt();
    //Obsolete insert() slot, but still used intensively inside QTextEdit !
    void insert(const QString & text, bool indent, bool checkNewLine = TRUE, bool removeSelected = TRUE);

signals:
    //Signal emitted after that a command is executed
    void commandExecuted(QString command);
};

#endif
