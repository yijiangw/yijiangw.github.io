/**
 *   @file  dopplerprochwa.h
 *
 *   @brief
 *      Implements Data path doppler processing functionality.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DOPPLERPROC_HWA_H
#define DOPPLERPROC_HWA_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* mmWave SDK Driver/Common Include Files */
#include <ti/drivers/hwa/hwa.h>

/* DPIF Components Include Files */
#include <ti/datapath/dpif/dpif_detmatrix.h>
#include <ti/datapath/dpif/dpif_radarcube.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpc/dpu/dopplerproc/dopplerproccommon.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DPU_DOPPLERPROC_ERROR_CODE
 *  Base error code for the dopplerProc DPU is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define DPU_DOPPLERPROCHWA_EINVAL                  (DP_ERRNO_DOPPLER_PROC_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define DPU_DOPPLERPROCHWA_ENOMEM                  (DP_ERRNO_DOPPLER_PROC_BASE-2)

/**
 * @brief   Error Code: DPU is in progress
 */
#define DPU_DOPPLERPROCHWA_EINPROGRESS             (DP_ERRNO_DOPPLER_PROC_BASE-3)

/**
 * @brief   Error Code: Out of HWA resources
 */
#define DPU_DOPPLERPROCHWA_EHWARES                 (DP_ERRNO_DOPPLER_PROC_BASE-4)

/**
 * @brief   Error Code: Semaphore creation failed
 */
#define DPU_DOPPLERPROCHWA_ESEMA                   (DP_ERRNO_DOPPLER_PROC_BASE-5)

/**
 * @brief   Error Code: Bad semaphore status 
 */
#define DPU_DOPPLERPROCHWA_ESEMASTATUS             (DP_ERRNO_DOPPLER_PROC_BASE-6)

/**
 * @brief   Error Code: Configure parameters exceed HWA memory bank size 
 */
#define DPU_DOPPLERPROCHWA_EEXCEEDHWAMEM           (DP_ERRNO_DOPPLER_PROC_BASE-7)

/**
 * @brief   Error Code: Unsupported radar cube format 
 */
#define DPU_DOPPLERPROCHWA_ECUBEFORMAT             (DP_ERRNO_DOPPLER_PROC_BASE-8)

/**
 * @brief   Error Code: Unsupported detection matrix format 
 */
#define DPU_DOPPLERPROCHWA_EDETMFORMAT             (DP_ERRNO_DOPPLER_PROC_BASE-9)

/**
 * @brief   Error Code: Insufficient detection matrix size
 */
#define DPU_DOPPLERPROCHWA_EDETMSIZE               (DP_ERRNO_DOPPLER_PROC_BASE-10)

/**
 * @brief   Error Code: Wrong window size
 */
#define DPU_DOPPLERPROCHWA_EWINDSIZE               (DP_ERRNO_DOPPLER_PROC_BASE-11)

/**
@}
*/

/**
 * @brief   Maximum number of HWA paramsets used by DPU.
 *          Computed as follows:\n
 * (1 for each TX antenna + 1 for sum) * 2 (ping/pong) = 8
 */
#define DPU_DOPPLERPROCHWA_MAX_NUM_HWA_PARAMSET  8 

/**
 * @brief   Number of HWA memory banks needed
 */
#define DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS  4 

/**
 * @brief   Disables first butterfly stage scaling
 */
#define DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED ((uint8_t)0U)

/**
 * @brief   Enables first butterfly stage scaling
 */
#define DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED ((uint8_t)1U)

         
/*!
 *  @brief   Handle for Doppler Processing DPU.
 */
typedef void*  DPU_DopplerProcHWA_Handle;

/**
 * @brief
 *  dopplerProc DPU initial configuration parameters
 *
 * @details
 *  The structure is used to hold the DPU initial configurations.
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_InitCfg_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;
    
}DPU_DopplerProcHWA_InitParams;

/**
 * @brief
 *  dopplerProc DPU HWA configuration parameters
 *
 * @details
 *  The structure is used to hold the HWA configuration parameters
 *  for the Doppler Processing DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_HwaCfg_t
{
    /*! @brief Indicates if HWA window is symmetric or non-symmetric.
        Use HWA macro definitions for symmetric/non-symmetric.
    */
    uint8_t     winSym;

    /*!  @brief Doppler FFT window size in bytes. 
         This is the number of coefficients to be programmed in the HWA for the windowing
         functionality. The size is a function of numDopplerChirps as follows:\n
         If non-symmetric window is selected: windowSize = numDopplerChirps * sizeof(int32_t) \n
         If symmetric window is selected and numDopplerChirps is even:
         windowSize = numDopplerChirps * sizeof(int32_t) / 2 \n
         If symmetric window is selected and numDopplerChirps is odd:
         windowSize = (numDopplerChirps + 1) * sizeof(int32_t) / 2        
    */
    uint32_t    windowSize;

    /*! @brief Pointer to Doppler FFT window coefficients. */
    int32_t     *window;

    /*! @brief HWA window RAM offset in number of samples. */
    uint32_t    winRamOffset;
    
    /*! @brief Indicates if HWA should enable butterfly scaling (divide by 2) of the 
         first radix-2 stage. Depending on the window definition, 
         user may want to skip the first stage scaling in order to avoid signal degradation.\n
         Options are:\n
         Disable first stage scaling: firstStageScaling = @ref DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED \n
         Enable first stage scaling: firstStageScaling = @ref DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED \n
         Note: All other butterfly stages have the scaling enabled. 
         This option applies only for the first stage.\n
    */
    uint8_t     firstStageScaling;

    /*! @brief Number of HWA paramsets reserved for the Doppler DPU. 
         The number of HWA paramsets required by this DPU is a function of the number of TX antennas 
         used in the configuration:\n 
         numParamSets = 2 x (Number of TX antennas) + 2\n
         The DPU will use numParamSets consecutively, starting from paramSetStartIdx.\n
    */     
    uint8_t     numParamSets;
    
    /*! @brief HWA paramset Start index.  
         Application has to ensure that paramSetStartIdx is such that \n
        [paramSetStartIdx, paramSetStartIdx + 1, ... (paramSetStartIdx + numParamSets - 1)] \n
        is a valid set of HWA paramsets.\n
    */
    uint32_t    paramSetStartIdx;
    
}DPU_DopplerProcHWA_HwaCfg;

