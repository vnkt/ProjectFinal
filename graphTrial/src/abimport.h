/* Import (extern) header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 7 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int ph_main( int argc, char **argv );
int setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void abc( PtWidget_t *widget, PhTile_t *damage ) 

;
int resize( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#endif
