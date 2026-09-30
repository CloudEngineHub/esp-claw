# ESP-Claw Lua LVGL Web Simulator

This directory contains the Emscripten build for the ESP-Claw Lua LVGL
simulator. It intentionally keeps only simulator-specific source files in the
repository.

模拟器支持 Lua LVGL 和 `require("display")` 的基础屏幕 API。Web display 复用固件绘图核心，支持 RGB565、单/双缓冲、图元、基础字体、裁剪、平移和单指触摸；图像与自定义字体加载明确报不支持。RAW 和 LVGL 共用排他屏幕会话。

Lua 正常返回、出错或停止后立即清理运行状态。RAF 使用 JS 持有的像素副本，切换 App 或分辨率会丢弃旧刷新。浏览器音频仍未提供当前固件的 `audio.open_output()`，可选音效 App 会降级为无声。

`sim/lua_display_sim.c` 的参数校验和返回值应与原生 `lua_module_display.c` 保持一致。修改绑定后，在浏览器上传执行 `tests/display_contract.lua`，检查 `DISPLAY_CONTRACT_OK` 和实际画面；重复打开屏幕的错误日志属于测试预期。`esp_claw_sim_heap_used` 导出供重复运行后的活跃堆检查使用。

Third-party dependencies are fetched into the CMake build directory:

- LVGL `v9.5.0`
- Lua `5.4.8`

## Build

From an Emscripten environment:

```bash
emcmake cmake -S tools/lua_lvgl_web_sim -B build/lua_lvgl_web_sim -G Ninja -DCMAKE_BUILD_TYPE=Release
embuilder build sdl2
emmake ninja -C build/lua_lvgl_web_sim esp_claw_sim esp_claw_sim_release
```

The normal simulator page uses `esp_claw_sim.html`, `esp_claw_sim.js`,
`esp_claw_sim.wasm`, and `esp_claw_sim.data`. The release target produces a
single-file HTML build.

## Runtime Assets

The simulator-specific runtime files live in:

- `sim/` for the C/JS runtime bridge and Lua shims
- `sim/compat/` for ESP/Lua compatibility headers
- `sim_assets/` for preloaded simulator assets such as fonts
- `tools/` for local helper scripts such as the stream proxy
