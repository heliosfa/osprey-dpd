/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogacSetBondUnbindsFromPolymerHeadEvent.cpp: implementation of the CLogacSetBondUnbindsFromPolymerHeadEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogacSetBondUnbindsFromPolymerHeadEvent.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogacSetBondUnbindsFromPolymerHeadEvent& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SetBondUnbindsFromPolymerHeadEvent</Name>"  << zEndl;
	os << "<Text>" << zEndl;
	os << "<ActiveCellNetwork>" << rMsg.m_ACNType        << "</ActiveCellNetwork>" << zEndl;
	os << "<Duration>"          << rMsg.m_Duration       << "</Duration>"     << zEndl;
	os << "<SpringConstant>"    << rMsg.m_SpringConstant << "</SpringConstant>"    << zEndl;
	os << "<UnstretchedLength>" << rMsg.m_Length         << "</UnstretchedLength>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << rMsg.m_ACNType  << " active cell network implements BondUnbindsFromPolymerHead active event with parameters: ";
	os << rMsg.m_Duration << " " << rMsg.m_SpringConstant << " " << rMsg.m_Length;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogacSetBondUnbindsFromPolymerHeadEvent::CLogacSetBondUnbindsFromPolymerHeadEvent(long time, const zString type, long duration, 
													   double spring, double length) : CLogConstraintMessage(time), 
													m_ACNType(type),
													m_Duration(duration),
													m_SpringConstant(spring),
													m_Length(length)
{

}

CLogacSetBondUnbindsFromPolymerHeadEvent::~CLogacSetBondUnbindsFromPolymerHeadEvent()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogacSetBondUnbindsFromPolymerHeadEvent::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

