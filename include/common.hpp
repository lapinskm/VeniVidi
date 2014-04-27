#ifndef VV_COMMON_H
#define VV_COMMON_H

typedef void (*finishCallback)(void* inputData,void* outputData);

typedef enum
{
   vVSuccess=0,
   vVWrongParams,
   vVExecutionFailure,
   vVFailure
}
VVResultCode;
#endif /*VV_COMMON_H*/
