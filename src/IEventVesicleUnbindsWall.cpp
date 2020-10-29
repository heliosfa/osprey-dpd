/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IEventVesicleUnbindsWall.cpp: implementation of the IEventVesicleUnbindsWall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IEventVesicleUnbindsWall.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// No constructor as this interface class has no state.

IEventVesicleUnbindsWall::~IEventVesicleUnbindsWall()
{

}

// Default implementation for aggregates that do not support this event.
// This must be overridden by a derived class, and return true, to indicate
// that the class provides its own implementation of the event's
// interface functions.

bool IEventVesicleUnbindsWall::VesicleUnbindsWallSupported() const
{
	return false;
}
 
double IEventVesicleUnbindsWall::GetCMXPos() const
{
	return 0.0;
}

double IEventVesicleUnbindsWall::GetCMYPos() const
{
	return 0.0;
}

double IEventVesicleUnbindsWall::GetCMZPos() const
{
	return 0.0;
}

double IEventVesicleUnbindsWall::GetRadius() const
{
	return 0.0;
}
