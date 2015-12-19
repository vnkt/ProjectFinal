/* Event header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 1012, init_start, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &data_acquisition_window, NULL, "1553_data_analysis", 2009, data_analysis, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &real_time_analysis, NULL, "real_time_data", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base_connectivity_check_button", 2009, base_test_connectivity_callback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_data_analysis_window[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "data_analysis_window", 1012, realised_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtana_show_data_button", 2009, show_data_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtana_rt_select_combo", 23010, activate_subaddress_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtana_subaddr_select_combo", 23010, activate_wordcount_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtana_add_combo_button", 2009, add_combo_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtana_cancel_button", 2009, cancel_callback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_package_analysis_window[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "package_analysis_window", 18024, packana_window_close_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "packana_stop_button", 2009, packana_stop_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "packana_ok_button", 2009, packan_ok_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "packana_rt_graph_button", 2009, rtgraph, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_real_time_analysis[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "real_time_analysis", 1012, rtana_realized_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_package_select_combo", 23010, rtana_hover_package, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_available_fields_list", 23010, rtana_field_selected_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_add_field_button", 2009, add_field_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_selected_package_list", 23010, display_fields_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_select_package_button", 2009, add_package_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_remove_field_button", 2009, remove_field_callback, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &package_analysis_window, NULL, "rtana_okay_button", 2009, packana_post_realise_callback, 1, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_available_groups_list", 23010, rtana_group_selected_callback, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &new_group_window, NULL, "rtana_create_new_group_button", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "rtana_cancel_button", 2009, rtana_cancel_callback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_new_group_window[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "new_group_window", 1012, newgrp_realised_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "newgrp_create_group_button", 2009, create_group_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "newgrp_add_field_button", 2009, add_fields_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "newgrp_remove_field_button", 2009, remove_fields_callback, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_data_acquisition_window[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtacq_start_data_acquisition_button", 2009, dtacq_start_da_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtacq_file_select_button", 2009, dtacq_select_file_callback, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtacq_stop_data_acquisition_button", 2009, dtacq_stop_da_callback, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &data_analysis_window, NULL, "dtacq_data_analysis_button", 2009, data_analysis, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "dtana_close_button", 2009, dtana_cancel_callback, 0, 0, 0, 0, },
	{ 0 }
	};

const char ApOptions[] = AB_OPTIONS;

#if defined(__cplusplus)
}
#endif

