/* Link header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 4
	};

ApWindowLink_t data_analysis_window = {
	"data_analysis_window.wgtw",
	&AbContext,
	AbLinks_data_analysis_window, 8, 6
	};

ApWindowLink_t package_analysis_window = {
	"package_analysis_window.wgtw",
	&AbContext,
	AbLinks_package_analysis_window, 21, 4
	};

ApWindowLink_t real_time_analysis = {
	"real_time_analysis.wgtw",
	&AbContext,
	AbLinks_real_time_analysis, 29, 11
	};

ApWindowLink_t new_group_window = {
	"new_group_window.wgtw",
	&AbContext,
	AbLinks_new_group_window, 42, 4
	};

ApWindowLink_t data_acquisition_window = {
	"data_acquisition_window.wgtw",
	&AbContext,
	AbLinks_data_acquisition_window, 49, 5
	};


#if defined(__cplusplus)
}
#endif

