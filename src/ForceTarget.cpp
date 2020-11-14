/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ForceTarget.cpp: implementation of the CForceTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMiscellaneousFlags.h"
#include "ForceTarget.h"
#include "ForceLaw.h"
#include "Bead.h"
#include "aaRegionToType.h"		// Needed for the predicate aaBeadIdLess()

#if EnableMiscClasses == SimMiscEnabled
    #include "RadialForce.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForceTarget::CForceTarget(const zString label, BeadVector beads) : m_Label(label), 
																	m_Beads(beads),														
																	m_End(0),
																	m_ForceLaw(0)
{
}

CForceTarget::~CForceTarget()
{
	if(m_ForceLaw)
	{
		delete m_ForceLaw;
		m_ForceLaw = NULL;
	}
}

// Store a CForceLaw object in the current target, making sure we destroy any
// previous object. We copy the end time into the target object so that it 
// can check when it should stop. This is done for speed of access instead of 
// having to use a function call to the CForceLaw object every time step.

void CForceTarget::SetForceLaw(CForceLaw *const pForce)
{
	m_End = pForce->GetEnd();

	if(m_ForceLaw)
		delete m_ForceLaw;

	m_ForceLaw = pForce;
}

// Function to add the force generated by the enclosed CForceLaw object to each 
// bead in the target. It returns a flag showing whether the target is still active
// or not so that it can be removed from the active list when its duration expires.
// If the force law depends on the bead coordinates, as the CRadialForce does,
// we call a different function to calculate the force on the target beads.

bool CForceTarget::AddForce(long simTime)
{
#if EnableMiscClasses == SimMiscEnabled
	if(dynamic_cast<CRadialForce*>(m_ForceLaw))
	{
		double fx = 0.0;
		double fy = 0.0;
		double fz = 0.0;

		for(BeadVectorIterator iterBead=m_Beads.begin(); iterBead!=m_Beads.end(); iterBead++)
		{
			m_ForceLaw->CalculateVariableForces(*iterBead, &fx, &fy, &fz);

			(*iterBead)->m_Force[0] += fx;
			(*iterBead)->m_Force[1] += fy;
			(*iterBead)->m_Force[2] += fz;
		}
	}
	else
	{
		for(BeadVectorIterator iterBead=m_Beads.begin(); iterBead!=m_Beads.end(); iterBead++)
		{
			(*iterBead)->m_Force[0] += m_ForceLaw->GetXForce(simTime);
			(*iterBead)->m_Force[1] += m_ForceLaw->GetYForce(simTime);
			(*iterBead)->m_Force[2] += m_ForceLaw->GetZForce(simTime);
		}
	}
#endif

	if(simTime < m_End)
		return true;
	else
		return false;
}

// Function to add a set of beads to an existing target. This is used by the
// AddxxxInSlice, AddxxxInRow and AddxxxInCell commands to group beads from 
// arbitrarily-selected regions into one force target. We concatenate the
// vectors that hold the bead pointers.

void CForceTarget::AddBeads(const BeadVector& beads)
{
	m_Beads.insert(m_Beads.end(), beads.begin(), beads.end());
}

// Function to remove any duplicate bead pointers from the force target. Duplicates
// can arise if two slices with different normals but overlapping volumes are
// selected and added to the same target. It can also occur with rows or cells.
// The sequence of beads is sorted before calling the unique() algorithm on the
// container to ensure that duplicates are adjacent. All but the first of a
// sequence of duplicates is removed by unique() but the size of the container
// is left unchanged. We call erase() to eliminate the duplicate elements.
//
// Note that it is not strictly necessary to use the bead id to define the ordering
// as the bead pointers also define a unique ordering, but it looks cleaner
// when viewing the sorted beads if the bead ids are used.

void CForceTarget::RemoveDuplicateBeads()
{
	// Sort the beads according to their (unique) id

	std::sort(m_Beads.begin(), m_Beads.end(), aaBeadIdLess() );

	// Now remove adjacent duplicates

	BeadVectorIterator new_end;

	new_end = std::unique(m_Beads.begin(), m_Beads.end());

	m_Beads.erase(new_end, m_Beads.end());

}

