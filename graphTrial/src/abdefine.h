/* Define header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_average;
#define ABW_average                          AbGetABW( ABN_average )
extern const int ABN_graph_base_raw;
#define ABW_graph_base_raw                   AbGetABW( ABN_graph_base_raw )
extern const int ABN_average_label;
#define ABW_average_label                    AbGetABW( ABN_average_label )
extern const int ABN_x_axis;
#define ABW_x_axis                           AbGetABW( ABN_x_axis )
extern const int ABN_yaxis;
#define ABW_yaxis                            AbGetABW( ABN_yaxis )
extern const int ABN_y_unit;
#define ABW_y_unit                           AbGetABW( ABN_y_unit )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

#if defined(__cplusplus)
}
#endif

