// AdhesionData.h: interface for the CAdhesionData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADHESIONDATA_H__88FC9139_4101_11D4_BF3D_004095086186__INCLUDED_)
#define AFX_ADHESIONDATA_H__88FC9139_4101_11D4_BF3D_004095086186__INCLUDED_


// Forward declarations

class CAnalysis;


#include "AnalysisData.h"

class CAdhesionData : public CAnalysisData  
{
	friend class CAdhesion;		// Needed to copy data into CAdhesion

public:
	CAdhesionData();
	virtual ~CAdhesionData();

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual CAnalysis* CreateAnalysis(const zString runId, long samplePeriod);	// Function to copy the data into a CAnalysis-derived object
										// Member function and variable necessary to allow
	static const zString GetType();	// the aggregate type to be accessed at class level


protected:
	virtual const zString GetAnalysisType() const;


private:
	static zString m_Type;			// Keyword used to identify structure for analysis


private:							// Data specific to aggregate type

};

#endif // !defined(AFX_ADHESIONDATA_H__88FC9139_4101_11D4_BF3D_004095086186__INCLUDED_)

