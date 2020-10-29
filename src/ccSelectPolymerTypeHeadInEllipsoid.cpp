/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInEllipsoid.cpp: implementation of the ccSelectPolymerTypeHeadInEllipsoid class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectPolymerTypeHeadInEllipsoid.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccSelectPolymerTypeHeadInEllipsoid::m_Type = "SelectPolymerTypeHeadInEllipsoid";

const zString ccSelectPolymerTypeHeadInEllipsoid::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccSelectPolymerTypeHeadInEllipsoid::m_ArgumentTotal = 11;

long ccSelectPolymerTypeHeadInEllipsoid::GetArgumentTotal()
{
	return m_ArgumentTotal;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new ccSelectPolymerTypeHeadInEllipsoid(executionTime);}

	const zString id = ccSelectPolymerTypeHeadInEllipsoid::GetType();
    const long  argTotal = ccSelectPolymerTypeHeadInEllipsoid::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInEllipsoid::ccSelectPolymerTypeHeadInEllipsoid(long executionTime) : ccSelectPolymerTypeInRegion(executionTime),
									m_XC(0.0), m_YC(0.0), m_ZC(0.0),
									m_BoundingRadius(0.0),
									m_SMA(0.0),
									m_SMB(0.0),
									m_SMC(0.0),
									m_Theta(0.0), m_Phi(0.0)
{
}

ccSelectPolymerTypeHeadInEllipsoid::ccSelectPolymerTypeHeadInEllipsoid(const ccSelectPolymerTypeHeadInEllipsoid& oldCommand) : ccSelectPolymerTypeInRegion(oldCommand),
									 m_XC(oldCommand.m_XC),
									 m_YC(oldCommand.m_YC),
									 m_ZC(oldCommand.m_ZC),
									 m_BoundingRadius(oldCommand.m_BoundingRadius),
									 m_SMA(oldCommand.m_SMA),
									 m_SMB(oldCommand.m_SMB),
									 m_SMC(oldCommand.m_SMC),
									 m_Theta(oldCommand.m_Theta),
									 m_Phi(oldCommand.m_Phi)
{
}

// Constructor for use when creating the command internally. We have to perform
// validation on the data explicitly. If an illegal value for a data item
// is found, the command has its valid flag set false in the base class.
// It is up to the calling routine to check that the command is valid.
//
// Note that angles must be passed in radians unlike the default constructor where
// they are converted from degrees to radians in the get() function.

ccSelectPolymerTypeHeadInEllipsoid::ccSelectPolymerTypeHeadInEllipsoid(long executionTime, 
							   const zString target, const zString poly, 
							   double xc, double yc, double zc,
							   double boundingRadius, double sma, double smb, double smc,
							   double theta, double phi) : ccSelectPolymerTypeInRegion(executionTime, target, poly),
							   m_XC(xc), m_YC(yc), m_ZC(zc),
							   m_BoundingRadius(boundingRadius),
							   m_SMA(sma),
							   m_SMB(smb),
							   m_SMC(smc),
							   m_Theta(theta), m_Phi(phi)
{
	if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	   m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
	   SetCommandValid(false);
	}

	// Note that the semi-major axis must be the largest dimension followed
	// by the first semi-minor axis and then the second semi-minor axix

	if(m_BoundingRadius < 1.0 || 
		 m_SMA < 1.0   || m_SMB < 1.0   || m_SMC < 1.0 ||
		 m_SMB > m_SMA || m_SMC > m_SMB || m_SMA > m_BoundingRadius)
	{
	   SetCommandValid(false);
	}

	if(m_Theta < 0.0 || m_Phi < 0.0 || 
	   m_Theta > xxBase::m_globalPI || m_Phi > xxBase::m_globalTwoPI)
	{
	   SetCommandValid(false);
	}

}


ccSelectPolymerTypeHeadInEllipsoid::~ccSelectPolymerTypeHeadInEllipsoid()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_XC
//	m_YC		        Ellipsoid's centre as fractions of SimBox side lengths
//  m_ZC
//	m_BoundingRadius	Radius of bounding sphere that contains the ellipsoid in units of bead diameter
//  m_SMA               Semi-major axis of ellipsoid (must be largest dimension, units of bead diameter)
//  m_SMB               First semi-minor axis (next largest dimension)
//  m_SMC               Second semi-minor axis (smallest dimension)
//  m_Theta             Polar angle of ellipsoid's SMA (0 <= m_Theta <= 180)
//  m_Phi               Azimuthal angle   "            (0 <= m_Phi <= 360)

