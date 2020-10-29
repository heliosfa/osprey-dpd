// acSetBondUnbindsFromPolymerTailEvent.h: interface for the acSetBondUnbindsFromPolymerTailEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETBONDUNBINDSFROMPOLYMERTAILEVENT_H__A8223BDD_01EB_47A4_A7E9_FE848DE06F70__INCLUDED_)
#define AFX_ACSETBONDUNBINDSFROMPOLYMERTAILEVENT_H__A8223BDD_01EB_47A4_A7E9_FE848DE06F70__INCLUDED_


#include "xxCommand.h" 

class acSetBondUnbindsFromPolymerTailEvent : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetBondUnbindsFromPolymerTailEvent(long executionTime);
	acSetBondUnbindsFromPolymerTailEvent(const acSetBondUnbindsFromPolymerTailEvent& oldCommand);

	virtual ~acSetBondUnbindsFromPolymerTailEvent();
	
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

	inline const zString GetACNType() const {return m_ACNType;}

	inline long   GetDuration()       const {return m_Duration;}
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}


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

	zString m_ACNType;		// Name of ACN to modify

	long    m_Duration;			// No of steps needed for complete binding
	double  m_SpringConstant;	// Final value of the transient bond spring constant
	double  m_Length;			// Final value of bond's unstretched length

};

#endif // !defined(AFX_ACSETBONDUNBINDSFROMPOLYMERTAILEVENT_H__A8223BDD_01EB_47A4_A7E9_FE848DE06F70__INCLUDED_)
