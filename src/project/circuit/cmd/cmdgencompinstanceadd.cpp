/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include "cmdgencompinstanceadd.h"
#include "../circuit.h"
#include "../genericcomponentinstance.h"

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdGenCompInstanceAdd::CmdGenCompInstanceAdd(Circuit& circuit, const QUuid& genComp,
                                             const QUuid& symbVar, UndoCommand* parent) throw (Exception) :
    UndoCommand(QCoreApplication::translate("CmdGenCompInstanceAdd", "Add generic component"), parent),
    mCircuit(circuit), mGenCompUuid(genComp), mSymbVarUuid(symbVar), mGenCompInstance(0)
{
    /*mGenComp = mProject.getLibrary().getGenericComponent(e->getGenCompUuid());
    if (!mGenComp)
    {
        throw RuntimeError(__FILE__, __LINE__, QString(),
            QString(tr("The generic component with the UUID \"%1\" was not found "
            "in the project's library.")).arg(e->getGenCompUuid().toString()));
    }

    mGenCompSymbVar = mGenComp->getSymbolVariantByUuid(e->getSymbVarUuid());
    if (!mGenCompSymbVar)
    {
        throw RuntimeError(__FILE__, __LINE__, QString(), QString(tr("The symbol "
            "variant with the UUID \"%1\" was not found in the generic component "
            "\"%2\".")).arg(e->getSymbVarUuid().toString(), e->getGenCompUuid().toString()));
    }*/
}

CmdGenCompInstanceAdd::~CmdGenCompInstanceAdd() noexcept
{
    if (!mIsExecuted)
        delete mGenCompInstance;
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

void CmdGenCompInstanceAdd::redo() throw (Exception)
{
    if (!mGenCompInstance) // only the first time
    {
        mGenCompInstance = mCircuit.createGenCompInstance(mGenCompUuid, mSymbVarUuid,
                           QString("TODO_%1").arg(qrand())); // throws an exception on error
    }

    mCircuit.addGenCompInstance(mGenCompInstance); // throws an exception on error

    try
    {
        UndoCommand::redo(); // throws an exception on error
    }
    catch (Exception &e)
    {
        mCircuit.removeGenCompInstance(mGenCompInstance);
        throw;
    }
}

void CmdGenCompInstanceAdd::undo() throw (Exception)
{
    mCircuit.removeGenCompInstance(mGenCompInstance); // throws an exception on error

    try
    {
        UndoCommand::undo();
    }
    catch (Exception& e)
    {
        mCircuit.addGenCompInstance(mGenCompInstance);
        throw;
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project