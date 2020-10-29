/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogSaveBeadKineticTemperatures.cpp: implementation of the CLogSaveBeadKineticTemperatures class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogSaveBeadKineticTemperatures.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogSaveBeadKineticTemperatures& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SaveBeadKineticTemperatures</Name>"  << zEndl;
	os << "<Text>" << zEndl;
    os << "<BeadTypeTotal>"        << rMsg.m_BeadTypeTotal         << "</BeadTypeTotal>"   << zEndl;
    os << "<StartTime>"            << rMsg.m_Start                 << "</StartTime>"       << "<EndTime>" << rMsg.m_End << "</EndTime>" << zEndl;
    os << "<SamplePeriod>"         << rMsg.m_SamplePeriod          << "</SamplePeriod>"    << zEndl;
    os << "<TotalAnalysisPeriods>" << rMsg.m_TotalAnalysisPeriods  << "</TotalAnalysisPeriods>" << zEndl;
    os << "<TotalDataPoints>"      << rMsg.m_TotalDataPoints       << "</TotalDataPoints>" << zEndl;
    os << "<SamplesPerPoint>"      << rMsg.m_SamplesPerDataPoint   << "</SamplesPerPoint>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << " Saving kinetic temperatures for all " << rMsg.m_BeadTypeTotal << " bead types:" << zEndl;
    os << " sampling during: " << rMsg.m_Start << " " << rMsg.m_End << " with sample period " << rMsg.m_SamplePeriod << zEndl;
    os << " (equivalent to " << rMsg.m_TotalAnalysisPeriods << " analysis periods)";
    os << " for " << rMsg.m_TotalDataPoints << " data points" << zEndl;
    os << " (equivalent to " << rMsg.m_SamplesPerDataPoint << " samples per point)";


#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSaveBeadKineticTemperatures::CLogSaveBeadKineticTemperatures(long time, long analysisPeriods, 
                                                            long dataPoints, long start, long end, 
                                                            long samplePeriod, long samplesPerPoint,
                                                            long beadTypes) : CLogInfoMessage(time), 
                                                            m_TotalAnalysisPeriods(analysisPeriods),
                                                            m_TotalDataPoints(dataPoints),
                                                            m_Start(start), m_End(end), 
                                                            m_SamplePeriod(samplePeriod),
                                                            m_SamplesPerDataPoint(samplesPerPoint), m_BeadTypeTotal(beadTypes)
{

}

CLogSaveBeadKineticTemperatures::~CLogSaveBeadKineticTemperatures()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogSaveBeadKineticTemperatures::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

