/*
 * Copyright (C) 2003 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute etc. this as long as the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

#include <assert.h>
#include <config.h>
#include <conversions.h>
#include <defines.h>
#include <debugging.h>
#include <dep_processor.h>
#include <enums.h>
#include <listmgmt.h>
#include <lookuptables.h>
#include <notifications.h>
#include "../mtxmatheval/mtxmatheval.h"
#include <rtv_processor.h>
#include <stdio.h>
#include <stdlib.h>
#include <structures.h>
#include <tabloader.h>



extern gint dbg_lvl;
volatile gchar *load_source = NULL;
/*!
 \brief convert_before_download() converts the value passed using the
 conversions bound to the widget
 \param widget (GtkWidget *) widget to extract the conversion info from
 \param value (gfloat *) the "real world" value from the tuning gui before
 translation to MS-units
 \returns the integere ms-units form after conversion
 */
gint convert_before_download(GtkWidget *widget, gfloat value)
{
	gint return_value = 0;
	gint tmpi = 0;
	gchar * conv_expr = NULL;
	void *evaluator = NULL;
	gint page = -1;
	gint offset = -1;
	gint lower = -1;
	gint upper = -1;
	gint i = 0;
	GHashTable *hash = NULL;
	gchar *key_list = NULL;
	gchar *expr_list = NULL;
	gchar **keys = NULL;
	gchar **exprs = NULL;
	gint table_num = 0;
	gchar *tmpbuf = NULL;
	extern gint *algorithm;

	static GStaticMutex mutex = G_STATIC_MUTEX_INIT;

	g_static_mutex_lock(&mutex);

	if (NULL == g_object_get_data(G_OBJECT(widget),"raw_lower"))
		lower = 0; // BAD assumption
	else
		lower = (gint)g_object_get_data(G_OBJECT(widget),"raw_lower");

	if (NULL == g_object_get_data(G_OBJECT(widget),"raw_upper"))
		upper = 255; // BAD assumption
	else
		upper = (gint)g_object_get_data(G_OBJECT(widget),"raw_upper");

	page = (gint)g_object_get_data(G_OBJECT(widget),"page");
	offset = (gint)g_object_get_data(G_OBJECT(widget),"offset");
	if (g_object_get_data(G_OBJECT(widget),"multi_expr_keys"))
	{
		if (!g_object_get_data(G_OBJECT(widget),"dl_eval_hash"))
		{
			hash = g_hash_table_new_full(g_str_hash,g_str_equal,g_free,evaluator_destroy);
			key_list = g_object_get_data(G_OBJECT(widget),"multi_expr_keys");
			expr_list = g_object_get_data(G_OBJECT(widget),"dl_conv_exprs");
			keys = g_strsplit(key_list,",",-1);
			exprs = g_strsplit(expr_list,",",-1);
			for (i=0;i<MIN(g_strv_length(keys),g_strv_length(exprs));i++)
			{
				evaluator = evaluator_create(exprs[i]);
				g_hash_table_insert(hash,g_strdup(keys[i]),evaluator);
			}
			g_strfreev(keys);
			g_strfreev(exprs);

			g_object_set_data(G_OBJECT(widget),"dl_eval_hash",hash);
		}
		hash = g_object_get_data(G_OBJECT(widget),"dl_eval_hash");
		tmpbuf = (gchar *)g_object_get_data(G_OBJECT(widget),"table_num");
		table_num = (gint)strtol(tmpbuf,NULL,10);
		switch (algorithm[table_num])
		{
			case SPEED_DENSITY:
				if (!load_source)
					evaluator = g_hash_table_lookup(hash,"DEFAULT");
				else
				{
					evaluator = g_hash_table_lookup(hash,(gchar *)load_source);
					if (!evaluator)
						evaluator = g_hash_table_lookup(hash,"DEFAULT");
				}
				break;
			case ALPHA_N:
				evaluator = g_hash_table_lookup(hash,"DEFAULT");
				break;
			case MAF:
				evaluator = g_hash_table_lookup(hash,"AFM_VOLTS");
				if (!evaluator)
					evaluator = g_hash_table_lookup(hash,"DEFAULT");
				break;
		}
	}
	else
	{
		conv_expr = (gchar *)g_object_get_data(G_OBJECT(widget),"dl_conv_expr");
		evaluator = (void *)g_object_get_data(G_OBJECT(widget),"dl_evaluator");

		if ((conv_expr) && (!evaluator))
		{
			evaluator = evaluator_create(conv_expr);
			assert(evaluator);
			g_object_set_data(G_OBJECT(widget),"dl_evaluator",(gpointer)evaluator);
		}
	}
	if (!evaluator)
	{
		if (dbg_lvl & CONVERSIONS)
			dbg_func(g_strdup_printf(__FILE__": convert_before_dl()\n\tNO CONVERSION defined for page: %i, offset: %i, value %i\n",page, offset, (gint)value));
		if(value > upper)
		{
			if (dbg_lvl & (CONVERSIONS|CRITICAL))
				dbg_func(g_strdup(__FILE__": convert_before_download()\n\t WARNING value clamped at 255 (no eval)!!\n"));
			value = upper;
		}
		if (value < lower)
		{
			if (dbg_lvl & (CONVERSIONS|CRITICAL))
				dbg_func(g_strdup(__FILE__": convert_before_download()\n\t WARNING value clamped at 0 (no eval)!!\n"));
			value = lower;
		}
		return_value = value;
	}
	else
	{
		return_value = evaluator_evaluate_x(evaluator,value)+0.001;

		if (dbg_lvl & CONVERSIONS)
			dbg_func(g_strdup_printf(__FILE__": convert_before_dl():\n\tpage %i, offset %i, raw %.2f, sent %i\n",page, offset,value,return_value));

		if (return_value > upper)
		{
			if (dbg_lvl & (CONVERSIONS|CRITICAL))
				dbg_func(g_strdup(__FILE__": convert_before_download()\n\t WARNING value clamped at 255 (evaluated)!!\n"));
			return_value = upper;
		}
		if (return_value < lower)
		{
			if (dbg_lvl & (CONVERSIONS|CRITICAL))
				dbg_func(g_strdup(__FILE__": convert_before_download()\n\t WARNING value clamped at 0 (evaluated)!!\n"));
			return_value = lower;
		}
	}

	tmpi = return_value;
	 if (g_object_get_data(G_OBJECT(widget),"lookuptable"))
		return_value = reverse_lookup(G_OBJECT(widget),tmpi);

	g_static_mutex_unlock(&mutex);
	return (return_value);
}