zOutStream& ccSelectPolymerTypeHeadInEllipsoid::put(zOutStream& os) const
{
	// We assume the angles are in radians and convert them to degrees before writing them

	const double theta = m_Theta*180.0/xxBase::m_globalPI;
	const double phi   =   m_Phi*180.0/xxBase::m_globalPI;

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ccSelectPolymerTypeInRegion::put(os);
	os << "<XC>" << m_XC << "</XC>" << zEndl;
	os << "<YC>" << m_YC << "</YC>" << zEndl;
	os << "<ZC>" << m_ZC << "</ZC>" << zEndl;
	os << "<BoundingRadius>"      << m_BoundingRadius << "</BoundingRadius>" << zEndl;
	os << "<SemiMajorAxis>"       << m_SMA   << "</SemiMajorAxis>" << zEndl;
	os << "<FirstSemiMinorAxis>"  << m_SMB   << "</FirstSemiMinorAxis>" << zEndl;
	os << "<SecondSemiMinorAxis>" << m_SMC   << "</SecondSemiMinorAxis>" << zEndl;
	os << "<PolarAngle>"          << theta << "</PolarAngle>" << zEndl;
	os << "<AzimuthalAngle>"      << phi   << "</AzimuthalAngle>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelectPolymerTypeInRegion::put(os);
	os << " " << m_XC     << " " << m_YC     << " " << m_ZC;
	os << " " << m_BoundingRadius;
	os << " " << m_SMA << " " << m_SMB << " " << m_SMC;
	os << " " << theta << " " << phi;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSelectPolymerTypeHeadInEllipsoid::get(zInStream& is)
{
	// Read the base class data first

	ccSelectPolymerTypeInRegion::get(is);

	// We perform some simple checks on the data here, but more complicated
	// validation is done in IsDataValid() below as it has access to CInputData.

	is >> m_XC >> m_YC >> m_ZC;

	if(!is.good() || m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
					 m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	   SetCommandValid(false);

	is >> m_BoundingRadius >> m_SMA >> m_SMB >> m_SMC;

	// Note that the semi-major axis must be the largest dimension followed
	// by the first semi-minor axis and then the second semi-minor axis

	if(!is.good() || m_BoundingRadius < 0.0 || 
		             m_SMA < 0.0   || m_SMB < 0.0   || m_SMC < 0.0 ||
					 m_SMB > m_SMA || m_SMC > m_SMB || m_SMA > m_BoundingRadius)
	{
	   SetCommandValid(false);
	}

	is >> m_Theta >> m_Phi;

	if(!is.good() || m_Theta < 0.0 || m_Phi < 0.0 || 
		             m_Theta > 180.0 || m_Phi > 360.0)
	{
	   SetCommandValid(false);
	}

	// Convert angles to radians

	m_Theta = m_Theta*xxBase::m_globalPI/180.0;
	m_Phi   =   m_Phi*xxBase::m_globalPI/180.0;

	return is;
}

// Non-static function to return the type of the command

const zString ccSelectPolymerTypeHeadInEllipsoid::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectPolymerTypeHeadInEllipsoid::GetCommand() const
{
	return new ccSelectPolymerTypeHeadInEllipsoid(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSelectPolymerTypeHeadInEllipsoid::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectPolymerTypeHeadInEllipsoid(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the sphereical region from
// which to select beads is valid. The target and and bead type are 
// validated in the base class and this is the ony check we need.

bool ccSelectPolymerTypeHeadInEllipsoid::IsDataValid(const CInputData& riData) const
{
	return ccSelectPolymerTypeInRegion::IsDataValid(riData);
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects.
//
// This command expects the following types:
//
// m_TargetLabel   - zString
// m_PolymerName   - zString
// XC, YC, ZC      - double, double, double
// BoundingRadius  - double
// SMA, SMB, SMC   - double, double, double,
// Theta, Phi      - double, double (assumed to be in degrees)

bool ccSelectPolymerTypeHeadInEllipsoid::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(ccSelectPolymerTypeInRegion::Pack(vArguments) && vArguments.size() == GetArgumentTotal())
    {
        vArguments.at(2)->GetValue(&m_XC);
        vArguments.at(3)->GetValue(&m_YC);
        vArguments.at(4)->GetValue(&m_ZC);
        vArguments.at(5)->GetValue(&m_BoundingRadius);
        vArguments.at(6)->GetValue(&m_SMA);
        vArguments.at(7)->GetValue(&m_SMB);
        vArguments.at(8)->GetValue(&m_SMC);
        vArguments.at(9)->GetValue(&m_Theta);
        vArguments.at(10)->GetValue(&m_Phi);

        // Validate the arguments: see the get() and IsDataValid() functions for details

        if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
           m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0 || 
	       m_BoundingRadius < 1.0   ||
		   m_SMA < 1.0   || m_SMB < 1.0   || m_SMC < 1.0 ||
		   m_SMB > m_SMA || m_SMC > m_SMB || m_SMA > m_BoundingRadius ||
           m_Theta < 0.0   || m_Phi < 0.0 || m_Theta > 180.0 || m_Phi > 360.0)
        {
            return false;
        }

        // Convert angles to radians

	    m_Theta = m_Theta*xxBase::m_globalPI/180.0;
	    m_Phi   =   m_Phi*xxBase::m_globalPI/180.0;

        return true;
    }
    else
    {
        return false;
    }

#else
    return false;
#endif
}
