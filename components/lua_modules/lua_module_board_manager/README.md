# Lua Board Manager

`board_manager` provides board metadata and camera paths.

## API

- `board_manager.get_board_info()` returns `{ name, chip, version, description, manufacturer }`.
- `board_manager.get_camera_paths()` returns `{ dev_path, meta_path }`; `meta_path` may be absent.
- `board_manager.get_device_config_handle(name)` returns an opaque configuration handle. It cannot be inspected as a Lua table or used to open a device.

These calls return `nil, err` when the requested information is unavailable. Invalid arguments raise Lua errors.

## Device access

`init_device`, `deinit_device`, `get_device_handle`, `get_display_lcd_params`, `get_audio_codec_input_params`, and `get_audio_codec_output_params` currently raise an unsupported-operation error. Use `display`, `audio`, `camera`, or the appropriate sensor module to access hardware.

The module also exports `PANEL_IF_IO`, `PANEL_IF_RGB`, `PANEL_IF_MIPI_DSI`, `PIXEL_FORMAT_RGB565`, and `PIXEL_FORMAT_RGB888`; these constants do not enable device access.

## Example

```lua
local board_manager = require("board_manager")

local info = assert(board_manager.get_board_info())
print(info.name, info.chip, info.version)

local paths, err = board_manager.get_camera_paths()
if paths then
    print(paths.dev_path)
else
    print(err)
end
```
