/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcGenerateRNGUniformImpl.cpp: implementation of the mcGenerateRNGUniformImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcGenerateRNGUniformImpl.h"
#include "mcGenerateRNGUniform.h"
#include "Monitor.h"
#include "CNTCell.h"   // Needed for the RNG 
#include "ISimBox.h"
#include "LogGenerateRNGSequence.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcGenerateRNGUniformImpl::mcGenerateRNGUniformImpl()
{
}

mcGenerateRNGUniformImpl::~mcGenerateRNGUniformImpl()
{

}

// Command handler to output a sequence of uniformly-distributed, normalised
// random numbers to the log file. They are drawn from the interval (0,1)
// using the current sequence whose seed is specified in the control data file.

void mcGenerateRNGUniformImpl::GenerateRNGUniform(const xxCommand* const pCommand)
{
	const mcGenerateRNGUniform* const pCmd = dynamic_cast<const mcGenerateRNGUniform*>(pCommand);

    const long sequenceSize = pCmd->GetSequenceSize();

    CMonitor* pMon = dynamic_cast<CMonitor*>(this);

    zDoubleVector sequence;
    sequence.clear();

    for(long i=0; i<sequenceSize; i++)
    {
        double x = CCNTCell::GetRandomNo();
        sequence.push_back(x);
    }

     new CLogGenerateRNGSequence(pMon->GetCurrentTime(), "Uniform", sequence);
}
