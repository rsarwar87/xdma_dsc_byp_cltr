#include <CSRMap.h>


class DSCcontroller 
{
    public:
        DSCcontroller (unsigned int offset, bool isSrcMM, bool isDstMM, bool isCyclic, 
                uint16_t ctrl, uint32_t srsaddr, 
                uint32_t dstaddr, uint32_t length, uint32_t repeat) 
        {
            csrMap = NULL;
            m_RepeatReg = 0x5;
            m_LengthReg = 0x200;
            m_DestAddrHighReg = 0x0;
            m_SrcAddrHighReg = 0x0;
            m_DestAddrLowReg = 0x0;
            m_SrcAddrLowReg = 0x0;
            m_StartReg = 0x9 ((isSrcMM ? 1 < 2 : 0) + (isDstMM ? 1 < 3 : 0) + (isCyclic ? 1 < 1 : 0) + 1); //0x5
            m_StopReg = 0x80;
            m_offset = offset;
        }
        



    private:
        uint16_t   m_ControlReg;
        uint32_t   m_SrcAddrHighReg;
        uint32_t   m_SrcAddrLowReg;
        uint32_t   m_DestAddrHighReg;
        uint32_t   m_DestAddrLowReg;
        uint32_t   m_LengthReg;
        uint32_t   m_RepeatReg;
        uint8_t    m_StartReg;
        uint8_t    m_StopReg;

        unsigned int m_offset;
        tcCSRMap *csrMap;
}
