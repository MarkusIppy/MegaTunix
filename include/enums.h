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

#ifndef __ENUMS_H__
#define __ENUMS_H__

/* Serial data comes in and handled by the following possibilities.
 * dataio.c uses these to determine which course of action to take...
 */
typedef enum
{
	REALTIME_VARS=0,
	VE_BLOCK,
	RAW_MEMORY_DUMP,
	C_TEST,
	GET_ERROR,
	NULL_HANDLER
}InputHandler;

/* Regular Buttons */
typedef enum
{
	START_REALTIME = 0x20,
	STOP_REALTIME,
	START_PLAYBACK,
	STOP_PLAYBACK,
	READ_VE_CONST,
	READ_RAW_MEMORY,
	BURN_MS_FLASH,
	INTERROGATE_ECU,
	OFFLINE_MODE,
	SELECT_DLOG_EXP,
	SELECT_DLOG_IMP,
	DLOG_SELECT_DEFAULTS,
	DLOG_SELECT_ALL,
	DLOG_DESELECT_ALL,
	DLOG_DUMP_INTERNAL,
	CLOSE_LOGFILE,
	START_DATALOGGING,
	STOP_DATALOGGING,
	EXPORT_VETABLE,
	IMPORT_VETABLE,
	REBOOT_GETERR,
	REVERT_TO_BACKUP,
	BACKUP_ALL,
	RESTORE_ALL,
	SELECT_PARAMS,
	REQ_FUEL_POPUP,
	RESCALE_TABLE,
	REQFUEL_RESCALE_TABLE,
	EXPORT_SINGLE_TABLE,
	IMPORT_SINGLE_TABLE,
	TE_TABLE,
	TE_TABLE_GROUP
}StdButton;

/* Toggle/Radio buttons */
typedef enum
{
	TOOLTIPS_STATE=0x50,
	TRACKING_FOCUS,
        FAHRENHEIT,
        CELSIUS,
	COMMA,
	TAB,
	REALTIME_VIEW,
	PLAYBACK_VIEW,
	HEX_VIEW,
	BINARY_VIEW,
	DECIMAL_VIEW,
	OFFLINE_FIRMWARE_CHOICE,
	START_TOOTHMON_LOGGER,
	START_TRIGMON_LOGGER,
	STOP_TOOTHMON_LOGGER,
	STOP_TRIGMON_LOGGER,
	START_COMPOSITEMON_LOGGER,
	STOP_COMPOSITEMON_LOGGER,
	COMM_AUTODETECT
}ToggleButton;

	
/* spinbuttons... */
typedef enum
{
	REQ_FUEL_DISP=0x80,
	REQ_FUEL_CYLS,
	REQ_FUEL_RATED_INJ_FLOW,
	REQ_FUEL_RATED_PRESSURE,
	REQ_FUEL_ACTUAL_PRESSURE,
	REQ_FUEL_AFR,
	LOCKED_REQ_FUEL,
	REQ_FUEL_1,
	REQ_FUEL_2,
	SER_INTERVAL_DELAY,
	RTSLIDER_FPS,
	RTTEXT_FPS,
	DASHBOARD_FPS,
	VE3D_FPS,
	SET_SER_PORT,
	NUM_SQUIRTS_1,
	NUM_SQUIRTS_2,
	NUM_CYLINDERS_1,
	NUM_CYLINDERS_2,
	NUM_INJECTORS_1,
	NUM_INJECTORS_2,
	TRIGGER_ANGLE,
	ODDFIRE_ANGLE,
	LOGVIEW_ZOOM,
	DEBUG_LEVEL,
	GENERIC,
	BAUD_CHANGE,
	MULTI_EXPRESSION,
	ALT_SIMUL
}MtxButton;

/* Runtime Status flags */
typedef enum 
{       
	STAT_CONNECTED = 0, 
        STAT_CRANKING, 
        STAT_RUNNING, 
        STAT_WARMUP, 
        STAT_AS_ENRICH, 
        STAT_ACCEL, 
        STAT_DECEL
}RuntimeStatus;

typedef enum
{
	VE_EXPORT = 0xb0,
	VE_IMPORT,
	DATALOG_INT_DUMP,
	DATALOG_EXPORT,
	DATALOG_IMPORT,
	FIRMWARE_LOAD,
	FULL_BACKUP,
	FULL_RESTORE
}FileIoType;


typedef enum
{	
	RED=0xc0,
	BLACK,
	GREEN,
	BLUE
}GuiColor;

typedef enum
{
	FONT=0x100,
	TRACE,
	GRATICULE,
	HIGHLIGHT,
	TTM_AXIS,
	TTM_TRACE
}GcType;

typedef enum
{	/* up to 32 Capability flags.... */
	/* No capabilities == Standard B&G code with no modifications */
	MS1		= 1<<0,
	MS1_STD		= 1<<1,
	MSNS_E		= 1<<2,
	MS1_DT		= 1<<3,
	MS2		= 1<<4,
	MS2_STD		= 1<<5,
	MS2_EXTRA	= 1<<6,
	MS2_EXTRA_COMPOSITEMON	= 1<<7
}Capability;

typedef enum
{
	WRITE_CMD = 0x130,
	NULL_CMD,
	FUNC_CALL
}CmdType;

typedef enum
{
	UPD_LOGBAR=0x180,
	UPD_WIDGET,
/*	UPD_RAW_MEMORY,*/
	UPD_RUN_FUNCTION
}UpdateFunction;

typedef enum
{
	MTX_INT = 0x190,
	MTX_ENUM,
	MTX_BOOL,
	MTX_FLOAT,
	MTX_STRING
}DataType;

