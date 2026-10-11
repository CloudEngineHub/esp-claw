# Lua Magnetometer

Magnetometer access is currently unavailable through this module. When the module is enabled, `require("magnetometer")` succeeds, but `magnetometer.new(...)` always raises a Lua error and does not return a usable device.

Changing constructor arguments does not enable hardware access.

## Check availability

```lua
local magnetometer = require("magnetometer")
local ok, err = pcall(magnetometer.new)
if not ok then
    print(err)
end
```
