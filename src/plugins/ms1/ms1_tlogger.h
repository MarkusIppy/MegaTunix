/*
 * Copyright (C) 2003 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute, etc. this as long as all the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

#ifndef __MS1_T_LOGGER_H__
#define __MS1_T_LOGGER_H__

#include <gtk/gtk.h>
#include <defines.h>

/* Externs */
extern void (*error_msg_f)(const gchar *);
extern void (*io_cmd_f)(const gchar *,void *);
extern void (*dbg_func_f)(int,gchar *);
extern void (*start_tickler_f)(gint);
extern void (*stop_tickler_f)(gint);
extern void (*signal_read_rtvars_f)(void);
extern gint (*ms_get_ecu_data_f)(gint,gint,gint,DataSize);
extern GtkWidget *(*lookup_widget_f)(const gchar *);
extern gboolean (*lookup_current_value_f)(const gchar * internal_name, gfloat *value);
extern GdkGC *(*initialize_gc_f)(GdkDrawable *, GcType);
/* Externs */

typedef enum {
	TOOTHMON_TICKLER=0x91,
	TRIGMON_TICKLER
}EcuPluginTickler;

typedef struct _TTMon_Data TTMon_Data;
/*!
 * \brief _TTMon_Data struct is a container used to hold private data
 * for the Trigger and Tooth Loggers (MSnS-E only)
 */
struct _TTMon_Data
{
	gboolean stop;		/*! Stop display */
	gfloat zoom;		/*! Zoom */
	gint page;		/*! page used to discern them apart */
	GdkPixmap *pixmap;	/*! Pixmap */
	GtkWidget *darea;	/*! Pointer to drawing area */
	gint min_time;		/*! Minimum, trigger/tooth time */
	gint num_maxes;		/*! Hot many long pips per block */
	gint mins_inbetween;	/*! How many normal teeth */
	gint max_time;		/*! Maximum, trigger/tooth time */
	gint midpoint_time;	/*! avg between min and max */
	gint est_teeth;		/*! Estimated number of teeth */
	gint units;		/*! Units multiplier */
	gint missing;		/*! Number of missing teeth */
	gint sample_time;	/*! Time delay between reads.. */
	gint capabilities;	/*! Enum of ECU capabilities */
	gfloat usable_begin;	/*! Usable begin point for bars */
	gfloat font_height;	/*! Font height needed for some calcs */
	gfloat rpm;		/*! Current RPM */
	gushort *current;	/*! Current block of times */
	gushort *last;		/*! Last block of times */
	gushort *captures;	/*! Array of capture points */
	gint wrap_pt;		/*! Wrap point */
	gint vdivisor;		/*! Vertical scaling divisor */
	gfloat peak;		/*! Vertical Peak Value */
	PangoFontDescription *font_desc;	/*! Pango Font Descr */
	PangoLayout *layout;	/*! Pango Layout */
	GdkGC *axis_gc;		/*! axis graphics context */
	GdkGC *trace_gc;	/*! axis graphics context */
};

/* Prototypes */
void setup_logger_display(GtkWidget *);
void reset_ttm_buttons(void);
gboolean logger_display_config_event(GtkWidget *, GdkEventConfigure *, gpointer);
gboolean logger_display_expose_event(GtkWidget *, GdkEventExpose *, gpointer);
gboolean ms1_tlogger_button_handler(GtkWidget *, gpointer);
void _crunch_trigtooth_data(gint);
void crunch_trigtooth_data_pf(void);
void update_trigtooth_display(gint);
void update_trigtooth_display_pf(void);
void bind_ttm_to_page(gint page);
void start(EcuPluginTickler);
void stop(EcuPluginTickler);
gboolean signal_toothtrig_read(EcuPluginTickler);
/* Prototypes */

#endif
