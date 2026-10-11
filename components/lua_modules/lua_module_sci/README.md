# Lua DFRobot SCI

DFRobot SCI sensor-board access is currently unavailable through this module. When the module is enabled, `require("sci")` succeeds, but `sci.new(...)` always raises a Lua error and does not return a usable device.

Changing constructor arguments does not enable hardware access.

## Check availability

```lua
local sci = require("sci")
local ok, err = pcall(sci.new)
if not ok then
    print(err)
end
```
