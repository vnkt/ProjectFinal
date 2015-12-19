/* Import (extern) header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 2 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int resize( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void abc( PtWidget_t *widget, PhTile_t *damage ) 

;
#endif
