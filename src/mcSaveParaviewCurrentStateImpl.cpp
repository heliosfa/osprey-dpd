/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveParaviewCurrentStateImpl.cpp: implementation of the mcSaveParaviewCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveParaviewCurrentStateImpl.h"
#include "mcSaveParaviewCurrentState.h"
#include "Monitor.h"
#include "ParaviewFormat.h"
#include "LogSaveCurrentState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveParaviewCurrentStateImpl::mcSaveParaviewCurrentStateImpl()
{
}

mcSaveParaviewCurrentStateImpl::~mcSaveParaviewCurrentStateImpl()
{

}

// Function to write a current state to file in the Amira format. The scene
// data required to set up the output must be set before executing this command
// or the default values will be used.
//
// Note that current states that are written as the result of commands
// are not saved or included in the current state analysis.
//

void mcSaveParaviewCurrentStateImpl::SaveParaviewCurrentState(const xxCommand* const pCommand)
{
	const mcSaveParaviewCurrentState* const pCmd = dynamic_cast<const mcSaveParaviewCurrentState*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

#if EnableParallelCommands == SimMPSEnabled

	CParaviewFormat* pFormat = new CParaviewFormat(pMon->m_SimBoxXLength, pMon->m_SimBoxYLength, pMon->m_SimBoxZLength,
											       pMon->m_bDisplayBox, pMon->m_BeadTypeSize);

    // Only P0 logs the command execution, other processors simply execute the command.
	
    if(xxParallelBase::GlobalGetRank() == 0)
	{
        if(pMon->InternalSaveCurrentState(pFormat))
	    {
		    if(pCmd->IsExecutionLogged())
			{
		        CLogSaveCurrentState* pMsg = new CLogSaveCurrentState(pMon->GetCurrentTime());
			}
	    }
	    else
	    {
		    CLogCommandFailed* pMsg = new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	    }
	}
	else
	{
	    pMon->InternalSaveCurrentState(pFormat);
	}

#else

	CParaviewFormat* pFormat = new CParaviewFormat(pMon->m_SimBoxXLength, pMon->m_SimBoxYLength, pMon->m_SimBoxZLength,
											 pMon->m_bDisplayBox, pMon->m_BeadTypeSize);

    if(pMon->InternalSaveCurrentState(pFormat))
	{
		CLogSaveCurrentState* pMsg = new CLogSaveCurrentState(pMon->GetCurrentTime());
	}
	else
	{
		CLogCommandFailed* pMsg = new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
	
#endif
}
