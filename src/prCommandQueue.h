// prCommandQueue.h: interface for the prCommandQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRCOMMANDQUEUE_H__73D77695_02ED_41C1_A33E_5BA736E0C0C0__INCLUDED_)
#define AFX_PRCOMMANDQUEUE_H__73D77695_02ED_41C1_A33E_5BA736E0C0C0__INCLUDED_


#include "xxProcess.h"

class prCommandQueue : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prCommandQueue();
	virtual ~prCommandQueue();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvents-derived class(es)
	// rather than doing dynamic_cast every time we want to access them.
	// We cannot provide such a function in the xxProcess base class
	// because different processes may need different numbers of aggregates
	// and events.


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:	

	bool ReadCommands(zFileStream& ioStream);

	// ****************************************
	// Data members

private:						// Data specific to the process

	long	m_Multiplier;		// Check for new commands every m_Multiplier samples
	zString	m_FileName;			// File containing commands to queue
	zString	m_Separator;		// Separator string showing which commands have been read
	zString	m_Comment;			// String that causes the rest of the line to be ignored


	long	m_CommandsRead;						// Number read on each call
	CommandObjectSequence  m_CommandObjects;	// Set of command objects

};

#endif // !defined(AFX_PRCOMMANDQUEUE_H__73D77695_02ED_41C1_A33E_5BA736E0C0C0__INCLUDED_)