/*!
 \brief convert_after_upload() converts the ms-units dat to the real world
 units for display on the GUI
 \param widget (GtkWidget *) to extract the conversion info from to perform
 the necessary math
 \returns the real world value for the GUI
 */
gfloat convert_after_upload(GtkWidget * widget)
{
	gfloat return_value = 0.0;
	gchar * conv_expr = NULL;
	void *evaluator = NULL;
	extern gint **ms_data;
	gint tmpi = 0;
	gint page = -1;
	gint offset = -1;
	gboolean ul_complex = FALSE;
	gint i = 0;
	GHashTable *hash = NULL;
	gchar *key_list = NULL;
	gchar *expr_list = NULL;
	gchar **keys = NULL;
	gchar **exprs = NULL;
	gchar * tmpbuf = NULL;
	gint table_num = 0;
	extern gint *algorithm;
	static GStaticMutex mutex = G_STATIC_MUTEX_INIT;

	g_static_mutex_lock(&mutex);

	ul_complex = (gboolean)g_object_get_data(G_OBJECT(widget),"ul_complex");
	if (ul_complex)
	{
		g_static_mutex_unlock(&mutex);
		return handle_complex_expr(G_OBJECT(widget),NULL,UPLOAD);
	}

	page = (gint)g_object_get_data(G_OBJECT(widget),"page");
	offset = (gint)g_object_get_data(G_OBJECT(widget),"offset");
	if (g_object_get_data(G_OBJECT(widget),"multi_expr_keys"))
	{
		if (!g_object_get_data(G_OBJECT(widget),"ul_eval_hash"))
		{
			hash = g_hash_table_new_full(g_str_hash,g_str_equal,g_free,evaluator_destroy);
			key_list = g_object_get_data(G_OBJECT(widget),"multi_expr_keys");
			expr_list = g_object_get_data(G_OBJECT(widget),"ul_conv_exprs");
			keys = g_strsplit(key_list,",",-1);
			exprs = g_strsplit(expr_list,",",-1);
			for (i=0;i<MIN(g_strv_length(keys),g_strv_length(exprs));i++)
			{
				evaluator = evaluator_create(exprs[i]);
				g_hash_table_insert(hash,g_strdup(keys[i]),evaluator);
			}
			g_strfreev(keys);
			g_strfreev(exprs);

			g_object_set_data(G_OBJECT(widget),"ul_eval_hash",hash);
		}
		hash = g_object_get_data(G_OBJECT(widget),"ul_eval_hash");
		tmpbuf = (gchar *)g_object_get_data(G_OBJECT(widget),"table_num");
		table_num = (gint)strtol(tmpbuf,NULL,10);
		switch (algorithm[table_num])
		{
			case SPEED_DENSITY:
				if (!load_source)
					evaluator = g_hash_table_lookup(hash,"DEFAULT");
				else
				{
					evaluator = g_hash_table_lookup(hash,(gchar *)load_source);
					if (!evaluator)
						evaluator = g_hash_table_lookup(hash,"DEFAULT");
				}
				break;
			case ALPHA_N:
				evaluator = g_hash_table_lookup(hash,"DEFAULT");
				break;
			case MAF:
				evaluator = g_hash_table_lookup(hash,"AFM_VOLTS");
				if (!evaluator)
					evaluator = g_hash_table_lookup(hash,"DEFAULT");
				break;
		}
	}
	else
	{
		conv_expr = (gchar *)g_object_get_data(G_OBJECT(widget),"ul_conv_expr");
		evaluator = (void *)g_object_get_data(G_OBJECT(widget),"ul_evaluator");
		if ((conv_expr) && (!evaluator)) 	/* if no evaluator create one */
		{
			evaluator = evaluator_create(conv_expr);
			assert(evaluator);
			g_object_set_data(G_OBJECT(widget),"ul_evaluator",(gpointer)evaluator);
		}

	}
	if (g_object_get_data(G_OBJECT(widget),"lookuptable"))
		tmpi = lookup_data(G_OBJECT(widget),ms_data[page][offset]);
	else
		tmpi = ms_data[page][offset];


	if (!evaluator)
	{
		return_value = tmpi;
		if (dbg_lvl & CONVERSIONS)
			dbg_func(g_strdup_printf(__FILE__": convert_after_ul():\n\tNO CONVERSION defined for page: %i, offset: %i, value %f\n",page, offset, return_value));
		g_static_mutex_unlock(&mutex);
		return (return_value);		
	}
	return_value = evaluator_evaluate_x(evaluator,tmpi)+0.0001;

	if (dbg_lvl & CONVERSIONS)
		dbg_func(g_strdup_printf(__FILE__": convert_after_ul()\n\t page %i,offset %i, raw %i, val %f\n",page,offset,tmpi,return_value));
	g_static_mutex_unlock(&mutex);
	return (return_value);
}


