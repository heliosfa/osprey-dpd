/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmISRestart.cpp: implementation of the pmISRestart class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmISRestart.h"
#include "InitialStateRestart.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmISRestart::m_Type = "isrestart";

const zString pmISRestart::GetType()
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
	mpmMessage* Create() {return new pmISRestart();}

	const zString id = pmISRestart::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the parameters needed to define the specified initial state here.
// A random state actually needs no parameters, but we pass a constant string
// just to allow the message to have a payload. Note that we have to convert the
// zString into a c-style string for MPI.

pmISRestart::pmISRestart() : mpmMessage(), m_RunId(""), m_StateId(0)
{
}

pmISRestart::pmISRestart(const pmISRestart& oldMessage) : mpmMessage(oldMessage),
                         m_RunId(oldMessage.m_RunId),
                         m_StateId(oldMessage.m_StateId)
{
}

pmISRestart::~pmISRestart()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmISRestart::put(zOutStream& os) const
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

zInStream& pmISRestart::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmISRestart::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmISRestart::GetMessage() const
{
	return new pmISRestart(*this);
}

// ****************************************
// Function to copy all the required data from the CInitialStateData-derived object
// into the message. Note that each concrete message class knows the type of
// ISD that it expects, so it can perform the dynamic_cast safely.

void pmISRestart::SetMessageData(const CInitialStateData* const pISD)
{
	const CInitialStateRestart* const pData = dynamic_cast<const CInitialStateRestart*>(pISD);

    // We need the string runId and the timestep of the original restart state

    m_RunId   = pData->GetRunId();
    m_StateId = pData->GetStateId();
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// For a restart state we need to check on P0 that the specified restart state file
// and old CDF exist and are readable.

bool pmISRestart::Validate()
{
    bool bSuccess = true;

    if(m_RunId.empty())
    {
        bSuccess = false;
    }
    else if(m_StateId < 1)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmISRestart::SendAllP()
{
#if SimMPS == SimulationEnabled

    // Declare a local variable that is an MPI_Datatype to hold the 
    // message type;  note that if MPI is not compiled in, this data type defaults 
    // to a long.

    char stringRunId[1000];
    strcpy(stringRunId, m_RunId.c_str());


//    MPI_Datatype  MsgDataType;

    // Pack the data into a derived MPI data type 

//    BuildDerivedType(&MsgDataType);

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

// Because this class needs to send just a string and a long, we separate them into 
// two messages using simple MPI types.

    int tag1 = 0;
    int tag2 = 1;
    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(stringRunId, strlen(stringRunId)+1, MPI_CHAR, m_ProcId, tag1, MPI_COMM_WORLD);
        MPI_Send(&m_StateId, 1, MPI_LONG, m_ProcId, tag2, MPI_COMM_WORLD);
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

void pmISRestart::Receive()
{
#if SimMPS == SimulationEnabled

    char stringRunId[1000];

//    MPI_Datatype MsgDataType;

    // Pack the data into a derived MPI type

//    BuildDerivedType(&MsgDataType);

    int tag1 = 0;
    int tag2 = 1;
    MPI_Status  status;
    m_SenderRank = 0;

    MPI_Recv(stringRunId, 1000, MPI_CHAR, m_SenderRank, tag1, MPI_COMM_WORLD, &status);
    MPI_Recv(&m_StateId, 1, MPI_LONG, m_SenderRank, tag2, MPI_COMM_WORLD, &status);

    // Convert the c-style string runId into a zString
    m_RunId = stringRunId;
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmISRestart::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmISRestart::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
    // Not needed for this class
}
