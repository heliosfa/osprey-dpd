// pmSendCubicBeads.h: interface for the pmSendCubicBeads class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDCUBICBEADS_H__D2948807_2D41_4E80_B8B5_54545F20AC51__INCLUDED_)
#define AFX_PMSENDCUBICBEADS_H__D2948807_2D41_4E80_B8B5_54545F20AC51__INCLUDED_


#include "mpmMessage.h"

class pmSendCubicBeads : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendCubicBeads();
	
	pmSendCubicBeads(const pmSendCubicBeads& oldMessage);

	virtual ~pmSendCubicBeads();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendCubicBeads& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendCubicBeads& message);

	static const zString GetType();	// Return the type of message
	
	static void InitialiseEmptyBeadContainer();     // Function to create the initial set of empty bead instances


private:

	static const zString m_Type;	// Identifier for the message

    static const long m_MaxMsgBuffer = 200000;      // Max number of chars in message buffer
    static const long m_MaxMsgBeads  = 1000;        // Max number of beads whose coordinates are sent
    static const long m_InitialEmptyBeads = 1000;   // Initial number of empty bead instances
	
	static BeadList m_lEmptyBeads;                  // Set of empty bead instances to receive incoming transfers
	

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSendCubicBeads pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

	virtual void ISend(long procId, MsgRequestPtr pRequest);

	virtual void IReceive(long procId, MsgRequestPtr pRequest);

	virtual bool Wait(MsgRequestPtr pRequest);

	// ****************************************
	// Public access functions
public:

    // Function to store the bead coordinates in the message

	void SetMessageData(BeadList lBeads);


    // Accessor functions to the message data

	bool CreateEmptyBeads();
	BeadList GetRecBeads();
	const zLongVector& GetBeadLocations()       const;
	const zLongVector& GetFirstBondIds()        const;
	const zLongVector& GetFirstBondPairIds()    const;
	const zLongVector& GetPolymerIds()          const;
	const zLongVector& GetPolymerTypes()        const;

    inline long GetSentBeadTotal()  const {return m_lBeads.size();}
    inline long GetRecBeadTotal()   const {return m_RecTotal;}
    inline long GetEmptyBeadTotal() const {return m_lEmptyBeads.size();}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetMessageType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    // MPI-related data

    MsgRequestPtr   m_pSendRequest;  // Request token for the send operation
    MsgRequestPtr   m_pRecRequest;   // Request token for the receive operation

    char m_SendBuffer[m_MaxMsgBuffer]; // Buffer used in non-blocking send operation
    char m_RecBuffer[m_MaxMsgBuffer];  // Buffer used in non-blocking receive operation

    // ********************
    // Data used when sending the message

    BeadList m_lBeads;               // Set of beads being sent to neighbouring processor
    BeadList m_lRecBeads;            // Set of beads received from neighbouring processor
	zLongVector m_vBeadLocations;    // Location of beads in their owning polymers
	zLongVector m_vFirstBondIds;     // Numeric id of the first bond in the polymer
	zLongVector m_vFirstBPIds;       // Numeric id of the first stiff bond in the polymer
	zLongVector m_vPolymerIds;       // Set of polymer ids owning the passed-in beads
	zLongVector m_vPolymerTypes;     // Set of polymer types
    
    // Arrays used to hold bead data before inserting into message buffer

    long     m_Ids[m_MaxMsgBeads];
	long     m_DisplayIds[m_MaxMsgBeads];
	long     m_Visibility[m_MaxMsgBeads];
    long     m_Types[m_MaxMsgBeads];
	long     m_Locations[m_MaxMsgBeads];
	long     m_FirstBondIds[m_MaxMsgBeads];
	long     m_FirstBPIds[m_MaxMsgBeads];
    long     m_PolyIds[m_MaxMsgBeads];
    long     m_PolyTypes[m_MaxMsgBeads];
    double   m_Radius[m_MaxMsgBeads];       // Hardwired to 0.5 for now

    double   m_X[m_MaxMsgBeads];
    double   m_Y[m_MaxMsgBeads];
    double   m_Z[m_MaxMsgBeads];
    double   m_VX[m_MaxMsgBeads];
    double   m_VY[m_MaxMsgBeads];
    double   m_VZ[m_MaxMsgBeads];

    double   m_OldVX[m_MaxMsgBeads];
    double   m_OldVY[m_MaxMsgBeads];
    double   m_OldVZ[m_MaxMsgBeads];
    double   m_OldFX[m_MaxMsgBeads];
    double   m_OldFY[m_MaxMsgBeads];
    double   m_OldFZ[m_MaxMsgBeads];

//    double   m_UnPBCX[m_MaxMsgBeads];
//    double   m_UnPBCY[m_MaxMsgBeads];
//    double   m_UnPBCZ[m_MaxMsgBeads];

//    double   m_InitialX[m_MaxMsgBeads];
//    double   m_InitialY[m_MaxMsgBeads];
//    double   m_InitialZ[m_MaxMsgBeads];

//    double   m_dX[m_MaxMsgBeads];
//    double   m_dY[m_MaxMsgBeads];
//    double   m_dZ[m_MaxMsgBeads];


    // ********************
    // Data used when receiving the message

    long m_RecTotal;  // Number of beads whose coordinates are received

	CAbstractBead* m_pBead;  // Bead pointer for use in filling empty bead instances

};

#endif // !defined(AFX_PMSENDCUBICBEADS_H__D2948807_2D41_4E80_B8B5_54545F20AC51__INCLUDED_)
