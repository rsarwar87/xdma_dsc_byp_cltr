#ifndef CSR_MAP_H
#define CSR_MAP_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <byteswap.h>
/* ltoh: little to host */
/* htol: little to host */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ltohl(x)       (x)
#define ltohs(x)       (x)
#define htoll(x)       (x)
#define htols(x)       (x)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define ltohl(x)     __bswap_32(x)
#define ltohs(x)     __bswap_16(x)
#define htoll(x)     __bswap_32(x)
#define htols(x)     __bswap_16(x)
#endif


class tcCSRMap
{
    public:
	tcCSRMap(const char *dev, unsigned int size, unsigned int RegsAddr)
        {
	    int fd;
            mpRegMapMM = NULL;
            sz = size;
	    /* map in the raw image buffer area */
	    printf("Opening device %s", dev);
	    if ((fd = open(dev, O_RDWR | O_SYNC)) < 0)
	    {       
                printf("Failed to creating mapping.\n");
                printf("ERRNO: %s\n", strerror(errno));
                exit (-1);
	    }
	    else
	    {
		void *lpmem;

		lpmem = mmap(0, size, PROT_READ|PROT_WRITE, MAP_FILE | MAP_SHARED, fd, RegsAddr);
		if (lpmem == MAP_FAILED)
		{
                    printf("Failed to creating mapping.\n");
                    printf("ERRNO: %s\n", strerror(errno));
		    exit(-1);
		}
		mpRegMapMM = lpmem;
	    }
        }


	virtual ~tcCSRMap(void)
        {
	    if (mpRegMapMM)
		munmap(mpRegMapMM, sz);
        }
        
        void WriteBuffer (char * buffer)
        {
            sprintf((char *)mpRegMapMM, "%s", buffer);
        }

        void ReadBuffer (char * buffer, unsigned int size)
        {
            snprintf(buffer, 13, "%s", (char *)mpRegMapMM);
        }

	unsigned int ReadCtrlRegByte(int offset, int offset_byte)
	{
		volatile uint8_t* preg = (volatile uint8_t*)(mpRegMapMM);
		return preg[offset*4 + offset_byte];
	}

	unsigned int ReadCtrlRegShort(int offset, int offset_half)
	{
		volatile uint16_t* preg = (volatile uint16_t*)(mpRegMapMM);
		return ltohs(preg[offset*2+offset_half]);
	}

	unsigned int ReadCtrlRegWord(int offset = 0)
	{
		volatile uint32_t* preg = (volatile uint32_t*)(mpRegMapMM);
		return ltohl(preg[offset]);
	}

	void WriteCtrlRegWord(int offset, unsigned int value)
	{
		volatile unsigned int* preg = (volatile unsigned int*)(mpRegMapMM);
		preg[offset] = htoll(value);
	}

        void WriteCtrlRegShort(int offset, int offset_half, uint16_t value)
	{
		volatile uint16_t* preg = (volatile uint16_t*)(mpRegMapMM);
		preg[offset*2 + offset_half] = htols(value);
	}
 
        void WriteCtrlRegByte(int offset, int offset_byte, uint8_t value)
	{
		volatile uint8_t* preg = (volatile uint8_t*)(mpRegMapMM);
		preg[offset*4+offset_byte] = value;
	}
 

	void	*mpRegMapMM;  //!< memory mapped pointer to registers
        int sz;
};

#endif

