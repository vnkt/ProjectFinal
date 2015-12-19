/* Import (extern) header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWindowLink_t data_analysis_window;
extern ApWindowLink_t package_analysis_window;
extern ApWindowLink_t real_time_analysis;
extern ApWindowLink_t new_group_window;
extern ApWindowLink_t data_acquisition_window;
extern ApWidget_t AbWidgets[ 68 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int data_analysis( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dtacq_select_file_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dtacq_start_da_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dtacq_stop_da_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int init_start( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int realised_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int base_test_connectivity_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int activate_subaddress_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int activate_wordcount_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int show_data_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int add_combo_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int add_field_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int add_package_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int remove_field_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int display_fields_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int packana_stop_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int packana_post_realise_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rtana_realized_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int packana_window_close_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int add_fields_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int remove_fields_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int create_group_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int newgrp_realised_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rtana_field_selected_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rtana_group_selected_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int packan_ok_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rtana_hover_package( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rtgraph( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int dtana_cancel_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cancel_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rtana_cancel_callback( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#endif
