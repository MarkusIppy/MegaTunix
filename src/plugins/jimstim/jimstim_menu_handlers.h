/*
 * Copyright (C) 2002-2012 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
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

/*!
  \file src/plugins/jimstim/jimstim_menu_handlers.h
  \ingroup JimStimPlugin,Headers
  \brief JimStim menu handlers
  \author David Andruczyk
  */
 
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __JIMSTIM_MENU_HANDLERS_H__
#define __JIMSTIM_MENU_HANDLERS_H__

#include <defines.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <threads.h>

/* Prototypes */
void ecu_plugin_menu_setup(GladeXML *);
/* Prototypes */

#endif
#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
