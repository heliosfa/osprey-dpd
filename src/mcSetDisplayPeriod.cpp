/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetDisplayPeriod.cpp: implementation of the mcSetDisplayPeriod class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetDisplayPeriod.h"
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

const zString mcSetDisplayPeriod::m_Type = "SetDisplayPeriod";

const zString mcSetDisplayPeriod::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSetDisplayPeriod(executionTime);}

	const zString id = mcSetDisplayPeriod::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetDisplayPeriod::mcSetDisplayPeriod(long executionTime) : xxCommand(executionTime),
										m_Period(0)
{
}

mcSetDisplayPeriod::mcSetDisplayPeriod(const mcSetDisplayPeriod& oldCommand) : xxCommand(oldCommand),
										m_Period(oldCommand.m_Period)
{
}

mcSetDisplayPeriod::mcSetDisplayPeriod(long executionTime, bool bLog, long period) : xxCommand(executionTime, bLog),
										m_Period(period)
{
}

mcSetDisplayPeriod::~mcSetDisplayPeriod()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_Period		Number of timesteps between current states
//

zOutStream& mcSetDisplayPeriod::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<Period>" << m_Period << "</Period>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Period;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSetDisplayPeriod::get(zInStream& is)
{
	is >> m_Period;

	if(!is.good() || m_Period < 1)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString mcSetDisplayPeriod::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSetDisplayPeriod::GetCommand() const
{
	return new mcSetDisplayPeriod(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcSetDisplayPeriod::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetDisplayPeriod(this);
		return true;
	}
	else
		return false;
}

// Function to check that the display period is valid: this has been
// validated above.

bool mcSetDisplayPeriod::IsDataValid(const CInputData& riData) const
{

	return true;
}
