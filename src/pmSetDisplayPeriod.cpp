/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSetDisplayPeriod.cpp: implementation of the pmSetDisplayPeriod class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSetDisplayPeriod.h"
#include "mcSetDisplayPeriod.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSetDisplayPeriod::m_Type = "SetDisplayPeriod";

const zString pmSetDisplayPeriod::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// message class but a global function hidden in the namespace.

namespace
{
	mpmMessage* Create() {return new pmSetDisplayPeriod();}

	const zString id = pmSetDisplayPeriod::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmSetDisplayPeriod::pmSetDisplayPeriod() : mpmCommandWrapper(), m_Period(0)
{
}

pmSetDisplayPeriod::pmSetDisplayPeriod(const pmSetDisplayPeriod& oldMessage) : mpmCommandWrapper(oldMessage),
                                       m_Period(oldMessage.m_Period)
{
}

pmSetDisplayPeriod::~pmSetDisplayPeriod()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSetDisplayPeriod::put(zOutStream& os) const
{
#if EnableXMLParallelMessagess == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<MessageId>"    << GetId()   << "</MessageId>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLParallelMessagess == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << GetId();
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& pmSetDisplayPeriod::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSetDisplayPeriod::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSetDisplayPeriod::GetMessage() const
{
	return new pmSetDisplayPeriod(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmSetDisplayPeriod::SetMessageData(const xxCommand* const pCommand)
{
	const mcSetDisplayPeriod* const pCmd = dynamic_cast<const mcSetDisplayPeriod*>(pCommand);

    if(pCmd)
    {
        m_Period = pCmd->GetPeriod();
    }
    else
    {
        m_Period = 0; // This illegal value is caught by the Validate function below
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmSetDisplayPeriod::Validate()
{
    bool bSuccess = true;

    // The display period must be a positive integer

    if(m_Period < 1)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSetDisplayPeriod::SendAllP()
{
#if SimMPS == SimulationEnabled

    // We send the single long value to all the other processors: note that 
	// we assume that the sending processor is P0, so we start the loop at 
	// processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(&m_Period, 1, MPI_LONG, m_ProcId, m_Tag, MPI_COMM_WORLD);
    }

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

// We retrieve the data from the message and fill up this instance's member variables. 
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

void pmSetDisplayPeriod::Receive()
{
#if SimMPS == SimulationEnabled

    MPI_Status status;

    MPI_Recv(&m_Period, 1, MPI_LONG, 0, m_Tag, MPI_COMM_WORLD, &status);

    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments. We log the message.

    m_pCommand = new mcSetDisplayPeriod(m_SimTime, true, m_Period);

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmSetDisplayPeriod::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSetDisplayPeriod::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


