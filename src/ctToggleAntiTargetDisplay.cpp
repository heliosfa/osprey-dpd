/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctToggleAntiTargetDisplay.cpp: implementation of the ctToggleAntiTargetDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctToggleAntiTargetDisplay.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ctToggleAntiTargetDisplay::m_Type = "ToggleAntiTargetDisplay";

const zString ctToggleAntiTargetDisplay::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctToggleAntiTargetDisplay::m_ArgumentTotal = 1;

long ctToggleAntiTargetDisplay::GetArgumentTotal()
{
	return m_ArgumentTotal;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new ctToggleAntiTargetDisplay(executionTime);}

	const zString id = ctToggleAntiTargetDisplay::GetType();
    const long  argTotal = ctToggleAntiTargetDisplay::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctToggleAntiTargetDisplay::ctToggleAntiTargetDisplay(long executionTime) : ctApplyCommand(executionTime)
{
}

ctToggleAntiTargetDisplay::ctToggleAntiTargetDisplay(const ctToggleAntiTargetDisplay& oldCommand) : ctApplyCommand(oldCommand)
{
}

// Constructor for use when creating the command internally.

ctToggleAntiTargetDisplay::ctToggleAntiTargetDisplay(long executionTime, const zString targetLabel) : ctApplyCommand(executionTime, targetLabel)
{
}

ctToggleAntiTargetDisplay::~ctToggleAntiTargetDisplay()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
// None - the target label is supplied by the ctApplyCommand base class

zOutStream& ctToggleAntiTargetDisplay::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ctApplyCommand::put(os);
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctToggleAntiTargetDisplay::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	return is;
}

// Non-static function to return the type of the command

const zString ctToggleAntiTargetDisplay::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctToggleAntiTargetDisplay::GetCommand() const
{
	return new ctToggleAntiTargetDisplay(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctToggleAntiTargetDisplay::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->ToggleAntiTargetDisplay(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. The only data is the name
// of a command target and this is validated in the base class.
//

bool ctToggleAntiTargetDisplay::IsDataValid(const CInputData& riData) const
{
	return ctApplyCommand::IsDataValid(riData);
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects.
//
// This command expects the following types (in addition to its base class types):
//
// m_TargetLabel  - string

bool ctToggleAntiTargetDisplay::Pack(const tguArgumentSequence& vArguments)
{
    if(vArguments.size() == GetArgumentTotal())
    {
        return ctApplyCommand::Pack(vArguments);
    }
    else
    {
        return false;
    }
}