/*!
 \brief convert_temps() changes the values of controls based on the currently
 selected temperature scale.  IT works for labels, spinbuttons, etc...
 \param widget (gpointer) pointer to the widget that contains the necessary
 paramaters re temp (Alt label, etc)
 \param units (gpointer) the temp scale selected
 */
void convert_temps(gpointer widget, gpointer units)
{
	GObject *dep_obj = NULL;
	gfloat upper = 0.0;
	gfloat lower = 0.0;
	gfloat value = 0.0;
	GtkAdjustment * adj = NULL;
	gchar *text = NULL;
//	gchar *depend_on = NULL;
	gboolean state = FALSE;
	gint widget_temp = -1;
	extern GdkColor black;
	extern volatile gboolean leaving;

	/* If this widgt depends on anything call check_dependancy which will
	 * return TRUE/FALSE.  True if what it depends on is in the matching
	 * state, FALSE otherwise...
	 */
//	depend_on = (gchar *)g_object_get_data(G_OBJECT(widget),"depend_on");
	if ((!widget) || (leaving))
		return;
	dep_obj = (GObject *)g_object_get_data(G_OBJECT(widget),"dep_object");
	widget_temp = (gint)g_object_get_data(G_OBJECT(widget),"widget_temp");
	if (dep_obj)
		state = check_dependancies(G_OBJECT(dep_obj));


	if ((int)units == FAHRENHEIT) 
	{
		if (GTK_IS_LABEL(widget))
		{
			if ((dep_obj) && (state))	
				text = (gchar *)g_object_get_data(G_OBJECT(widget),"alt_f_label");
			else
				text = (gchar *)g_object_get_data(G_OBJECT(widget),"f_label");
			gtk_label_set_text(GTK_LABEL(widget),text);
			gtk_widget_modify_text(widget,GTK_STATE_NORMAL,&black);
			g_object_set_data(G_OBJECT(widget),"widget_temp",GINT_TO_POINTER(units));

		}

		if ((GTK_IS_SPIN_BUTTON(widget)) && (widget_temp == CELSIUS))
		{

			adj = (GtkAdjustment *) gtk_spin_button_get_adjustment(
					GTK_SPIN_BUTTON(widget));
			upper = adj->upper;
			value = adj->value;
			lower = adj->lower;
			adj->value = (value *(9.0/5.0))+32;
			adj->upper = (upper *(9.0/5.0))+32;
			adj->lower = (lower *(9.0/5.0))+32;

			gtk_adjustment_changed(adj);
			gtk_spin_button_set_value(
					GTK_SPIN_BUTTON(widget),
					adj->value);
			gtk_widget_modify_text(widget,GTK_STATE_NORMAL,&black);
			g_object_set_data(G_OBJECT(widget),"widget_temp",GINT_TO_POINTER(units));
		}
		if ((GTK_IS_RANGE(widget)) && (widget_temp == CELSIUS))
		{
			adj = (GtkAdjustment *) gtk_range_get_adjustment(
					GTK_RANGE(widget));
			upper = adj->upper;
			lower = adj->lower;
			value = adj->value;
			adj->value = (value *(9.0/5.0))+32;
			adj->lower = (lower *(9.0/5.0))+32;
			adj->upper = (upper *(9.0/5.0))+32;

			gtk_range_set_adjustment(GTK_RANGE(widget),adj);
			g_object_set_data(G_OBJECT(widget),"widget_temp",GINT_TO_POINTER(units));
		}
	}
	else
	{
		if (GTK_IS_LABEL(widget))
		{
			if ((dep_obj) && (state))	
				text = (gchar *)g_object_get_data(G_OBJECT(widget),"alt_c_label");
			else
				text = (gchar *)g_object_get_data(G_OBJECT(widget),"c_label");
			gtk_label_set_text(GTK_LABEL(widget),text);
			gtk_widget_modify_text(widget,GTK_STATE_NORMAL,&black);
			g_object_set_data(G_OBJECT(widget),"widget_temp",GINT_TO_POINTER(units));
		}

		if ((GTK_IS_SPIN_BUTTON(widget)) && (widget_temp == FAHRENHEIT))
		{
			adj = (GtkAdjustment *) gtk_spin_button_get_adjustment(
					GTK_SPIN_BUTTON(widget));
			upper = adj->upper;
			lower = adj->lower;
			value = adj->value;
			adj->value = (value-32)*(5.0/9.0);
			adj->lower = (lower-32)*(5.0/9.0);
			adj->upper = (upper-32)*(5.0/9.0);
			gtk_adjustment_changed(adj);
			gtk_spin_button_set_value(
					GTK_SPIN_BUTTON(widget),
					adj->value);
			gtk_widget_modify_text(widget,GTK_STATE_NORMAL,&black);
			g_object_set_data(G_OBJECT(widget),"widget_temp",GINT_TO_POINTER(units));
		}
		if ((GTK_IS_RANGE(widget)) && (widget_temp == FAHRENHEIT))
		{
			adj = (GtkAdjustment *) gtk_range_get_adjustment(
					GTK_RANGE(widget));
			upper = adj->upper;
			lower = adj->lower;
			value = adj->value;
			adj->value = (value-32)*(5.0/9.0);
			adj->lower = (lower-32)*(5.0/9.0);
			adj->upper = (upper-32)*(5.0/9.0);

			gtk_range_set_adjustment(GTK_RANGE(widget),adj);
			g_object_set_data(G_OBJECT(widget),"widget_temp",GINT_TO_POINTER(units));
		}
	}

}


/*!
 \brief reset_temps() calls the convert_temps function for each widget in
 the "temperature" list
 \param type (gpointer) the temp scale now selected
 */
void reset_temps(gpointer type)
{
	/* Better way.. :) */
	g_list_foreach(get_list("temperature"),convert_temps,type);

}
