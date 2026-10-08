# Lua GPIO

This module describes how to correctly use gpio when writing Lua scripts.

## How to call
- Import it with `local gpio = require("gpio")`
- Call `gpio.set_direction(pin, mode)` to set pin mode
- Call `gpio.set_level(pin, level)` to set output level
- Call `gpio.get_level(pin)` to read pin level
- Call `gpio.close(pin)` to reset the pin and release its claim; repeated close is safe

Supported modes are `"input"`, `"output"`, `"input_output"`, `"output_od"`, `"input_output_od"`, and `"disable"`.

`set_direction` and `set_level` return no values on success and raise a Lua error on failure. `get_level` returns `0` or `1`.

The first operation claims the pin for the current Lua environment. Pins held by an initialized board driver or another hardware object are rejected before configuration. The environment keeps a strong reference to each claimed pin: ordinary garbage collection does not release it. Call `gpio.close(pin)` explicitly or let job-exit cleanup close it. A failed hardware reset retains the claim for retry.

The example uses GPIO4, which is not reserved by the ACM2 board manifest; verify availability on other boards.

## Example
```lua
local gpio = require("gpio")
gpio.set_direction(4, "output")
gpio.set_level(4, 1)
gpio.close(4)
```
