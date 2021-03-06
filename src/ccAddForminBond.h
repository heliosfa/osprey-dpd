// ccAddForminBond.h: interface for the ccAddForminBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDFORMINBOND_H__8955EFA2_5A81_4DB5_888C_6859353E1198__INCLUDED_)
#define AFX_CCADDFORMINBOND_H__8955EFA2_5A81_4DB5_888C_6859353E1198__INCLUDED_


#include "ccAddActiveCellNetwork.h"

class ccAddForminBond : public ccAddActiveCellNetwork  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccAddForminBond(long executionTime);
	ccAddForminBond(const ccAddForminBond& oldCommand);

	virtual ~ccAddForminBond();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	
private:

	static const zString m_Type;	// Identifier used in control data file for command


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline const zString GetHeadMonomerName()	const {return m_HeadMonomerName;}
	inline const zString GetTailMonomerName()	const {return m_TailMonomerName;}
	inline  double		  GetSpringConstant()	const {return m_SpringConstant;}
	inline  double	   GetUnstretchedLength()	const {return m_UnstretchedLength;}
	inline  double		  GetBendingModulus()	const {return m_BendingModulus;}
	inline  double					GetPhi0()	const {return m_Phi0;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString m_HeadMonomerName;	// Name of polymer type bound to this bond type
	zString m_TailMonomerName;	// Name of polymer type wrapped by this bond type

	double m_SpringConstant;	// Parameters defining the bond's force law
	double m_UnstretchedLength;
	double m_BendingModulus;
	double m_Phi0;

};

#endif // !defined(AFX_CCADDFORMINBOND_H__8955EFA2_5A81_4DB5_888C_6859353E1198__INCLUDED_)
