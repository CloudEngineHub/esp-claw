# Lua Environmental Sensor

Environmental sensor access is currently unavailable through this module. When the module is enabled, `require("environmental_sensor")` succeeds, but `environmental_sensor.new(...)` always raises a Lua error and does not return a usable device.

Changing constructor arguments does not enable hardware access.

## Check availability

```lua
local environmental_sensor = require("environmental_sensor")
local ok, err = pcall(environmental_sensor.new)
if not ok then
    print(err)
end
```
