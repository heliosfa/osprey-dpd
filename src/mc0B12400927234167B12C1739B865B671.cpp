/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mc0B12400927234167B12C1739B865B671.cpp: implementation of the mc0B12400927234167B12C1739B865B671 class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mc0B12400927234167B12C1739B865B671.h"
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

const zString mc0B12400927234167B12C1739B865B671::m_Type = "mc0B12400927234167B12C1739B865B671";

const zString mc0B12400927234167B12C1739B865B671::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new mc0B12400927234167B12C1739B865B671(executionTime);}

	const zString id = mc0B12400927234167B12C1739B865B671::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mc0B12400927234167B12C1739B865B671::mc0B12400927234167B12C1739B865B671(long executionTime) : xxCommand(executionTime)
{
}

mc0B12400927234167B12C1739B865B671::mc0B12400927234167B12C1739B865B671(const mc0B12400927234167B12C1739B865B671& oldCommand) : xxCommand(oldCommand)
{
}

mc0B12400927234167B12C1739B865B671::~mc0B12400927234167B12C1739B865B671()
{
}

// Member functions to read/write the data specific to the command.
// Note that the xxCommand base class does not provide an implementation of
// the put() and get() functions, but it does provide helper functions to
// write the start and end tags for the XML command output. The flag showing
// if the output is to be written as ASCII or XML is set in SimXMLFlags.h that is
// #included in the xxCommand.h header file, so  it is visible to all command classes.
//
// Arguments
// *********
//
//

zOutStream& mc0B12400927234167B12C1739B865B671::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mc0B12400927234167B12C1739B865B671::get(zInStream& is)
{
	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
//
// Note that even though this command is destined for the IMonitor interface, we
// have to pass it to the ISimCmd interface first because it will be checked for
// execution in the CSimBox's command loop and then passed on to the CMonitor.

bool mc0B12400927234167B12C1739B865B671::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->RunGud(this);
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mc0B12400927234167B12C1739B865B671::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mc0B12400927234167B12C1739B865B671::GetCommand() const
{
	return new mc0B12400927234167B12C1739B865B671(*this);
}

// Function to validate the command arguments: not applicable to this command.

bool mc0B12400927234167B12C1739B865B671::IsDataValid(const CInputData &riData) const
{
	return true;
}
