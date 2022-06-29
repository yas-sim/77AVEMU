#ifndef FM77AVCRTC_IS_INCLUDED
#define FM77AVCRTC_IS_INCLUDED
/* { */



#include "device.h"
#include "fm77avdef.h"

class FM77AVCRTC : public Device
{
public:
	virtual const char *DeviceName(void) const{return "CRTC";}

	class Palette
	{
	public:
		uint8_t digitalPalette[8];
		uint16_t analogPalette[4096];
		void Reset(void);
	};

	enum
	{
		// VSYNC_CYCLE is 1670000, but it is close enough to 0x1000000(16777216)
		VSYNC_CYCLE=         0x1000000,
		// Measurement taken from actual FM TOWNS II MX hardware tells VSYNC lasts for 60us.
		CRT_VERTICAL_DURATION=0x1000000-60000, // Time CRTC spends for drawing.  VSYNC_CYCLE-CRT_VERTICAL_DURATION gives duration of VSYNC.
		// HSYNC_CYCLE should be 32000, but it is close enough to 0x8000(32768)
		HSYNC_CYCLE=            0x8000, // Not accurate.  Fixed at 31K
		CRT_HORIZONTAL_DURATION= 30000,
	};

	class State
	{
	public:
		Palette palette;
		unsigned int scrnMode=SCRNMODE_640X200_SINGLE;
		uint16_t VRAMOffset=0;
	};
	FM77AVCRTC(VMBase *vmBase);
	void Reset(void);
	State state;
	Palette &GetPalette(void);
	const Palette &GetPalette(void) const;
	bool InVSYNC(uint64_t fm77avTime) const;
	bool InHSYNC(uint64_t fm77avTime) const;

	static inline uint32_t TransformVRAMAddress(uint32_t addr,unsigned int scrnMode,uint16_t VRAMOffset)
	{
		uint32_t addrHI,addrLO;
		switch(scrnMode)
		{
		case SCRNMODE_640X200_SINGLE:
			addrHI=addr&~0x3FFF;
			addrLO=(addr+VRAMOffset)&0x3FFF;
			return addrHI|addrLO;
		}
		return addr;
	}
	inline uint32_t TransformVRAMAddress(uint32_t addr) const
	{
		return TransformVRAMAddress(addr,state.scrnMode,state.VRAMOffset);
	}

	/*
	VSYNC Cycle        |<----------------->|
	Vertical Duration  |<------------->|
	VSYNC              000000000000000001111
                                           |
                                    NextVSYNCEndTime
	*/
	uint64_t NextVSYNCRisingEdge(uint64_t fm77avTime) const;
	uint64_t NextVSYNCEndTime(uint64_t fm77avTime) const;
};


/* } */
#endif