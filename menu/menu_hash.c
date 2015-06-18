/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2015 - Daniel De Matteis
 *
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <rhash.h>

#include "menu_hash.h"

const char *menu_hash_to_str(uint32_t hash)
{
   switch (hash)
   {
      case MENU_LABEL_VALUE_DISK_OPTIONS:
         return "Core Disk Options";
      case MENU_LABEL_VALUE_CORE_OPTIONS:
         return "Core Options";
      case MENU_LABEL_VALUE_CORE_CHEAT_OPTIONS:
         return "Core Cheat Options";
      case MENU_LABEL_CORE_CHEAT_OPTIONS:
         return "core_cheat_options";
      case MENU_LABEL_CORE_OPTIONS:
         return "core_options";
      case MENU_LABEL_DATABASE_MANAGER_LIST:
         return "database_manager_list";
      case MENU_LABEL_CURSOR_MANAGER_LIST:
         return "cursor_manager_list";
      case MENU_LABEL_FRONTEND_COUNTERS:
         return "frontend_counters";
      case MENU_LABEL_CORE_COUNTERS:
         return "core_counters";
      case MENU_LABEL_DISK_CYCLE_TRAY_STATUS:
         return "disk_cycle_tray_status";
      case MENU_LABEL_DISK_IMAGE_APPEND:
         return "disk_image_append";
      case MENU_LABEL_DEFERRED_CORE_LIST:
         return "deferred_core_list";
      case MENU_LABEL_DEFERRED_CORE_LIST_SET:
         return "deferred_core_list_set";
      case MENU_LABEL_VALUE_OPTIONS:
         return "Options";
      case MENU_LABEL_VALUE_RESUME:
         return "Resume";
      case MENU_LABEL_VALUE_SAVE_STATE:
         return "Save State";
      case MENU_LABEL_VALUE_DISK_INDEX:
         return "Disk Index";
      case MENU_LABEL_VALUE_FRONTEND_COUNTERS:
         return "Frontend Counters";
      case MENU_LABEL_VALUE_CORE_COUNTERS:
         return "Core Counters";
      case MENU_LABEL_VALUE_DISK_IMAGE_APPEND:
         return "Disk Image Append";
      case MENU_LABEL_VALUE_DISK_CYCLE_TRAY_STATUS:
         return "Disk Cycle Tray Status";
      case MENU_LABEL_VALUE_NO_CORE_INFORMATION_AVAILABLE:
         return "No core information available.";
      case MENU_LABEL_VALUE_NO_CORE_OPTIONS_AVAILABLE:
         return "No core options available.";
      case MENU_LABEL_VALUE_NO_CORES_AVAILABLE:
         return "No cores available.";
      case MENU_LABEL_VALUE_CORE_INFORMATION:
         return "Core Information";
      case MENU_LABEL_VALUE_DATABASE_MANAGER:
         return "Database Manager";
      case MENU_LABEL_VALUE_CURSOR_MANAGER:
         return "Cursor Manager";
      case MENU_VALUE_RECORDING_SETTINGS:
         return "Recording Settings";
      case MENU_VALUE_MAIN_MENU:
         return "Main Menu";
      case MENU_VALUE_SETTINGS:
         return "Settings";
      case MENU_LABEL_QUIT_RETROARCH:
         return "Quit RetroArch";
      case MENU_LABEL_HELP:
         return "Help";
      case MENU_LABEL_SAVE_NEW_CONFIG:
         return "Save New Config";
      case MENU_LABEL_RESTART_CONTENT:
         return "Restart Content";
      case MENU_LABEL_TAKE_SCREENSHOT:
         return "Take Screenshot";
      case MENU_LABEL_CORE_UPDATER_LIST:
         return "Core Updater";
      case MENU_LABEL_SYSTEM_INFORMATION:
         return "System Information";
      case MENU_LABEL_OPTIONS:
         return "Options";
      case MENU_LABEL_CORE_INFORMATION:
         return "Core Information";
      case MENU_LABEL_CORE_LIST:
         return "Load Core";
      case MENU_LABEL_UNLOAD_CORE:
         return "Unload Core";
      case MENU_LABEL_MANAGEMENT:
         return "Advanced Management";
      case MENU_LABEL_PERFORMANCE_COUNTERS:
         return "Performance Counters";
      case MENU_LABEL_SAVE_STATE:
         return "Save State";
      case MENU_LABEL_LOAD_STATE:
         return "Load State";
      case MENU_LABEL_RESUME_CONTENT:
         return "Resume Content";
      case MENU_LABEL_DRIVER_SETTINGS:
         return "Driver Settings";
      case MENU_LABEL_OVERLAY_SCALE:
         return "Overlay Scale";
      case MENU_LABEL_OVERLAY_PRESET:
         return "Overlay Preset";
      case MENU_LABEL_KEYBOARD_OVERLAY_PRESET:
         return "Keyboard Overlay Preset";
      case MENU_LABEL_OVERLAY_OPACITY:
         return "Overlay Opacity";
      case MENU_LABEL_VALUE_MENU_WALLPAPER:
         return "Menu Wallpaper";
      case MENU_LABEL_VALUE_DYNAMIC_WALLPAPER:
         return "Dynamic Wallpaper";
      case MENU_LABEL_VALUE_BOXART:
         return "Display Boxart";
      case MENU_LABEL_VALUE_CORE_INPUT_REMAPPING_OPTIONS:
         return "Core Input Remapping Options";
      case MENU_LABEL_VIDEO_OPTIONS:
         return "video_options";
      case MENU_LABEL_VALUE_VIDEO_OPTIONS:
         return "Video Options";
      case MENU_LABEL_VALUE_SHADER_OPTIONS:
         return "Shader Options";
      case MENU_LABEL_SHADER_OPTIONS:
         return "shader_options";
      case MENU_LABEL_VALUE_NO_SHADER_PARAMETERS:
         return "No shader parameters.";
      default:
         break;
   }

   return "null";
}

uint32_t menu_hash_calculate(const char *s)
{
   return djb2_calculate(s);
}