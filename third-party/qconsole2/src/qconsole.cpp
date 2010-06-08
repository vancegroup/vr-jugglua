/***************************************************************************
                          qconsole.cpp  -  description
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

#include "qconsole.h"
#include <qfile.h>

#if QT_VERSION >= 0x040000
#include <QMouseEvent>
#include <QTextStream>
#include <QKeyEvent>
#include <Q3PopupMenu>
//for Q3TextCursor usage
#include "q3richtext_p.h"
#define QTEXTCURSOR_CLASSNAME Q3TextCursor
#define CONTROL_BUTTON ControlModifier
#define WRITE_ONLY QIODevice::WriteOnly
#else
//for QTextCursor usage
#include <private/qrichtext_p.h>
#define QTEXTCURSOR_CLASSNAME QTextCursor
#define CONTROL_BUTTON ControlButton
#define WRITE_ONLY IO_WriteOnly
#endif

//Clear the console
void QConsole::clear()
{
   QTEXTEDIT_CLASSNAME::clear();
}

//Reset the console
void QConsole::reset()
{
    clear();
    //set the style of the QTextEdit
    setTextFormat(Qt::PlainText);
    setCurrentFont(QFont("Courier"));
    //init attributes
    historyIndex = 0;
    history.clear();
    recordedScript.clear();
}

//QConsole constructor (init the QTextEdit & the attributes)
QConsole::QConsole(QWidget *parent, const char *name, bool initInterceptor) : QTEXTEDIT_CLASSNAME(parent,name),
   cmdColor(Qt::black), errColor(Qt::red), outColor(Qt::blue), completionColor(Qt::green),
   stdoutInterceptor(NULL), stderrInterceptor(NULL)
{
    //resets the console
    reset();

    if (initInterceptor)
    {
	//Initialize the interceptors
	stdoutInterceptor = new Interceptor(this);
	stdoutInterceptor->initialize(1);
	connect(stdoutInterceptor, SIGNAL(received(QTextIStream *)), SLOT(displayPrompt()));

	stderrInterceptor = new Interceptor(this);
	stderrInterceptor->initialize(2);
	connect(stderrInterceptor, SIGNAL(received(QTextIStream *)), SLOT(displayPrompt()));
    }
}

//Sets the prompt and cache the prompt length to optimize the processing speed
void QConsole::setPrompt(QString newPrompt, bool display)
{
    prompt = newPrompt;
    promptLength = prompt.length();
    //display the new prompt
    if (display)
        displayPrompt();
}

//Displays the prompt and move the cursor to the end of the line.
void QConsole::displayPrompt()
{
    //flush the stdout/stderr before displaying the prompt
    if (stdoutInterceptor)
    {
	setColor(outColor);
	stdReceived(stdoutInterceptor->textIStream());
    }
    if (stderrInterceptor)
    {
	setColor(errColor);
	stdReceived(stderrInterceptor->textIStream());
    }
    //displays the prompt
    setColor(cmdColor);
    append(prompt);
    QTEXTEDIT_CLASSNAME::moveCursor(QTEXTEDIT_CLASSNAME::MoveEnd, false);
    //Saves the paragraph number of the prompt
    promptParagraph = paragraphs() - 1;
}

//displays redirected stdout/stderr
void QConsole::stdReceived(QTextIStream *s)
{
    QString line;
#if QT_VERSION >= 0x040000
    line = s->readAll();
    append(line);
#else
    while (!s->atEnd())
    {
        line = s->readLine();
        append(line);
    }
#endif
}

//Correctly handle the cursor when moved according to the different actions
void QConsole::moveCursor(CursorAction action, bool select)
{
    int para, index;
    //save the old cursor position
    getCursorPosition(&para, &index );
    //if home pressed, move the cursor just after the prompt (if in the first line)
    //and select the covered text if needed
    if ( (action == QTEXTEDIT_CLASSNAME::MoveLineStart) && (promptParagraph == para) )
    {
        if (select)
            setSelection(para, index, para, promptLength);
        else
            setCursorPosition(para, promptLength);
        return;
    }
    //Process the up & down keys to navigate into the history (if not empty & if in the first line)
    else if ( ( (action == QTEXTEDIT_CLASSNAME::MoveDown) || (action == QTEXTEDIT_CLASSNAME::MoveUp) )
        && history.size() && (promptParagraph == para))
    {
        //update the historyIndex if up or down
        if ( (action == QTEXTEDIT_CLASSNAME::MoveDown) && (historyIndex + 1 < history.size()) )
            historyIndex ++;
        else if ((action == QTEXTEDIT_CLASSNAME::MoveUp) && historyIndex)
            historyIndex --;
        //replace the current command with the command found in the history
        replaceCurrentCommand(history[historyIndex]);
    }
    else
        QTEXTEDIT_CLASSNAME::moveCursor(action, select);
    //Undo the new position if it is out of the edition zone and unselect as well
    if (!isInEditionZone())
    {
        setCursorPosition(para, index);
        selectAll(false);
    }
}

//Redirect keyboard actions to perform text suppression and validation
//of the commands
void QConsole::doKeyboardAction(KeyboardAction action)
{
    //Get the current paragraph and the current cursor position
    int para, index;
    getCursorPosition(&para, &index );
    //Get the cursor
    QTEXTCURSOR_CLASSNAME *cursor = textCursor();
    switch (action)
    {
        //Don't delete the prompt if backspace is pressed
        case QTEXTEDIT_CLASSNAME::ActionBackspace:
        {
            if ( (promptParagraph == para) && (index == promptLength) )
                return;
        }
        break;

        //Don't delete the prompt if ctrl+backspace is pressed
        case QTEXTEDIT_CLASSNAME::ActionWordBackspace:
        {
            //trick to get the new position of the cursor
            cursor->gotoPreviousWord();
            //exit if the new position is out of the edition zone
            bool error = false;
            if ( !isInEditionZone())
                error = true;
            setCursorPosition(para, index);
            if (error)
                return;
        }
        break;

        // If return pressed, do the evaluation and append the result
        case QTEXTEDIT_CLASSNAME::ActionReturn:
        {
            //Get the command to validate
            QString command = getCurrentCommand();
            //execute the command and get back its text result and its return value
            if (isCommandComplete(command))
                execCommand(command, false);
            else
            {
                append("");
                QTEXTEDIT_CLASSNAME::moveCursor(QTEXTEDIT_CLASSNAME::MoveEnd, false);
            }
            return;
        }
        break;

        default:
            break;
    }
    //If we are here, this means that we can perform the action
    //by calling the parent implementation
    QTEXTEDIT_CLASSNAME::doKeyboardAction(action);
}

//Don't remove any text if the selection is not inside the edition zone
void QConsole::removeSelectedText(int selNum)
{
    //get where the current selection starts and where it ends
    int paraFrom, indexFrom, paraTo, indexTo;
    getSelection( &paraFrom, &indexFrom, &paraTo, &indexTo, selNum);
    //check if the selection is inside the selection zone and then delete it
    //else unselect
    if (isInEditionZone() && ((paraFrom > promptParagraph) ||
        ( (paraFrom == promptParagraph) && (indexFrom >= promptLength) )) )
        QTEXTEDIT_CLASSNAME::removeSelectedText(selNum);
    else
        selectAll(false);
}

//Reimplemented mouse press event
void QConsole::contentsMousePressEvent( QMouseEvent *e )
{
    //Saves the old position of the cursor before any mouse click
    getCursorPosition(&oldPara, &oldIndex );
    //Call the parent implementation
    QTEXTEDIT_CLASSNAME::contentsMousePressEvent( e );
    //Saves the new position of the cursor
    getCursorPosition(&newPara, &newIndex );
}

//Reimplemented mouse release event
void QConsole::contentsMouseReleaseEvent( QMouseEvent *e )
{
    //Call the parent implementation
    QTEXTEDIT_CLASSNAME::contentsMouseReleaseEvent( e );
    //Undo the new cursor position if it is out of the edition zone
    if (!isInEditionZone())
        setCursorPosition(oldPara, oldIndex );
}

//Reimplemented mouse dblclk event
void QConsole::contentsMouseDoubleClickEvent( QMouseEvent *e )
{
    setCursorPosition(newPara, newIndex );
    //Call the parent implementation
    QTEXTEDIT_CLASSNAME::contentsMouseDoubleClickEvent( e );
}

//give suggestions to autocomplete a command (should be reimplemented)
//the return value of the function is the string list of all suggestions
QStringList QConsole::autocompleteCommand(QString)
{
    return QStringList();
}
  
//Reimplemented key press event
void QConsole::keyPressEvent( QKeyEvent *e )
{
    //If Ctrl + C pressed, then undo the current command
    if ( (e->key() == Qt::Key_C) && (e->state() == Qt::CONTROL_BUTTON) )
        displayPrompt();
    //Treat the tab key & autocomplete the current command
    else if (e->key() == Qt::Key_Tab)
    {
        QString command = getCurrentCommand();
        QStringList sl = autocompleteCommand(command);
        QString str = sl.join(" ");
        if (sl.count() == 1)
            replaceCurrentCommand(sl[0] + " ");
        else if (sl.count() > 1)
        {
            setColor(completionColor);
            append(sl.join(" "));
            setColor(cmdColor);
            displayPrompt();
            QTEXTEDIT_CLASSNAME::insert(command);
        }
    }
    else
        QTEXTEDIT_CLASSNAME::keyPressEvent( e );
}

//Get the current command
QString QConsole::getCurrentCommand()
{
    //Get the current command: we just remove the prompt
    setSelection(promptParagraph, promptLength, paragraphs()-1, paragraphLength(paragraphs()-1));
    QString command = selectedText();
    selectAll(false);
    return command;
}

//Replace current command with a new one
void QConsole::replaceCurrentCommand(QString newCommand)
{
    setSelection(promptParagraph, promptLength, paragraphs()-1, paragraphLength(paragraphs()-1));
    QTEXTEDIT_CLASSNAME::insert(newCommand);
}

//default implementation: command always complete
bool QConsole::isCommandComplete(QString )
{
    return true;
}

//Tests whether the cursor is in th edition zone or not (after the prompt
//or in the next lines (in case of multi-line mode)
bool QConsole::isInEditionZone()
{
    int para, index;
    getCursorPosition(&para, &index );
    return (para > promptParagraph) || ( (para == promptParagraph) && (index >= promptLength) );
}

//Basically, puts the command into the history list
//And emits a signal (should be called by reimplementations)
QString QConsole::interpretCommand(QString command, int *res)
{
    //Add the command to the recordedScript list
    if (!*res)
        recordedScript.append(command);
    //update the history and its index
    history.append(command.replace("\n", "\\n"));
    historyIndex = history.size();
    //emit the commandExecuted signal
    emit commandExecuted(command);
    return "";
}

//execCommand(QString) executes the command and displays back its result
void QConsole::execCommand(QString command, bool writeCommand, bool showPrompt)
{
    //Display the prompt with the command first
    if (writeCommand)
    {
        if (getCurrentCommand() != "")
            displayPrompt();
        QTEXTEDIT_CLASSNAME::insert(command);
    }
    //execute the command and get back its text result and its return value
    int res;
    QString strRes = interpretCommand(command, &res);
    //According to the return value, display the result either in red or in blue
    if (res == 0)
        setColor(outColor);
    else
        setColor(errColor);
    append(strRes);
    //Display the prompt again
    if (showPrompt)
        displayPrompt();
}

//saves a file script
int QConsole::saveScript(QString fileName)
{
    QFile f(fileName);
    if (!f.open(WRITE_ONLY))
        return -1;
    QTextStream ts(&f);
    for ( QStringList::Iterator it = recordedScript.begin(); it != recordedScript.end(); ++it)
        ts << *it << "\n";

    f.close();
    return 0;
}

//loads a file script
int QConsole::loadScript(QString fileName)
{
    QFile f(fileName);
    if (!f.open(IO_ReadOnly))
        return -1;
    QTextStream ts(&f);
    QString command;
    while(true)
    {
        command=ts.readLine();
        if (command.isNull())
           break; //done
        execCommand(command, true, false);
    }
    f.close();
    return 0;
}

//redefined insert() slot to avoid inserting text outside of the
//edition zone: Fixes a bug that happens when holding the mouse button
//pressed outside the edition zone and typing something
void QConsole::insert(const QString & text, bool indent, bool checkNewLine, bool removeSelected)
{
    if (isInEditionZone())
        QTEXTEDIT_CLASSNAME::insert(text, indent, checkNewLine, removeSelected);
}

//Just disable the popup menu
QPOPUPMENU_CLASSNAME * QConsole::createPopupMenu (const QPoint &)
{
    return NULL;
}

//Allows pasting with middle mouse button (x window)
//when clicking outside of the edition zone
void QConsole::paste()
{
    setCursorPosition(oldPara, oldIndex );
    QTEXTEDIT_CLASSNAME::paste();
}
