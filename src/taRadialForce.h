// taRadialForce.h: interface for the taRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARADIALFORCE_H__2AAF0779_E301_49BA_BA26_3B22ECB15783__INCLUDED_)
#define AFX_TARADIALFORCE_H__2AAF0779_E301_49BA_BA26_3B22ECB15783__INCLUDED_


#include "taForceDecorator.h"

class taRadialForce : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taRadialForce(const zString label, CCommandTargetNode* const pDec,
				  long xn, long yn, long zn, 
			  	  double xc, double yc, double zc, double mag);

    taRadialForce(const zString label);


	virtual ~taRadialForce();

    // ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;


    virtual void Execute(long simTime);

	virtual double GetXForce(long simTime);
	virtual double GetYForce(long simTime);
	virtual double GetZForce(long simTime);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:


private:

	long m_X;
	long m_Y;	// Integer components of normal to plane of force
	long m_Z;

	double	m_XCentre;
	double	m_YCentre;	    // Coordinates of force origin in units of bead diameters
	double	m_ZCentre;

	double	m_Magnitude;    // Magnitude of the force

	// Local data

	double m_FX;
	double m_FY;			// Force components
	double m_FZ;

	double m_RelOX;
	double m_RelOY;			// Bead position vector relative to force origin
	double m_RelOZ;
	double m_RelDotN;		// Scalar product of rel. pos. vector and normal

	double m_ForceX;
	double m_ForceY;		// Unnormalized force components
	double m_ForceZ;
	double m_ForceMag;		// Magnitude of force
};

#endif // !defined(AFX_TARADIALFORCE_H__2AAF0779_E301_49BA_BA26_3B22ECB15783__INCLUDED_)
