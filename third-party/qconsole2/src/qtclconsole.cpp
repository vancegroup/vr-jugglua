/***************************************************************************
                          qtclconsole.cpp  -  description
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

#include "qtclconsole.h"
#include "commands.h"
#include "commandsManager.h"

//callback method that implements the history command
int QtclConsole::showHistory( ClientData, Tcl_Interp* interp, int argc, const char *[])
{
    // Reset result data
    Tcl_ResetResult(interp);

    //Help message in case of wrong parameters
    if (argc != 1)
    {
        Tcl_AppendResult(interp, "Usage: history\n", (char*) NULL);
        return TCL_ERROR;
    }

    uint index = 1;
    for ( QStringList::Iterator it = history.begin(); it != history.end(); ++it )
    {
        Tcl_AppendResult(interp, QString("%1\t%2\n").arg(index).arg(*it).ascii(), (char*) NULL);
        index ++;
    }

    return TCL_OK;
}

//callback method that implements the set_prompt command
int QtclConsole::setPrompt( ClientData, Tcl_Interp* interp, int argc, const char *argv[])
{
    // Reset result data
    Tcl_ResetResult(interp);

    //Help message in case of wrong parameters
    if (argc != 2)
    {
        Tcl_AppendResult(interp, "Usage: set_prompt newPrompt\n", (char*) NULL);
        return TCL_ERROR;
    }

    QConsole::setPrompt(argv[1], false);

    return TCL_OK;
}

//callback method that calls the saveScript() method
int QtclConsole::saveScript( ClientData, Tcl_Interp* interp, int argc, const char *argv[])
{
    // Reset result data
    Tcl_ResetResult(interp);

    //Help message in case of wrong parameters
    if (argc != 2)
    {
        Tcl_AppendResult(interp, "Usage: save_script script_name\n", (char*) NULL);
        return TCL_ERROR;
    }

    if (!QConsole::saveScript(argv[1]))
        return TCL_ERROR;
    else
        return TCL_OK;
}

QtclConsole *QtclConsole::theInstance = NULL;

QtclConsole *QtclConsole::getInstance(QWidget *parent, const char *name)
{
    if (!theInstance)
        theInstance = new QtclConsole(parent, name);
    return theInstance;
}

//QTcl console constructor (init the QTextEdit & the attributes)
QtclConsole::QtclConsole(QWidget *parent, const char *name) : QConsole(parent,name)
{
    //Register the msgbox command
    TclCallBack<QtclConsole>::registerMethod(this, "history", &QtclConsole::showHistory, "Shows the commands history");

    //Register the set_prompt command
    TclCallBack<QtclConsole>::registerMethod(this, "set_prompt", &QtclConsole::setPrompt, "Set a new prompt");

    //Register the set_prompt command
    TclCallBack<QtclConsole>::registerMethod(this, "save_script", &QtclConsole::saveScript, "Saves a script of executed commands");

    //Get the Tcl interpreter
    interp = commandsManager::getInstance()->tclInterp();

    //set the Tcl Prompt
    QConsole::setPrompt("QTcl shell> ");
}

//Destructor
QtclConsole::~QtclConsole()
{
    //unregister all the methods
    TclCallBack<QtclConsole>::unregisterAll();
}

//Call the TCL interpreter to execute the command
//And retrieve back the result
QString QtclConsole::interpretCommand(QString command, int *res)
{
    if (!command.isEmpty())
    {
        //Do the Tcl evaluation
        *res = Tcl_Eval( interp, command.ascii() );
        //Get the string result of the executed command
        const char * result = Tcl_GetString( Tcl_GetObjResult( interp ) );
        //Call the parent implementation
        QConsole::interpretCommand(command, res);
        return result;
    }
    else
        return "";
}

bool QtclConsole::isCommandComplete(QString command)
{
    //return Tcl_CommandComplete(command) && (!command.stripWhiteSpace().endsWith("\\"));
    return Tcl_CommandComplete(command);
}

QStringList QtclConsole::autocompleteCommand(QString cmd)
{
    int res = Tcl_Eval( interp, "info commands [join {" + cmd + "*}]" );
    if (!res)
    {
        //Get the string result of the executed command
        const char * result = Tcl_GetString( Tcl_GetObjResult( interp ) );
        return QStringList::split(" ", result);
    }
    else
        return QStringList();
}
