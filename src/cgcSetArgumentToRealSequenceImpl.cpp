/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentToRealSequenceImpl.cpp: implementation of the cgcSetArgumentToRealSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentToRealSequenceImpl.h"
#include "cgcSetArgumentToRealSequence.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "tguCommandGroup.h"
#include "LogcgcSetArgumentToRealSequence.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentToRealSequenceImpl::cgcSetArgumentToRealSequenceImpl()
{
}

cgcSetArgumentToRealSequenceImpl::~cgcSetArgumentToRealSequenceImpl()
{
}

// Command handler to assign a real value from a sequence to an argument of a
// specified command in a specified group.

void cgcSetArgumentToRealSequenceImpl::SetArgumentToRealSequence(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

    const cgcSetArgumentToRealSequence* const pCmd = dynamic_cast<const cgcSetArgumentToRealSequence*>(pCommand);

    const zString groupName	 = pCmd->GetCommandGroupName();	// Name of command group
    const long    cmdIndex      = pCmd->GetCommandIndex();	    // Index of command to add
    const zString argName	 = pCmd->GetArgumentName();	    // Placeholder name of argument
    const double  initialValue  = pCmd->GetInitialValue();      // Initial value of the sequence
    const double  increment     = pCmd->GetIncrement();         // Increment for the sequence

    
    CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);


	// Retrieve the group and the command name (for the log message), noting that
    // the container is zero-indexed, and modify the command's argument

    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        pGroup->SetArgumentToRealSequence(cmdIndex, argName, initialValue, increment);

               const zString cmdName = pGroup->GetCommandName(cmdIndex);
		new CLogcgcSetArgumentToRealSequence(pSimBox->GetCurrentTime(), groupName, cmdName, cmdIndex, argName, initialValue, increment);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
 
#endif
}

