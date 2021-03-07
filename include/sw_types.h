/**
 * SchmartWatsch Firmware - Common types
 * 
 * Author: Bastian Neumann 2018
 * 
 */

#ifndef SW_TYPES_H
#define SW_TYPES_H

/**
 * @brief Enumerator with available rotations.
 */
typedef enum
{
    RET_ERROR = -99,      /**< Error occured. */
    NO_CALLBACK,          /**< No callback function found. */
    NOT_COMPATIBLE,       /**< Interface is not compatible. */
    OUT_OF_BOUNDS,        /**< Values where out of bounds error occured. */
    BUSY,                 /**< Device is busy. */
    DELAYED,              /**< Execution is delayed and execution is scheduled. */
    NO_ACTION_NEEDED,     /**< No execution is necessary. */
    NOT_INITIALIZED,      /**< Driver was used before initalization */
    SUCCESS = 0,          /**< No error occured. */
    NOT_IMPLEMENTED = 99, /**< Function not implemented yet. */
} return_code_t;

/**
 * @brief Driver state.
 */
typedef enum
{
    DRV_UNINITIALIZED,     ///< Uninitialized.
    DRV_STATE_INITIALIZED, ///< Initialized but powered off.
    DRV_STATE_POWERED_ON,  ///< Initialized and powered on.
    DRV_STATE_IDLE,        ///< Driver is waiting.
    DRV_STATE_BUSY,        ///< Driver is busy do not disturb.
} drv_state_t;

#define ABS(a) ((a) < 0 ? -(a) : (a))
#define SIGN(a) ((a) < 0 ? -1 : 1)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

#endif // SW_TYPES_H