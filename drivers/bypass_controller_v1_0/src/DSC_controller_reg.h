
#ifndef BYPASS_CONTROLLER_H
#define BYPASS_CONTROLLER_H


/****************** Include Files ********************/

#define BYPASS_CONTROLLER_CTL_REG_OFFSET         0x0        // 15 downto 
#define BYPASS_CONTROLLER_DST_HIGH_OFFSET        0x1
#define BYPASS_CONTROLLER_DST_LOW_OFFSET         0x2
#define BYPASS_CONTROLLER_SRC_HIGH_OFFSET        0x3
#define BYPASS_CONTROLLER_SRC_LOW_OFFSET         0x4
#define BYPASS_CONTROLLER_LENG_REG_OFFSET        0x5	    // max 250 MB
#define BYPASS_CONTROLLER_REPT_REG_OFFSET        0x6
#define BYPASS_CONTROLLER_STAT_REG_OFFSET        0x7       // 

typedef union
{
    struct
    {
	unsigned int stop			: 1;
	unsigned int completed    		: 1;
	unsigned int reserved0			: 2;
	unsigned int eop			: 1;
	unsigned int reserved1			: 3;
	unsigned int unknown			: 8;
	unsigned int reserved2			:16;
    } msBits;
    unsigned int mnWord;
} descCtrl;

typedef union
{
    struct
    {
	unsigned int start			: 1;
	unsigned int is_cyclic	    		: 1;
	unsigned int is_src_mm			: 1;
	unsigned int is_dst_mm			: 1;
	unsigned int reserved1			:27;
	unsigned int stop			: 1;
    } msBits;
    unsigned int mnWord;
} descStatus;


typedef union
{
    struct
    {
	unsigned int length			: 28;
	unsigned int reserve			: 4;
    } msBits;
    unsigned int mnWord;
} descLength;

struct DscRegMap
{
    descCtrl   m_controlreg;
    uint32_t   m_srcaddrhighreg;
    uint32_t   m_srcaddrlowreg;
    uint32_t   m_destaddrhighreg;
    uint32_t   m_destaddrlowreg;
    descLength m_lengthreg;
    uint32_t   m_repeatreg;
    descStatus m_statusreg;
};



#endif // BYPASS_CONTROLLER_H
