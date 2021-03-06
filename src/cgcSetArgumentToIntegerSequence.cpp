/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentToIntegerSequence.cpp: implementation of the cgcSetArgumentToIntegerSequence class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentToIntegerSequence.h"
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

const zString cgcSetArgumentToIntegerSequence::m_Type = "SetArgumentToIntegerSequence";

const zString cgcSetArgumentToIntegerSequence::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long cgcSetArgumentToIntegerSequence::m_ArgumentTotal = 5;

long cgcSetArgumentToIntegerSequence::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgcSetArgumentToIntegerSequence(executionTime);}

	const zString id = cgcSetArgumentToIntegerSequence::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentToIntegerSequence::cgcSetArgumentToIntegerSequence(long executionTime) : xxCommand(executionTime), 
                                            m_GroupName(""), m_CommandIndex(0),
                                            m_ArgName(""), m_InitialValue(0), m_Increment(0)
{
}

// Constructor for use when creating the command internally.

cgcSetArgumentToIntegerSequence::cgcSetArgumentToIntegerSequence(long executionTime, 
                         const zString groupName, long cmdIndex, const zString argName, 
                         long initialValue, long increment) : xxCommand(executionTime),
						 m_GroupName(groupName), m_CommandIndex(cmdIndex),
						 m_ArgName(argName), m_InitialValue(initialValue), m_Increment(increment)
{
}

// Copy constructor
cgcSetArgumentToIntegerSequence::cgcSetArgumentToIntegerSequence(const cgcSetArgumentToIntegerSequence& oldCommand) : xxCommand(oldCommand),
										   m_GroupName(oldCommand.m_GroupName),
										   m_CommandIndex(oldCommand.m_CommandIndex),
										   m_ArgName(oldCommand.m_ArgName),
										   m_InitialValue(oldCommand.m_InitialValue), 
										   m_Increment(oldCommand.m_Increment)
{
}

cgcSetArgumentToIntegerSequence::~cgcSetArgumentToIntegerSequence()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	GroupName		Command group being modified
//  CommandIndex    Index of the command being modified
//  ArgName         Placeholder name of the argument to modify
//  InitialValue    Initial integer value of the modified argument
//  Increment       Integer increment for the sequence

zOutStream& cgcSetArgumentToIntegerSequence::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CommandGroupName>"     << m_GroupName    << "</CommandGroupName>"    << zEndl;
	os << "<CommandIndex>"         << m_CommandIndex << "</CommandIndex>"        << zEndl;
	os << "<ArgumentName>"         << m_ArgName      << "</ArgumentName>"        << zEndl;
	os << "<IntegerInitialValue>"  << m_InitialValue << "</IntegerInitialValue>" << zEndl;
	os << "<IntegerIncrement>"     << m_Icrement     << "</IntegerIncrement>"    << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_GroupName << " " << m_CommandIndex << " " << m_ArgName << " " << m_InitialValue << " " << m_Increment << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& cgcSetArgumentToIntegerSequence::get(zInStream& is)
{
	is >> m_GroupName >> m_CommandIndex >> m_ArgName >> m_InitialValue >> m_Increment;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString cgcSetArgumentToIntegerSequence::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgcSetArgumentToIntegerSequence::GetCommand() const
{
	return new cgcSetArgumentToIntegerSequence(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgcSetArgumentToIntegerSequence::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. 

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->SetArgumentToIntegerSequence(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the names of the command group and its contained command
// are valid. Note that we allow negative and zero values for the initial value and 
// increment.

bool cgcSetArgumentToIntegerSequence::IsDataValid(const CInputData& riData) const
{
	// Check group name is unique and consists only of allowed characters.

	if(!riData.IsExternalNameValid(m_GroupName))
		return ErrorTrace("Invalid name for a command group being modified");
	if(!riData.IsExternalNameValid(m_ArgName))
		return ErrorTrace("Invalid name for an argument in a command group being modified");
	else if(m_CommandIndex < 1)
		return ErrorTrace("Invalid command index for command group being modified");

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
// This command expects the following types (in addition to its base class types):
//
// m_GrouName       zString
// m_CommandIndex   long
// m_ArgName        zString
// m_InitialValue   double
// m_Increment      double

bool cgcSetArgumentToIntegerSequence::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_GroupName);
        vArguments.at(1)->GetValue(&m_CommandIndex);
        vArguments.at(2)->GetValue(&m_ArgName);
        vArguments.at(3)->GetValue(&m_InitialValue);
        vArguments.at(4)->GetValue(&m_Increment);

        return true;
    }
    else
    {
        return false;
    }

#endif
}

