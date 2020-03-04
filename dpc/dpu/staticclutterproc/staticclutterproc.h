/**
 *   @file  staticclutterproc.h
 *
 *   @brief
 *      staticclutterproc DPU include file
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
#ifndef DPU_STATICCLUTTERPROC_H
#define DPU_STATICCLUTTERPROC_H

/* Data Path include files */
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpif/dpif_radarcube.h>

/** @defgroup STATICCLUTTER_PROC_DPU_EXTERNAL       staticClutterProc DPU External
 */

/**
@defgroup DPU_STATICCLUTTERPROC_EXTERNAL_FUNCTION            staticclutterproc DPU External Functions
@ingroup STATICCLUTTER_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the exported APIs which the application needs to
*   invoke in order to use the DPU
*/
/**
@defgroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE      staticclutterproc DPU External Data Structures
@ingroup STATICCLUTTER_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup DPU_STATICCLUTTERPROC_ERROR_CODE                   staticclutterproc DPU Error Codes
@ingroup STATICCLUTTER_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the error codes which are generated by the staticclutterproc DPU
*/

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************
 ****        staticclutterproc DPU exposed definitions     ****
 **************************************************************/
/**
 * @brief   Error Code: Invalid argument
 *  \ingroup DPU_STATICCLUTTERPROC_ERROR_CODE
 *  Base error code for the static clutter proc is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 */
#define DPU_STATICCLUTTERPROC_EINVAL        (DP_ERRNO_STATIC_CLUTTER_PROC_BASE-1)

/**
 * @brief   Error Code: No memory
 *  \ingroup DPU_STATICCLUTTERPROC_ERROR_CODE
 */
#define DPU_STATICCLUTTERPROC_ENOMEM        (DP_ERRNO_STATIC_CLUTTER_PROC_BASE-2)

/**
 * @brief   Error Code: Scratch buffer has incorrect size or incorrect alignment
 *  \ingroup DPU_STATICCLUTTERPROC_ERROR_CODE
 */
#define DPU_STATICCLUTTERPROC_ESCRATCHBUF   (DP_ERRNO_STATIC_CLUTTER_PROC_BASE-3)

/**
 * @brief   Required alignment for scratch buffer provided by application
 *          (if DPU is running on DSP (C674X))
 */
#define DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_DSP  8

/**
 * @brief   Required alignment for scratch buffer provided by application
 *          (if DPU is running on R4F)
 */
#define DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_R4F  4

/*!
 *  @brief   Handle for static clutter DPU.
 */
typedef void*  DPU_StaticClutterProc_Handle;

/**
 * @brief
 *  staticclutterproc DPU EDMA configuration parameters
 *
 * @details
 *  The structure is used to hold the EDMA configuration parameters
 *  for the static clutter removal DPU
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_EdmaCfg_t
{
    /*! @brief  EDMA Ping channel. */
    DPEDMA_ChanCfg  ping;
    
    /*! @brief  EDMA Pong channel. */
    DPEDMA_ChanCfg  pong;    
}DPU_StaticClutterProc_EdmaCfg;

/**
 * @brief
 *  Scratch buffer used by DPU
 *
 * @details
 *  The structure defines the scratch buffer interface, including
 *  size and data pointer. This buffer should be allocated from
 *  internal memory (TCMB if running on R4F and L2/L1 SRAM on DSP) as the 
 *  clutter removal algorithm will be performed on data sitting 
 *  on this buffer.
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_ScratchBuf_t
{
    /*! @brief  Scratch buffer size in bytes.\n
        Size must be at least:\n
        Y*2*sizeof(cmplx16ImRe_t)\n
        Where:\n
        For R4F: Y is equal to numDopplerChirps\n
        For DSP (C674X): Y is the smallest multiple of 4 greater or equal to numDopplerChirps\n
    */
    uint32_t                bufSize;

    /*! @brief  Scratch buffer data pointer.\n
        For R4F:\n
        Byte alignment Requirement = @ref DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_R4F \n
        For DSP (C674X):\n
        Byte alignment Requirement = @ref DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_DSP \n
    */
    void                    *buf;
}DPU_StaticClutterProc_ScratchBuf;

