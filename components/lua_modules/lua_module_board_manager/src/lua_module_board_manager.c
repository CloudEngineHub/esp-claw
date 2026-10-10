/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "lua_module_board_manager.h"

#include <string.h>
#include "esp_err.h"
#include "lauxlib.h"
#include "esp_board_manager_includes.h"
#include "cap_lua.h"

#define LUA_BM_DISPLAY_PANEL_IF_IO        0
#define LUA_BM_DISPLAY_PANEL_IF_RGB       1
#define LUA_BM_DISPLAY_PANEL_IF_MIPI_DSI  2
#define LUA_BM_DISPLAY_PIXEL_FORMAT_RGB565 0
#define LUA_BM_DISPLAY_PIXEL_FORMAT_RGB888 1

static int lua_bm_unsupported(lua_State *L)
{
    return luaL_error(L, "raw board device access is not supported; use audio/display services or a managed device module");
}

static int push_err(lua_State *L, esp_err_t err, const char *msg)
{
    lua_pushnil(L);
    if (msg) {
        lua_pushfstring(L, "%s: %s", msg, esp_err_to_name(err));
    } else {
        lua_pushstring(L, esp_err_to_name(err));
    }
    return 2;
}

static esp_err_t lua_module_board_manager_get_camera_paths(const char **dev_path,
                                                           const char **meta_path)
{
#if CONFIG_ESP_BOARD_DEV_CAMERA_SUPPORT
    dev_camera_handle_t *camera_handle = NULL;
    esp_err_t err;

    if (dev_path == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    *dev_path = NULL;
    if (meta_path != NULL) {
        *meta_path = NULL;
    }

    err = esp_board_manager_get_device_handle(ESP_BOARD_DEVICE_NAME_CAMERA,
                                                     (void **)&camera_handle);
    if (err != ESP_OK) {
        return err;
    }
    if (camera_handle == NULL || camera_handle->dev_path == NULL) {
        return ESP_ERR_NOT_FOUND;
    }

    *dev_path = camera_handle->dev_path;
    if (meta_path != NULL) {
        *meta_path = camera_handle->meta_path;
    }
    return ESP_OK;
#else
    if (dev_path != NULL) {
        *dev_path = NULL;
    }
    if (meta_path != NULL) {
        *meta_path = NULL;
    }
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

static int lua_bm_get_board_info(lua_State *L)
{
    esp_board_info_t info = {0};
    esp_err_t err = esp_board_manager_get_board_info(&info);
    if (err != ESP_OK) {
        return push_err(L, err, "get_board_info");
    }
    lua_newtable(L);
    lua_pushstring(L, info.name         ? info.name         : "");
    lua_setfield(L, -2, "name");
    lua_pushstring(L, info.chip         ? info.chip         : "");
    lua_setfield(L, -2, "chip");
    lua_pushstring(L, info.version      ? info.version      : "");
    lua_setfield(L, -2, "version");
    lua_pushstring(L, info.description  ? info.description  : "");
    lua_setfield(L, -2, "description");
    lua_pushstring(L, info.manufacturer ? info.manufacturer : "");
    lua_setfield(L, -2, "manufacturer");
    return 1;
}

static int lua_bm_get_device_config_handle(lua_State *L)
{
    const char *name = luaL_checkstring(L, 1);
    void *config = NULL;
    esp_err_t err = esp_board_manager_get_device_config(name, &config);
    if (err != ESP_OK) {
        return push_err(L, err, name);
    }
    if (config == NULL) {
        lua_pushnil(L);
        lua_pushstring(L, "device config is NULL");
        return 2;
    }
    lua_pushlightuserdata(L, config);
    return 1;
}

static int lua_bm_get_camera_paths(lua_State *L)
{
    const char *dev_path = NULL;
    const char *meta_path = NULL;
    esp_err_t err = lua_module_board_manager_get_camera_paths(&dev_path, &meta_path);

    if (err != ESP_OK) {
        return push_err(L, err, "get_camera_paths");
    }

    lua_newtable(L);
    lua_pushstring(L, dev_path);
    lua_setfield(L, -2, "dev_path");
    if (meta_path != NULL) {
        lua_pushstring(L, meta_path);
        lua_setfield(L, -2, "meta_path");
    }
    return 1;
}

int luaopen_board_manager(lua_State *L)
{
    static const luaL_Reg funcs[] = {
        {"get_board_info",    lua_bm_get_board_info},
        {"init_device",       lua_bm_unsupported},
        {"deinit_device",     lua_bm_unsupported},
        {"get_device_handle", lua_bm_unsupported},
        {"get_device_config_handle", lua_bm_get_device_config_handle},
        {"get_display_lcd_params", lua_bm_unsupported},
        {"get_audio_codec_input_params", lua_bm_unsupported},
        {"get_audio_codec_output_params", lua_bm_unsupported},
        {"get_camera_paths",  lua_bm_get_camera_paths},
        {NULL, NULL},
    };

    lua_newtable(L);
    luaL_setfuncs(L, funcs, 0);
    lua_pushinteger(L, LUA_BM_DISPLAY_PANEL_IF_IO);
    lua_setfield(L, -2, "PANEL_IF_IO");
    lua_pushinteger(L, LUA_BM_DISPLAY_PANEL_IF_RGB);
    lua_setfield(L, -2, "PANEL_IF_RGB");
    lua_pushinteger(L, LUA_BM_DISPLAY_PANEL_IF_MIPI_DSI);
    lua_setfield(L, -2, "PANEL_IF_MIPI_DSI");
    lua_pushinteger(L, LUA_BM_DISPLAY_PIXEL_FORMAT_RGB565);
    lua_setfield(L, -2, "PIXEL_FORMAT_RGB565");
    lua_pushinteger(L, LUA_BM_DISPLAY_PIXEL_FORMAT_RGB888);
    lua_setfield(L, -2, "PIXEL_FORMAT_RGB888");
    return 1;
}

esp_err_t lua_module_board_manager_register(void)
{
    return cap_lua_register_module("board_manager", luaopen_board_manager);
}
