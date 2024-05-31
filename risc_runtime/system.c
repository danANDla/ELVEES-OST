#include "system.h"

unsigned int sys_kernel_pa_to_va(unsigned int pa)
{
    if(pa < 0x40000000) return (pa + 0xa0000000);
    if(pa < 0xC0000000) return (pa - 0x40000000);
    return pa;
}
unsigned int sys_kernel_va_to_pa(unsigned int va)
{
    if(va >= 0xE0000000) return va;
    if(va >= 0xa0000000) return (va - 0xa0000000);
    if(va < 0x80000000) return (va + 0x40000000);
    return sys_kernel_va_to_pa(va | 0x20000000); //cache address
}

unsigned get_cpu_clock(void)
{
#if (defined(TARGET_MCT04)||defined(TARGET_MCT05)||defined(TARGET_MCT03P)||defined(TARGET_MCT06)||defined(TARGET_MCT07)||defined(TARGET_MCT08))
    if (CR_PLL&0x80)
        if (CR_PLL & 0x7f)
            return((XTI)*(CR_PLL&0x7f));
        else
            return XTI>>4;
    else
        return XTI;
#elif (defined(TARGET_MC0428)||defined(TARGET_NVCOM02T))
    if (CR_PLL & 0x7f)
        return((XTI)*(CR_PLL&0x7f));
    else
        return XTI>>4;
#elif (defined(TARGET_MC24R))
    if (CR_PLL & 0x1f)
        return((XTI)*(CR_PLL&0x1f));
    else
        return XTI>>4;
#elif (defined(TARGET_MC30SF6))
    if (CR_PLL0&0x80)
        if (CR_PLL0 & 0x7f)
            return((XTI)*(CR_PLL0&0x7f));
        else
            return XTI>>4;
    else
        return XTI;
#elif (defined(TARGET_MCOM02))
    if ((SEL_APLL & 0xff) >= 0x3D)
        return XTI*0x3E;
    else
        return((XTI)*((SEL_APLL&0xFF) + 1));
#elif (defined(TARGET_NVCOM02))
    if (CR_PLL & 0xff)
        return((XTI)*(CR_PLL&0xff));
    else
        return XTI;
#else
#error Unknown define for target device
#endif
}

unsigned get_dsp_clock(void)
{
#if (defined(TARGET_NVCOM02T)||defined(TARGET_MC30SF6))
    return((XTI)*((CR_PLL&0x7f0000)>>16));
#else
    return 0;
#endif
};

unsigned _get_cpu_id(void)
{
#if (defined(TARGET_MCT04)) 
    return 0;
#else
    return 0;
#endif
}
