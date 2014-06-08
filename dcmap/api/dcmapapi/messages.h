
//////////////////////////////////////////////////////////////////////////
// standert message results

#define	DCMM_SUCCESS   (1<<0)
#define	DCMM_FAIL      (1<<1)
#define	DCMM_PROCESSED (1<<2)
#define	DCMM_OK        DCMM_SUCCESS

//////////////////////////////////////////////////////////////////////////
#define	DCMM_USER           0x00ff0000  // User message area


#define	DCMM_PLANET_CHANGED           1  // When current planet changed
// wParam=id, lParam=age

#define	DCMM_DATA_CHANGED             2  // when data changed

#define	DCMM_MAPDATA_CHANGED          3  // When Processed data changed

#define	DCMM_PLANET_MAPDATA_CHANGED   4  // When Processed data for single planet changed 
// wParam=x, lParam=y

#define	DCMM_SELECTION_DATA_CHANGED   5  // When selected database item changed
// wParam=id, lParam=age

#define	DCMM_PLANET_FILTER_CHANGED   6  // When selected database item changed
// wParam=id, lParam=age
#define	DCMM_POST_INIT               7  // After general init

#define	DCMM_POINT_PLANET            8  // When mouse points new planet
// wparam:x,lparam:y     

#define	DCMM_WORKSPACE_ACTIVATE      9  // When workspace activated, no params
#define	DCMM_WORKSPACE_DEACTIVATE    10 // When workspace deactivated, no params

#define	DCMM_CURRENT_RACE_CHANGED    11 // When current race changed - used for geo value calculations

#define	DCMM_CONFIG_CHANGED           12  // when config changed

#define	DCMM_LAYERS_CHANGED           13  // when layers changed

#define	DCMM_LAYERS_VISIBLITY_CHANGED 14  // when layers showed & hided

#define	DCMM_FILTERUI_SET_FILTER      15  // sender - filter module

#define	DCMM_PRE_DESTROY			  16  // all modules alive - chance to free up all allocated modules

#define	DCMM_EXECUTE				  17  // wparam = command number, lparam - dcmapWCSTR: command string, can be NULL

#define	DCMM_SHOW_CELL                18  // When map cell need to be shown

#define	DCMM_SKIN_CHANGED             19  // When map cell need to be shown

#define	DCMM_CHECK_WEB_UPDATES        20  // Modules can check for web updates on this message

#define	DCMM_WORKSPACE_SHOW           21  // When workspace activated, no params
#define	DCMM_WORKSPACE_HIDE           22  // When workspace deactivated, no params

#define	DCMM_SYSTEM_TIMER_TICK        23 // On timer tick (per 15 sec) we can update something
										 // wParam= tick count, lParam = tick interval

#define	DCMM_DATA_PREPROCESS          24  // when data changed
#define	DCMM_DATA_PROCESS             25  // when data changed
#define	DCMM_DATA_POSTPROCESS         26  // when data changed

#define	DCMM_FLEET_SELECT             27  // when data changed

// UI messages
#define	DCMM_UI_MESSAGES              0x100  

#define	DCMM_UI_COMMAND               (DCMM_UI_MESSAGES+1)  // wParam= control id, lParam = IDCMapVarValue* - param
#define	DCMM_UI_TEST                  (DCMM_UI_MESSAGES+2)  // wParam= control id, lParam = IDCMapVarValue* - param

#define	DCMM_UI_COMMAND_STR_PROC      (DCMM_UI_MESSAGES+3)  // wParam= control id, lParam = dcmapWCSTR - string param
#define	DCMM_UI_COMMAND_STR_FUNC      (DCMM_UI_MESSAGES+4)  // wParam= control id, lParam = dcmapWCSTR - string param, return = dcmapWCSTR

#define	DCMM_UI_SET_TEXT        (DCMM_UI_MESSAGES+0x10+1)  /// (dcmapWCSTR)wparam: Text
#define	DCMM_UI_GET_TEXT        (DCMM_UI_MESSAGES+0x10+2)  /// (dcmapWCSTR)Return: Text

#define	DCMM_MAP_MESSAGES             0x200  

// Map messages all params in cells (not in pixels)

#define	DCMM_MAP_CLICK                (DCMM_MAP_MESSAGES+2)  // wparam: x,lparam: y
#define	DCMM_MAP_DCLICK               (DCMM_MAP_MESSAGES+3)  // wparam: x,lparam: y
#define	DCMM_MAP_SCALE_CHANGED        (DCMM_MAP_MESSAGES+4)  // wparam: cell size
#define	DCMM_MAP_PAN                  (DCMM_MAP_MESSAGES+5)  // wparam: left,lparam: top
#define	DCMM_MAP_RESIZE               (DCMM_MAP_MESSAGES+6)  // wparam: width,lparam: height
#define	DCMM_MAP_VIEW_RESIZE          (DCMM_MAP_MESSAGES+7)  // wparam: width,lparam: height

#define	DCMM_MAP_LAYER_ACTIVATE       (DCMM_MAP_MESSAGES+7)  //
#define	DCMM_MAP_LAYER_DEACTIVATE     (DCMM_MAP_MESSAGES+8)  //

#define	DCMM_MAP_LAYER_SHOW           (DCMM_MAP_MESSAGES+9)  //
#define	DCMM_MAP_LAYER_HIDE           (DCMM_MAP_MESSAGES+10)  //

#define	DCMM_MAP_KEYDOWN			  (DCMM_MAP_MESSAGES+11) // analog to windows WM_KEYDOWN message

#define	DCMM_MAP_RCLICK               (DCMM_MAP_MESSAGES+12)  // wparam: x,lparam: y

#define	DCMM_MAP_SET_FILTER_SHADE_MODE (DCMM_MAP_MESSAGES+13)  // wparam: mode

// Map messages all params in cells (not in pixels)

#define	DCMM_GRAPHICS_MESSAGES        0x300  

#define	DCMM_GRAPHICS_DRAW_BEGIN      (DCMM_GRAPHICS_MESSAGES+1)  
#define	DCMM_GRAPHICS_DRAW_END        (DCMM_GRAPHICS_MESSAGES+2)  
#define	DCMM_GRAPHICS_POINT_PLACED    (DCMM_GRAPHICS_MESSAGES+3) // wparam: x,y
#define	DCMM_GRAPHICS_TEXT_PLACED     (DCMM_GRAPHICS_MESSAGES+4) // wparam: x,y lparam: dcmapWCSTR - text 
