/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctCompositeCommandTarget.cpp: implementation of the ctCompositeCommandTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctCompositeCommandTarget.h"
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

const zString ctCompositeCommandTarget::m_Type = "CompositeCommandTarget";

const zString ctCompositeCommandTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctCompositeCommandTarget::m_ArgumentTotal = 1;

long ctCompositeCommandTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctCompositeCommandTarget(executionTime);}

	const zString id = ctCompositeCommandTarget::GetType();
    const long  argTotal = ctCompositeCommandTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctCompositeCommandTarget::ctCompositeCommandTarget(long executionTime) : xxCommand(executionTime),
																		 m_TargetLabel("")
{
}

ctCompositeCommandTarget::ctCompositeCommandTarget(const ctCompositeCommandTarget& oldCommand) : xxCommand(oldCommand),
													m_TargetLabel(oldCommand.m_TargetLabel)
{
	
}

// Constructor for use when creating the command internally.

ctCompositeCommandTarget::ctCompositeCommandTarget(long executionTime, const zString target) : xxCommand(executionTime),
													m_TargetLabel(target)
{
	
}

ctCompositeCommandTarget::~ctCompositeCommandTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// This command creates an empty composite command target to which 
// other command targets can be added later.
//
// Arguments
// *********
//
//	m_TargetLabel	Name of the created command target
//

zOutStream& ctCompositeCommandTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<TargetLabel>" << m_TargetLabel << "</TargetLabel>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_TargetLabel;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctCompositeCommandTarget::get(zInStream& is)
{
	is >> m_TargetLabel;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctCompositeCommandTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctCompositeCommandTarget::GetCommand() const
{
	return new ctCompositeCommandTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctCompositeCommandTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->CompositeCommandTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. We check that no existing
// command target has the specified name.

bool ctCompositeCommandTarget::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_TargetLabel))
		return ErrorTrace("Invalid command target name");
	else if(riData.IsCommandTargetPresent(m_TargetLabel))
		return ErrorTrace("Command target already exists");

	// New target is unique so add it to the container in the CInputData

	rincData.AddCommandTargetName(m_TargetLabel);

	return true;
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
// This command expects the following types:
//
// m_TargetLabel  - string

bool ctCompositeCommandTarget::Pack(const tguArgumentSequence& vArguments)
{
    if(vArguments.size() == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_TargetLabel);

        return true;
    }
    else
    {
        return false;
    }
}
