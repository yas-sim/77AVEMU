#ifndef AY38910_IS_INCLUDED
#define AY38910_IS_INCLUDED
/* { */



#include <stdint.h>
#include <vector>

class AY38910
{
public:
	enum
	{
		NUM_CHANNELS=3,
		NUM_REGS=16,
		REG_MASK=NUM_REGS-1,
		FREQ_CONST=1228800,  // FM-Techknow pp.230, 200000 for PC88?
		MAX_AMPLITUDE=2048,

		WAVE_SAMPLING_RATE=44100,  // Must be equal from ym2612.h

		REG_CH0_FREQ_LOW=0,
		REG_CH0_FEEQ_HIGH=1,
		REG_CH1_FREQ_LOW=2,
		REG_CH1_FEEQ_HIGH=3,
		REG_CH2_FREQ_LOW=4,
		REG_CH2_FEEQ_HIGH=5,
		REG_NOISE_FREQ=6,
		REG_MIXER=7,
		REG_CH0_VOL=8,
		REG_CH1_VOL=9,
		REG_CH2_VOL=10,
		REG_ENV_FREQ_LOW=11,
		REG_ENV_FREQ_HIGH=12,
		REG_ENV_PATTERN=13,
		REG_PORTA=14,
		REG_PORTB=15,
	};

	class Channel
	{
	public:
		unsigned int toneSign=0;
		unsigned int tonePeriodBalance=0;
	};
	class State
	{
	public:
		uint8_t regs[NUM_REGS];
		Channel ch[NUM_CHANNELS];
	};
	State state;

	void Reset(void);
	uint8_t Read(uint8_t reg) const;
	void Write(uint8_t reg,uint8_t value);

	inline bool IsPlaying(void) const
	{
		return (state.regs[REG_MIXER]&0x3F)!=0x3F;
	}

	inline unsigned int GetF_NUM(int ch) const;
	inline unsigned int ChannelFrequencyX1000(int ch) const;
	inline unsigned int GetAmplitude(int ch) const;
	inline unsigned int EnvelopeFreqX1000(void) const;

	std::vector <unsigned char> MakeWaveAllChannels(unsigned long long int millisec);
	void AddWaveAllChannelsForNumSamples(unsigned char data[],unsigned long long int numSamples);
};

/* } */
#endif