#ifndef VV_COMMON_H
#define VV_COMMON_H

typedef void (*finishCallback)(void* inputData,void* outputData);

enum VVResultCode
{
   vVSuccess=0,
   vVWrongParams,
   vVExecutionFailure,
   vVFailure
};
#endif /*VV_COMMON_H*/