/**
 * @brief
 *  staticclutterproc DPU HW configuration parameters
 *
 * @details
 *  The structure is used to hold the  HW configuration parameters
 *  for the static clutter removal DPU
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_HW_Resources_t
{
    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;
    
    /*! @brief  EDMA configuration for Input data (Radar cube -> Scratch buffer). */
    DPU_StaticClutterProc_EdmaCfg edmaIn;
    
    /*! @brief  EDMA configuration for Output data (Scratch buffer -> Radar cube). */
    DPU_StaticClutterProc_EdmaCfg edmaOut;
    
    /*! @brief  Radar Cube. This DPU supports only one cube format: @ref DPIF_RADARCUBE_FORMAT_1 */
    DPIF_RadarCube radarCube;
    
    /*! @brief  Scratch buffer */
    DPU_StaticClutterProc_ScratchBuf scratchBuf;
}DPU_StaticClutterProc_HW_Resources;


/**
 * @brief
 *  Doppler DPU static configuration parameters
 *
 * @details
 *  The structure is used to hold the static configuration parameters
 *  for the Doppler DPU
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_StaticConfig_t
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
    
}DPU_StaticClutterProc_StaticConfig;

/**
 * @brief
 *  staticclutterproc DPU configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for the static clutter removal DPU
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_Config_t
{
    /*! @brief  HW resources. */
    DPU_StaticClutterProc_HW_Resources  hwRes;
    
    /*! @brief Static configuration. */
    DPU_StaticClutterProc_StaticConfig  staticCfg;
    
}DPU_StaticClutterProc_Config;

/**
 * @brief
 *  Data processing unit statistics
 *
 * @details
 *  The structure is used to hold the statistics of the DPU 
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_Stats_t
{
    /*! @brief total processing time for one execution of the DPU*/
    uint32_t   processingTime;
}DPU_StaticClutterProc_Stats;


/**
 * @brief
 *  DPU processing output parameters
 *
 * @details
 *  The structure is used to hold the output parameters DPU processing
 *
 *  \ingroup DPU_STATICCLUTTERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_OutParams_t
{
    /*! @brief DPU statistics */
    DPU_StaticClutterProc_Stats  stats;
}DPU_StaticClutterProc_OutParams;

/**************************************************************
 ****               staticclutterproc DPU exposed APIs     ****
 **************************************************************/
/**
 *  @b Description
 *  @n
 *      staticclutterproc DPU init function. It allocates memory to store
 *  DPU's internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]  errCode      Pointer to errCode generates from the API
 *                           @ref DPU_STATICCLUTTERPROC_ERROR_CODE
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid handle
 *  @retval
 *      Error       - NULL
 */
DPU_StaticClutterProc_Handle DPU_StaticClutterProc_init(int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      staticclutterproc DPU deinit function. It release resources used for the DPU.
 *
 *  @param[in]  handle                  staticclutterproc handle.
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error       !=0  @ref DPU_STATICCLUTTERPROC_ERROR_CODE
 */
int32_t DPU_StaticClutterProc_deinit(DPU_StaticClutterProc_Handle handle);

/**
 *  @b Description
 *  @n
 *      staticclutterproc DPU process function.
 *      This function executes the static clutter removal algorithm on data sitting
 *      on the scratch buffer in a ping/pong fashion.\n
 *      The data is first brought from radarCube into scratch buffer by EDMA,
 *      then static clutter removal is performed and data is moved back by EDMA
 *      from scratch buffer to radarCube. All hardware resources (EDMA properties),
 *      buffers and static configuration are provided by application through
 *      @ref DPU_StaticClutterProc_config.\n
 *      Note that the subtraction step of the static clutter removal
 *      algorithm performs a saturated subtraction on every sample.
 *
 *  @param[in]  handle                  staticclutterproc handle.
 *  @param[out] outParams               Output parameters.
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error       !=0 @ref DPU_STATICCLUTTERPROC_ERROR_CODE
 */
int32_t DPU_StaticClutterProc_process(DPU_StaticClutterProc_Handle handle, 
                                     DPU_StaticClutterProc_OutParams *outParams);

/**
 *  @b Description
 *  @n
 *      staticclutterproc DPU config function. It stores the data path parameters
 *  and configures relevant components that will be used in its process and.
 *
 *  @param[in]  handle     staticclutterproc handle.
 *  @param[in]  cfg        staticclutterproc configuration.0
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error       !=0  @ref DPU_STATICCLUTTERPROC_ERROR_CODE
 */
int32_t DPU_StaticClutterProc_config(DPU_StaticClutterProc_Handle    handle,
                                     DPU_StaticClutterProc_Config    *cfg);
                                    


 
#ifdef __cplusplus
}
#endif

#endif