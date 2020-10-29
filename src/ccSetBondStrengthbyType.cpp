/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSetBondStrengthbyType.cpp: implementation of the ccSetBondStrengthbyType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSetBondStrengthbyType.h"
#include "ISimCmd.h"
#include "InputData.h"
#include "Builder.h"		// Needed for pi

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccSetBondStrengthbyType::m_Type = "SetBondStrengthbyType";

const zString ccSetBondStrengthbyType::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSetBondStrengthbyType(executionTime);}

	const zString id = ccSetBondStrengthbyType::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSetBondStrengthbyType::ccSetBondStrengthbyType(long executionTime) : xxCommand(executionTime),
									m_Name("Unknown"), m_BondType(-1),
									m_SpringConstant(0.0), m_UnstretchedLength(0.0)
{
}

ccSetBondStrengthbyType::ccSetBondStrengthbyType(const ccSetBondStrengthbyType& oldCommand) : xxCommand(oldCommand),
									 m_Name(oldCommand.m_Name), m_BondType(oldCommand.m_BondType),
									 m_SpringConstant(oldCommand.m_SpringConstant),
									 m_UnstretchedLength(oldCommand.m_UnstretchedLength)
{
}

ccSetBondStrengthbyType::~ccSetBondStrengthbyType()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	BondType			Sequential identifier for all bond types
//	Spring Constant		New value for the bond's Hookean spring constant
//	Phi0				New value for the bond's unstretched length

zOutStream& ccSetBondStrengthbyType::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<BondType>"			<< m_BondType			<< "</BondType>"		  << zEndl;
	os << "<SpringConstant>"	<< m_SpringConstant		<< "</SpringConstant>"	  << zEndl;
	os << "<UnStretchedLength>" << m_UnstretchedLength	<< "</UnStretchedLength>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_BondType << " " << m_SpringConstant << " " << m_UnstretchedLength;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSetBondStrengthbyType::get(zInStream& is)
{
	// Check that the spring constant and unstretched length are positive.
	//
	// We can only check that the specified bond type is non-negative here,
	// and the ValidateData() function cannot do any further checks because 
	// the polymerisation bonds required for the initial state have not been
	// created at the time CInputData tries to validate the commands. Hence, we
	// just have to assume the bond type exists and ignore the command if it does not.

	is >> m_BondType >> m_SpringConstant >> m_UnstretchedLength;

	if(!is.good() || m_BondType < 0 || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ccSetBondStrengthbyType::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSetBondStrengthbyType::GetCommand() const
{
	return new ccSetBondStrengthbyType(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSetBondStrengthbyType::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetBondStrengthbyType(this);
		return true;
	}
	else
		return false;
}

// Function to check that the name of the CBond object is valid. Note that 
// we have already validated the spring constant and unstretched length above.

bool ccSetBondStrengthbyType::IsDataValid(const CInputData& riData) const
{
	// Check target name is unique and consists only of allowed characters.

//	if(!riData.IsExternalNameValid(m_Name))
//		return ErrorTrace("Invalid Bond name");
//	else if(!riData.IsBondinMap(m_Name))
//		return ErrorTrace("Bond not found in map");

	return true;
}
