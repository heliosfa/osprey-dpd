/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentToStringSequence.cpp: implementation of the cgcSetArgumentToStringSequence class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentToStringSequence.h"
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

const zString cgcSetArgumentToStringSequence::m_Type = "SetArgumentToStringSequence";

const zString cgcSetArgumentToStringSequence::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long cgcSetArgumentToStringSequence::m_ArgumentTotal = 4;

long cgcSetArgumentToStringSequence::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgcSetArgumentToStringSequence(executionTime);}

	const zString id = cgcSetArgumentToStringSequence::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentToStringSequence::cgcSetArgumentToStringSequence(long executionTime) : xxCommand(executionTime), 
                                            m_GroupName(""), m_CommandIndex(0),
                                            m_ArgName(""), m_Root("")
{
}

// Constructor for use when creating the command internally.

cgcSetArgumentToStringSequence::cgcSetArgumentToStringSequence(long executionTime, 
                         const zString groupName, long cmdIndex, const zString argName, 
                         const zString root) : xxCommand(executionTime),
						 m_GroupName(groupName), m_CommandIndex(cmdIndex),
						 m_ArgName(argName), m_Root(root)
{
}

// Copy constructor
cgcSetArgumentToStringSequence::cgcSetArgumentToStringSequence(const cgcSetArgumentToStringSequence& oldCommand) : xxCommand(oldCommand),
										   m_GroupName(oldCommand.m_GroupName),
										   m_CommandIndex(oldCommand.m_CommandIndex),
										   m_ArgName(oldCommand.m_ArgName),
										   m_Root(oldCommand.m_Root)
{
}

cgcSetArgumentToStringSequence::~cgcSetArgumentToStringSequence()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_GroupName		  Command group being modified
//  m_CommandIndex    Index of the command being modified
//  m_ArgName         Placeholder name of the argument to modify
//  m_Root            Prefix defining the modified argument

zOutStream& cgcSetArgumentToStringSequence::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CommandGroupName>" << m_GroupName    << "</CommandGroupName>" << zEndl;
	os << "<CommandIndex>"     << m_CommandIndex << "</CommandIndex>"     << zEndl;
	os << "<ArgumentName>"     << m_ArgName      << "</ArgumentName>"     << zEndl;
	os << "<ArgumentPrefix>"   << m_Root         << "</ArgumentPrefix>"   << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_GroupName << " " << m_CommandIndex << " " << m_ArgName << " " << m_Root << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& cgcSetArgumentToStringSequence::get(zInStream& is)
{
	is >> m_GroupName >> m_CommandIndex >> m_ArgName >> m_Root;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString cgcSetArgumentToStringSequence::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgcSetArgumentToStringSequence::GetCommand() const
{
	return new cgcSetArgumentToStringSequence(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgcSetArgumentToStringSequence::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. 

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->SetArgumentToStringSequence(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the names of the command group and its contained command
// are valid. We also check that the command group exists. The test on the
// names of the argument and its new value are performed in the implementation class.

bool cgcSetArgumentToStringSequence::IsDataValid(const CInputData& riData) const
{
	// Check group name is unique and consists only of allowed characters.

	if(!riData.IsExternalNameValid(m_GroupName))
		return ErrorTrace("Invalid name for a command group being modified");
	else if(m_CommandIndex < 1)
		return ErrorTrace("Invalid command index for command group being modified");
	else if(m_Root.empty())
		return ErrorTrace("Invalid prefix for command group's string sequence");

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
// m_Root           zString

bool cgcSetArgumentToStringSequence::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_GroupName);
        vArguments.at(1)->GetValue(&m_CommandIndex);
        vArguments.at(2)->GetValue(&m_ArgName);
        vArguments.at(3)->GetValue(&m_Root);

        return true;
    }
    else
    {
        return false;
    }

#endif
}

