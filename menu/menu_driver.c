/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2015 - Daniel De Matteis
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

#include <string.h>
#include <string/string_list.h>

#include "menu_driver.h"
#include "menu.h"
#include "menu_cbs.h"
#include "menu_displaylist.h"
#include "menu_video.h"
#include "../driver.h"
#include "../general.h"

static bool menu_alive = false;

static const menu_ctx_driver_t *menu_ctx_drivers[] = {
#if defined(HAVE_RMENU)
   &menu_ctx_rmenu,
#endif
#if defined(HAVE_RMENU_XUI)
   &menu_ctx_rmenu_xui,
#endif
#if defined(HAVE_GLUI)
   &menu_ctx_glui,
#endif
#if defined(HAVE_XMB)
   &menu_ctx_xmb,
#endif
#if defined(HAVE_RGUI)
   &menu_ctx_rgui,
#endif
   &menu_ctx_null,
   NULL
};

/**
 * menu_driver_find_handle:
 * @idx              : index of driver to get handle to.
 *
 * Returns: handle to menu driver at index. Can be NULL
 * if nothing found.
 **/
const void *menu_driver_find_handle(int idx)
{
   const void *drv = menu_ctx_drivers[idx];
   if (!drv)
      return NULL;
   return drv;
}

/**
 * menu_driver_find_ident:
 * @idx              : index of driver to get handle to.
 *
 * Returns: Human-readable identifier of menu driver at index. Can be NULL
 * if nothing found.
 **/
const char *menu_driver_find_ident(int idx)
{
   const menu_ctx_driver_t *drv = menu_ctx_drivers[idx];
   if (!drv)
      return NULL;
   return drv->ident;
}

/**
 * config_get_menu_driver_options:
 *
 * Get an enumerated list of all menu driver names,
 * separated by '|'.
 *
 * Returns: string listing of all menu driver names,
 * separated by '|'.
 **/
const char* config_get_menu_driver_options(void)
{
   union string_list_elem_attr attr;
   unsigned i;
   char *options = NULL;
   int options_len = 0;
   struct string_list *options_l = string_list_new();

   attr.i = 0;

   if (!options_l)
      return NULL;

   for (i = 0; menu_driver_find_handle(i); i++)
   {
      const char *opt = menu_driver_find_ident(i);
      options_len += strlen(opt) + 1;
      string_list_append(options_l, opt, attr);
   }

   options = (char*)calloc(options_len, sizeof(char));

   if (!options)
   {
      string_list_free(options_l);
      options_l = NULL;
      return NULL;
   }

   string_list_join_concat(options, options_len, options_l, "|");

   string_list_free(options_l);
   options_l = NULL;

   return options;
}

void find_menu_driver(void)
{
   driver_t *driver     = driver_get_ptr();
   settings_t *settings = config_get_ptr();

   int i = find_driver_index("menu_driver", settings->menu.driver);
   if (i >= 0)
      driver->menu_ctx = (const menu_ctx_driver_t*)menu_driver_find_handle(i);
   else
   {
      unsigned d;
      RARCH_WARN("Couldn't find any menu driver named \"%s\"\n",
            settings->menu.driver);
      RARCH_LOG_OUTPUT("Available menu drivers are:\n");
      for (d = 0; menu_driver_find_handle(d); d++)
         RARCH_LOG_OUTPUT("\t%s\n", menu_driver_find_ident(d));
      RARCH_WARN("Going to default to first menu driver...\n");

      driver->menu_ctx = (const menu_ctx_driver_t*)menu_driver_find_handle(0);

      if (!driver->menu_ctx)
         rarch_fail(1, "find_menu_driver()");
   }
}

static void init_menu_fallback(void)
{
#ifdef HAVE_RGUI
   settings_t *settings = config_get_ptr();
   driver_t *driver     = driver_get_ptr();
   int i = find_driver_index("menu_driver", "rgui");

   if (i >= 0)
   {
      driver->menu_ctx = (const menu_ctx_driver_t*)menu_driver_find_handle(i);
      if (settings)
         strlcpy(settings->menu.driver, "rgui", sizeof(settings->menu.driver));
   }
#endif
}

