// acDisassembleForminFilaments.h: interface for the acDisassembleForminFilaments class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACDISASSEMBLEFORMINFILAMENTS_H__4167865B_7764_46CD_9E1C_99D60A96ED58__INCLUDED_)
#define AFX_ACDISASSEMBLEFORMINFILAMENTS_H__4167865B_7764_46CD_9E1C_99D60A96ED58__INCLUDED_


#include "xxCommand.h" 

class acDisassembleForminFilaments : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acDisassembleForminFilaments(long executionTime);
	acDisassembleForminFilaments(const acDisassembleForminFilaments& oldCommand);

	virtual ~acDisassembleForminFilaments();
	
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

	zString m_ACNType;		 // Name of ACN to modify

};

#endif // !defined(AFX_ACDISASSEMBLEFORMINFILAMENTS_H__4167865B_7764_46CD_9E1C_99D60A96ED58__INCLUDED_)
