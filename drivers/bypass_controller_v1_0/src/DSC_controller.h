#pragma once 

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "CSRMap.h"
#include "DSC_controller_reg.h"


class DSCcontroller 
{
    public:
        DSCcontroller (unsigned int offset, bool isSrcMM, bool isDstMM, bool isCyclic, 
                uint64_t srcaddr, uint64_t dstaddr, uint32_t length, uint32_t repeat) 
        {
            csrMap = NULL;
		
	    m_dsc_map.m_controlreg.msBits.stop = 1;
	    m_dsc_map.m_controlreg.msBits.completed = 1;
	    AssignAddress(srcaddr, true);
	    AssignAddress(dstaddr, false);
	    m_dsc_map.m_lengthreg.msBits.length = length;
	    m_dsc_map.m_repeatreg = repeat;
            AssignStatus(isSrcMM, isDstMM, isCyclic);
		
            m_offset = offset;
	    if (m_offset != 0)
		csrMap = new tcCSRMap ("/dev/xdma0_user", 1*1024*1024, m_offset);
	    else
	    {
		exit(-11);
	    }
        
        }
        
        

	~DSCcontroller()
	{
	    if (csrMap)
		delete csrMap;
	} 

	void SyncBits()
	{
	    if (!csrMap) exit(-11);
		
            // control bit
            m_dsc_map.m_controlreg.mnWord = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_CTL_REG_OFFSET);    
            
            // address bits
            m_dsc_map.m_srcaddrhighreg = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_SRC_HIGH_OFFSET);    
            m_dsc_map.m_srcaddrlowreg = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_SRC_LOW_OFFSET);    
            m_dsc_map.m_destaddrhighreg = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_DST_HIGH_OFFSET);    
            m_dsc_map.m_destaddrlowreg = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_DST_LOW_OFFSET);    
            
            // length/repeat
            m_dsc_map.m_lengthreg.mnWord = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_LENG_REG_OFFSET);    
            m_dsc_map.m_repeatreg = csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_REPT_REG_OFFSET);    
            
	}

	unsigned int GetStatusBits()
	{
	    if (!csrMap) exit(-11);
            
            return (csrMap->ReadCtrlRegWord(BYPASS_CONTROLLER_STAT_REG_OFFSET));    
	}

	void IssueCommand()
	{
	    if (!csrMap) exit(-11);

            // control bit
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_CTL_REG_OFFSET, m_dsc_map.m_controlreg.mnWord);    
            
            // address bits
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_SRC_HIGH_OFFSET, m_dsc_map.m_srcaddrhighreg);    
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_SRC_LOW_OFFSET, m_dsc_map.m_srcaddrlowreg);    
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_DST_HIGH_OFFSET, m_dsc_map.m_destaddrhighreg);    
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_DST_LOW_OFFSET, m_dsc_map.m_destaddrlowreg);    
            
            // length/repeat
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_LENG_REG_OFFSET, m_dsc_map.m_lengthreg.mnWord);    
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_REPT_REG_OFFSET, m_dsc_map.m_repeatreg);    
            
            // control bit
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_STAT_REG_OFFSET, m_dsc_map.m_statusreg.mnWord);    
            
	}

	void HaltIssue()
	{
	    if (!csrMap) exit(-11);

	    m_dsc_map.m_statusreg.msBits.start = 0;
	    m_dsc_map.m_statusreg.msBits.stop = 1;
		
            csrMap->WriteCtrlRegWord(BYPASS_CONTROLLER_STAT_REG_OFFSET, m_dsc_map.m_statusreg.mnWord);    

	    m_dsc_map.m_statusreg.msBits.start = 1;
	    m_dsc_map.m_statusreg.msBits.stop = 0;
		
	}	
	
	void AssignAddress(uint64_t addr, bool isSrc = true)
	{
	    if (isSrc)
	    {
		m_dsc_map.m_srcaddrhighreg = ((addr >> 32) & 0xFFFFFFFF);
		m_dsc_map.m_srcaddrlowreg = (addr & 0xFFFFFFFF);
	    }
	    else
	    {		
		m_dsc_map.m_destaddrhighreg = ((addr >> 32) & 0xFFFFFFFF);
		m_dsc_map.m_destaddrlowreg = (addr & 0xFFFFFFFF);
	    }
	}

        void AssignStatus(bool isSrcMM, bool isDstMM, bool isCyclic)
        {
	    m_dsc_map.m_statusreg.msBits.start = 1;
	    m_dsc_map.m_statusreg.msBits.stop = 0;

	    m_dsc_map.m_statusreg.msBits.is_cyclic = (isCyclic ? 1 : 0);
	    m_dsc_map.m_statusreg.msBits.is_src_mm = (isSrcMM ? 1 : 0);
	    m_dsc_map.m_statusreg.msBits.is_dst_mm = (isDstMM ? 1 : 0);

        }

	DscRegMap m_dsc_map;
    private:

        tcCSRMap *csrMap;
	unsigned int m_offset;
};
