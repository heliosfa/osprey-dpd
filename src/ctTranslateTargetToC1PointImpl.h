// ctTranslateTargetToC1PointImpl.h: interface for the ctTranslateTargetToC1PointImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTRANSLATETARGETTOC1POINTIMPL_H__5ef88a9f_4fe8_4f18_9654_b0b38ad9959d__INCLUDED_)
#define AFX_CTTRANSLATETARGETTOC1POINTIMPL_H__5ef88a9f_4fe8_4f18_9654_b0b38ad9959d__INCLUDED_


#include "ISimBoxCmd.h"

class ctTranslateTargetToC1PointImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctTranslateTargetToC1PointImpl();

	virtual ~ctTranslateTargetToC1PointImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void TranslateTargetToC1Point(const xxCommand* const pCommand);


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
private:

};

#endif // !defined(AFX_CTTRANSLATETARGETTOC1POINTIMPL_H__5ef88a9f_4fe8_4f18_9654_b0b38ad9959d__INCLUDED_)