typedef enum
{
	ABOUT_TAB=0x1a0,
	GENERAL_TAB,
	COMMS_TAB,
	ENG_VITALS_TAB,
	CONSTANTS_TAB,
	DT_PARAMS_TAB,
	IGNITON_TAB,
	RUNTIME_TAB,
	ACCEL_WIZ_TAB,
	ENRICHMENTS_TAB,
	TUNING_TAB,
	TOOLS_TAB,
	RAW_MEM_TAB,
	WARMUP_WIZ_TAB,
	VETABLES_TAB,
	SPARKTABLES_TAB,
	AFRTABLES_TAB,
	ALPHA_N_TAB,
	BOOSTTABLES_TAB,
	ROTARYTABLES_TAB,
	DATALOGGING_TAB,
	LOGVIEWER_TAB,
	VE3D_VIEWER_TAB,
	ERROR_STATUS_TAB
}TabIdent;

typedef enum
{
	VE_EMB_BIT=0x1c0,
	VE_VAR,
	RAW_VAR,
	RAW_EMB_BIT
}ComplexExprType;

typedef enum
{
	UPLOAD=0x1c8,
	DOWNLOAD,
	RTV,
	GAUGE
}ConvType;

typedef enum
{
	MTX_HEX=0x1d0,
	MTX_DECIMAL
}Base;

typedef enum
{
	MTX_ENTRY=0x1e0,
	MTX_TITLE,
	MTX_LABEL,
	MTX_RANGE,
	MTX_SPINBUTTON,
	MTX_PROGRESS,
	MTX_SCALE
}WidgetType;


typedef enum
{
	RTV_TICKLER=0x210,
	LV_PLAYBACK_TICKLER,
	TOOTHMON_TICKLER,
	TRIGMON_TICKLER,
	COMPOSITEMON_TICKLER,
	SCOUNTS_TICKLER
}TicklerType;

typedef enum
{
	ALPHA_N=0x220,
	SPEED_DENSITY,
	MAF,
	SD_AN_HYBRID,
	MAF_AN_HYBRID,
	SD_MAF_HYBRID
}Algorithm;

typedef enum
{
	VEX_IMPORT=0x230,
	VEX_EXPORT,
	ECU_BACKUP,
	ECU_RESTORE
}FioAction;

typedef enum
{
	MTX_SIMPLE_WRITE=0x240,
	MTX_CHUNK_WRITE,
	MTX_CMD_WRITE
}WriteMode;

typedef enum
{
	MTX_CHAR=0x250,
	MTX_U08,
	MTX_S08,
	MTX_U16,
	MTX_S16,
	MTX_U32,
	MTX_S32,
	MTX_UNDEF
}DataSize;

typedef enum
{
	COUNT=0x260,
	NUMMATCH,
	SUBMATCH,
	FULLMATCH
}MatchClass;

typedef enum
{
	LV_PLAYBACK=0x270,
	LV_REALTIME
}Lv_Mode;

typedef enum
{
	LV_GOTO_START=0x280,
	LV_GOTO_END,
	LV_REWIND,
	LV_FAST_FORWARD,
	LV_STOP,
	LV_PLAY
}Lv_Handler;

typedef enum
{
	WRITE_VERIFY=0x290,
	MISMATCH_COUNT,
	MS1_CLOCK,
	MS2_CLOCK,
	NUM_REV,
	TEXT_REV,
	SIGNATURE,
	MS1_VECONST,
	MS2_VECONST,
	MS2_BOOTLOADER,
	MS1_RT_VARS,
	MS2_RT_VARS,
	MS1_GETERROR,
	MS1_E_TRIGMON,
	MS1_E_TOOTHMON,
	MS2_E_TRIGMON,
	MS2_E_TOOTHMON,
	MS2_E_COMPOSITEMON
}XmlCmdType;

typedef enum
{
	DATA=0x2A0,
	ACTION,
	STATIC_STRING
}ArgType;

typedef enum
{
	SLEEP=0x2C0
}Action;

typedef enum
{
	FUNCTIONS=0x2D0,
	SEQUENCE,
	ARGS
}ArrayType;

typedef enum
{
	ADD=0,
	SUBTRACT,
	MULTIPLY,
	DIVIDE
}ScaleOp;

typedef enum
{
	DEP_TYPE=0,
	DEP_SIZE,
	DEP_PAGE,
	DEP_OFFSET,
	DEP_BITMASK,
	DEP_BITSHIFT,
	DEP_BITVAL
}DepVector;

typedef enum
{
	HELP = 0x3F0,
	QUIT,
	GET_SIGNATURE,
	GET_RT_VARS,
	GET_RTV_LIST,
	GET_ECU_VARS,
	GET_ECU_VAR_U08,
	GET_ECU_VAR_S08,
	GET_ECU_VAR_U16,
	GET_ECU_VAR_S16,
	GET_ECU_VAR_U32,
	GET_ECU_VAR_S32,
	SET_ECU_VAR_U08,
	SET_ECU_VAR_S08,
	SET_ECU_VAR_U16,
	SET_ECU_VAR_S16,
	SET_ECU_VAR_U32,
	SET_ECU_VAR_S32,
	BURN_FLASH,
	GET_RAW_ECU,
	SET_RAW_ECU

}TcpCommand;

typedef enum
{
	MTX_ASCII = 0x410,
	MTX_BINARY
}SocketMode;

typedef enum
{
	_X_ = 0x420,
	_Y_,
	_Z_
}Axis;

typedef enum
{
        CLT=0x430,
        IAT
}SensorType;


#endif