void init_menu(void)
{
   const char *video_driver;
   driver_t *driver     = driver_get_ptr();

   if (driver->menu)
      return;

   find_menu_driver();

   video_driver = menu_video_get_ident();

   switch (driver->menu_ctx->type)
   {
      case MENU_VIDEO_DRIVER_GENERIC:
         break;
      case MENU_VIDEO_DRIVER_DIRECT3D:
         if (video_driver && (strcmp(video_driver, "d3d") != 0))
            init_menu_fallback();
         break;
      case MENU_VIDEO_DRIVER_OPENGL:
         if (video_driver && (strcmp(video_driver, "gl") != 0))
            init_menu_fallback();
         break;
   }

   if (!(driver->menu = (menu_handle_t*)menu_init(driver->menu_ctx)))
      rarch_fail(1, "init_menu()");

   if (!(menu_displaylist_init(driver->menu)))
      rarch_fail(1, "init_menu()");
}

menu_handle_t *menu_driver_get_ptr(void)
{
   driver_t *driver = driver_get_ptr();
   if (!driver || !driver->menu)
      return NULL;
   return driver->menu;
}

const menu_ctx_driver_t *menu_ctx_driver_get_ptr(void)
{
   driver_t *driver = driver_get_ptr();
   if (!driver || !driver->menu_ctx)
      return NULL;
   return driver->menu_ctx;
}

void  menu_driver_list_free(file_list_t *list, size_t idx, size_t list_size)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->list_free)
      driver->list_free(list, idx, list_size);

   file_list_free_userdata  (list, idx);
   file_list_free_actiondata(list, idx);
}

void  menu_driver_list_clear(file_list_t *list)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();
   unsigned i;

   if (driver->list_clear)
      driver->list_clear(list);

   for (i = 0; i < list->size; i++)
      file_list_free_actiondata(list, i);
}

void  menu_driver_context_destroy(void)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver && driver->context_destroy)
      driver->context_destroy();
}

void  menu_driver_list_set_selection(file_list_t *list)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver && driver->list_set_selection)
      driver->list_set_selection(list);
}

size_t  menu_driver_list_get_selection(void)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();
   menu_handle_t *menu             = menu_driver_get_ptr();

   if (driver && driver->list_get_selection)
      return driver->list_get_selection(menu);
   return 0;
}

void menu_driver_list_cache(menu_list_type_t type, unsigned action)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->list_cache)
      driver->list_cache(type, action);
}

size_t menu_driver_list_get_size(menu_list_type_t type)
{
   menu_handle_t *menu             = menu_driver_get_ptr();
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver && driver->list_get_size)
      return driver->list_get_size(menu, type);
   return 0;
}

void *menu_driver_list_get_entry(menu_list_type_t type, unsigned i)
{
   menu_handle_t *menu             = menu_driver_get_ptr();
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver && driver->list_get_entry)
      return driver->list_get_entry(menu, type, i);
   return NULL;
}

void menu_driver_set_texture(void)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->set_texture)
      driver->set_texture();
}

void menu_driver_context_reset(void)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->context_reset)
      driver->context_reset();
}

void menu_driver_frame(void)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->frame)
      driver->frame();
}

int menu_driver_bind_init(menu_file_list_cbs_t *cbs,
      const char *path, const char *label, unsigned type, size_t idx,
      const char *elem0, const char *elem1,
      uint32_t label_hash, uint32_t menu_label_hash)
{
   int ret = 0;
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver && driver->bind_init)
      ret = driver->bind_init(cbs, path, label, type, idx, elem0, elem1,
            label_hash, menu_label_hash);

   return ret;
}

void menu_driver_free(menu_handle_t *menu)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->free)
      driver->free(menu);
}

void menu_driver_toggle(bool latch)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->toggle)
      driver->toggle(latch);
}

bool menu_driver_load_image(void *data, menu_image_type_t type)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->load_image)
      return driver->load_image(data, type);

   return false;
}

bool menu_driver_alive(void)
{
   return menu_alive;
}

void menu_driver_set_alive(void)
{
   menu_alive = true;
}

void menu_driver_unset_alive(void)
{
   menu_alive = false;
}

bool menu_environment_cb(menu_environ_cb_t type, void *data)
{
   const menu_ctx_driver_t *driver = menu_ctx_driver_get_ptr();

   if (driver->environ_cb)
   {
      int ret = driver->environ_cb(type, data);
      if (ret == 0)
         return true;
   }

   return false;
}
