/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevBondBindsToPolymerHead.cpp: implementation of the aevBondBindsToPolymerHead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevBondBindsToPolymerHead.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "Bond.h"
#include "aeCNTCell.h"

#include "IActiveSimBox.h"
#include "IActiveBondBindsToPolymer.h"
#include "aeActiveCellNetwork.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevBondBindsToPolymerHead::m_Type = "BondBindsToPolymerHead";

const zString aevBondBindsToPolymerHead::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	aevActiveEvent* Create() {return new aevBondBindsToPolymerHead();}

	const zString id = aevBondBindsToPolymerHead::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevBondBindsToPolymerHead::aevBondBindsToPolymerHead() : m_Duration(0), 
									 m_Range(0.0),
									 m_SpringConstant(0.0),
									 m_Length(0.0),
									 m_pIEvent(0),
									 m_pPolymer(0), 
									 m_pNewHeadBond(0),
									 m_pInternalBond(new CBond(-1,0.0,0.0)),
									 m_Counter(0)

								 
{

}

// The same aggregate object is passed to the copied event. Note that we create
// a new CBond here as the old one will be destroyed when the oldEvent instance is
// destroyed.
//

aevBondBindsToPolymerHead::aevBondBindsToPolymerHead(const aevBondBindsToPolymerHead& oldEvent) : aevActiveEvent(oldEvent),
																	m_Duration(oldEvent.m_Duration),
																	m_Range(oldEvent.m_Range),
																	m_SpringConstant(oldEvent.m_SpringConstant),
																	m_Length(oldEvent.m_Length),                                                                   
																	m_pPolymer(oldEvent.m_pPolymer),
																	m_pNewHeadBond(oldEvent.m_pNewHeadBond),
																	m_pInternalBond(new CBond(-1,0.0,0.0)),
																	m_Counter(oldEvent.m_Counter)
{
}

aevBondBindsToPolymerHead::~aevBondBindsToPolymerHead()
{
	// Destroy the contained active event interface instance if it has been created
	
	if(m_pIEvent)
	{
		delete m_pIEvent;
		m_pIEvent = 0;
	}

	// Delete the transient bond between monomers if it exists

	if(m_pInternalBond)
		delete m_pInternalBond;
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& aevBondBindsToPolymerHead::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevBondBindsToPolymerHead::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevBondBindsToPolymerHead::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevBondBindsToPolymerHead::GetEvent() const
{
	return new aevBondBindsToPolymerHead(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

// Function to store the active polymer that is being monitored by this event.
// If the pointer passed in is null there is not much we can do.

void aevBondBindsToPolymerHead::SetPolymer(aeActivePolymer *const pPolymer)
{
	if(pPolymer)
	{
		m_pPolymer = pPolymer;
	}
	else
	{
		m_pPolymer = 0;
	}
}

void aevBondBindsToPolymerHead::SetDuration(long time)
{
	if(time > 0)
	{
		m_Duration = time;
	}
}

void aevBondBindsToPolymerHead::SetRange(double range)
{
	if(range > 0.0)
	{
		m_Range = range;
	}
}

void aevBondBindsToPolymerHead::SetSpringConstant(double spring)
{
	if(spring > 0.0)
	{
		m_SpringConstant = spring;
	}
}

void aevBondBindsToPolymerHead::SetLength(double length)
{
	if(length > 0.0)
	{
		m_Length = length;
	}
}


// **********************************************************************
// Function to validate the data when an event is created on the fly. It 
// should be implemented by all event classes that may be created during a simulation.

bool aevBondBindsToPolymerHead::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions. This event is automatically created when a new polymer is formed.

bool aevBondBindsToPolymerHead::CheckPreconditions()
{
	SetDataValid(true);
	return true;
}

// **********************************************************************
// Implementation of the Execute command sent by the ActiveSimBox to aevActiveEvents.
//
// The event is active for its lifetime and indicates that it should be 
// destroyed by returning false from its Execute() function.
//
// This event wraps a newly-created active polymer instance and checks
// to see if an active bond can attach to its head end. The conditions for
// a bond to attach are that it should be within a specified distance of the
// polymer's head bond for a specified time.
//

bool aevBondBindsToPolymerHead::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer to the active SimBox.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActiveBondBindsToPolymer();
	}

	// Only search for a new bond to attach to the polymer if one is not
	// already being worked on and no contra events are active.

	if(!IsAnyContraEventActive() && !m_pNewHeadBond)
	{
		aeCNTCell* const pHeadCell = GetCellFromBond(m_pPolymer->GetHeadBond());
				
		aeActiveBond* pTargetBond = pHeadCell->UpdatePolymerisation(m_pPolymer->GetHeadBond());

		if(pTargetBond)
		{
			m_pNewHeadBond = pTargetBond;

			// If there is a free bond within the specified distance of the polymer's
			// head, we attempt to attach it to the polymer; we simultaneously
			// remove it from the ACN's free bond container and set its polymerised 
			// flag to true so that it is not made the target of a different event.
			
            // Indicate to all contra events that this event is active
            // until the new bond has been attached

            SetActive();
            std::cout <<"Adding bond to head active" << zEndl;

			m_Counter = GetDuration();
			m_pNewHeadBond->SetPolymerised();
			RemoveFreeBond(m_pNewHeadBond);

			// Create the internal bond to bring the new head bond into line

			m_pInternalBond->SetBeads(m_pPolymer->GetHeadBond()->GetTailHeadBead(), m_pNewHeadBond->GetTailHeadBead());
			m_pInternalBond->SetUnStretchedLength(GetLength());
		}
	}
	else
	{
		// Get the initial separation of the two monomers

		const double dr = m_pInternalBond->CalculatePBCLength();

		if(m_Counter > 0)
		{
			// On subsequent invocations, gradually increase the strength of the 
			// enclosed transient bond until it reaches its pre-defined value, 
			// so that the bond are gradually positioned at the user-defined 
			// separation for attaching to the polymer's head.

			m_pInternalBond->SetSpringConstant(GetSpringConstant()/static_cast<double>(m_Counter));
			m_pInternalBond->AddPBCForce();
			m_Counter--;
		}
		else if(dr > GetLength())
		{
			// Bond is not yet close enough to attach to the polymer, so release it
			// back into the free monomer pool, reset its polymerised flag,
			// and wait for a new one to attach

			m_pNewHeadBond->SetDepolymerised();
			AddFreeBond(m_pNewHeadBond);
			m_pNewHeadBond = 0;
            SetInactive();
            std::cout <<"Adding bond to head failed:  now inactive" << zEndl;

		}
		else
		{
			// Now that the bond is in the right configuration make it the
			// head of the polymer. We call the SetPolymer() function simply
			// to reset the head pointer to the new bond.

			m_pPolymer->AddHeadBond(m_pNewHeadBond);
			SetPolymer(m_pPolymer);
			m_pNewHeadBond = 0;
            SetInactive();
            std::cout <<"Adding bond to head succeeded:  now inactive" << zEndl;

			SetSpringConstant(dynamic_cast<aevBondBindsToPolymerHead*>(GetACN()->GetInternalBondBindsToPolymerHeadEvent())->GetSpringConstant());
		}
	}

	// This event always returns true as it continually monitors
	// the polymer for growth. It is destroyed by the event
	// that monitors the polymer's dissolution.

	return true;
}


