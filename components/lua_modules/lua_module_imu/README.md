# Lua IMU

This module describes how to read IMU data from Lua.
When a request mentions `bmi270`, `icm42670`, `mpu6050`, `imu`, `accelerometer`, or `gyroscope`, use this module by default.

## How to call
- Import it with `local imu = require("imu")`
- Open the sensor with one of:
  - `local sensor = imu.new()` — use board defaults from the device named `imu_sensor`
  - `local sensor = imu.new("imu_sensor")` — choose a device name explicitly
  - `local sensor = imu.new({ device = "imu_sensor" })` — select the board device through an options table
- Call `local sample = sensor:read()` to get accel and gyro raw data
- Call `local temp = sensor:read_temperature()` to read the raw temperature value
- Call `local status = sensor:read_int_status()` to get the interrupt status bits
- Call `sensor:name()` to read the resolved board device name
- Call `sensor:close()` when needed

The board must publish its initialized I2C bus. Each sensor instance atomically claims its I2C address and configured INT/SDO GPIOs, together with a shared borrow of that bus. `custom`/`init_skip` configuration alone does not reserve these resources. Two instances of the same IMU, or a raw GPIO/I2C consumer using the same resources, conflict before hardware initialization. Use `sensor:close()` for prompt release; Lua GC and job exit also finalize handles. Retry a failed explicit close before dropping the handle.

## Options table

Only `device` is accepted, defaulting to `"imu_sensor"`. Bus selection, I2C address, clock, interrupt pin, and address-select pin come from the board configuration. Lua hardware overrides are rejected; update the board configuration when wiring changes.

## Data format
- `sample.accel.x`, `sample.accel.y`, `sample.accel.z`
- `sample.gyro.x`, `sample.gyro.y`, `sample.gyro.z`
- `sample.sens_time`
- `sample.status`

## Example
```lua
local imu = require("imu")

local sensor = imu.new()                  -- uses board defaults
local sample = sensor:read()
print(sample.accel.x, sample.accel.y, sample.accel.z)
print(sample.gyro.x, sample.gyro.y, sample.gyro.z)
sensor:close()
```