/**
 * @brief
 *  dopplerProc DPU EDMA configuration parameters
 *
 * @details
 *  The structure is used to hold the EDMA configuration parameters
 *  for the Doppler Processing DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_EdmaCfg_t
{
    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;
    
    /*! @brief  EDMA configuration for Input data (Radar cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaIn;
    
    /*! @brief  EDMA configuration for Output data (HWA memory -> detection matrix). */
    DPU_DopplerProc_Edma edmaOut;
    
    /*! @brief  EDMA configuration for hot signature. */
    DPU_DopplerProc_Edma edmaHotSig;
}DPU_DopplerProcHWA_EdmaCfg;

/**
 * @brief
 *  Doppler DPU HW configuration parameters
 *
 * @details
 *  The structure is used to hold the  HW configuration parameters
 *  for the Doppler DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_HW_Resources_t
{
    /*! @brief  EDMA configuration */
    DPU_DopplerProcHWA_EdmaCfg edmaCfg;
    
    /*! @brief  HWA configuration */
    DPU_DopplerProcHWA_HwaCfg  hwaCfg;
    
    /*! @brief  Radar Cube */
    DPIF_RadarCube radarCube;
    
    /*! @brief  Detection matrix */
    DPIF_DetMatrix detMatrix;
}DPU_DopplerProcHWA_HW_Resources;


/**
 * @brief
 *  Doppler DPU static configuration parameters
 *
 * @details
 *  The structure is used to hold the static configuration parameters
 *  for the Doppler DPU. The following conditions must be satisfied:
 *
 *    @verbatim
      numTxAntennas * numRxAntennas * numDopplerChirps * sizeof(cmplx16ImRe_t) <= 16 KB (one HWA memory bank)
      numTxAntennas * numRxAntennas * numDopplerBins * sizeof(uint16_t) <= 16 KB (one HWA memory bank)
      @endverbatim
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_StaticConfig_t
{
    /*! @brief  Number of transmit antennas */
    uint8_t     numTxAntennas;
    
    /*! @brief  Number of receive antennas */
    uint8_t     numRxAntennas;
    
    /*! @brief  Number of virtual antennas */
    uint8_t     numVirtualAntennas; 
    
    /*! @brief  Number of range bins */
    uint16_t    numRangeBins;
    
    /*! @brief  Number of Doppler chirps. */
    uint16_t    numDopplerChirps;
    
    /*! @brief  Number of Doppler bins */
    uint16_t    numDopplerBins;
    
    /*! @brief  Log2 of number of Doppler bins */
    uint8_t     log2NumDopplerBins;
    
}DPU_DopplerProcHWA_StaticConfig;

/**
 * @brief
 *  dopplerProc DPU configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for the Doppler Processing removal DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_Config_t
{
    /*! @brief HW resources. */
    DPU_DopplerProcHWA_HW_Resources  hwRes;
    
    /*! @brief Static configuration. */
    DPU_DopplerProcHWA_StaticConfig  staticCfg;
    
}DPU_DopplerProcHWA_Config;


/**
 * @brief
 *  DPU processing output parameters
 *
 * @details
 *  The structure is used to hold the output parameters DPU processing
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_OutParams_t
{
    /*! @brief DPU statistics */
    DPU_DopplerProc_Stats  stats;
}DPU_DopplerProcHWA_OutParams;


DPU_DopplerProcHWA_Handle DPU_DopplerProcHWA_init(DPU_DopplerProcHWA_InitParams *initCfg, int32_t* errCode);
int32_t DPU_DopplerProcHWA_process(DPU_DopplerProcHWA_Handle handle, DPU_DopplerProcHWA_OutParams *outParams);
int32_t DPU_DopplerProcHWA_deinit(DPU_DopplerProcHWA_Handle handle);
int32_t DPU_DopplerProcHWA_config(DPU_DopplerProcHWA_Handle handle, DPU_DopplerProcHWA_Config *cfg);

#ifdef __cplusplus
}
#endif

#endif
