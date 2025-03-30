> AUTOMATICALLY GENERATED FROM FIRMWARE - DO NOT EDIT
{.is-danger}

> This documentation is current as of Firmware **V1.13.13**
{.is-warning}

## INITIALIZE FAMILY
<br>

### Initialize Scope
This puts the Arduino in Serial Control Mode and displays RA on line 1 and DEC on line 2 of the display. Serial Control Mode can be ended manually by pressing the SELECT key, or programmatically with the :Qq# command. 

**Command:**
`:I#`

**Returns:**
- nothing

<br>

## SYNC CONTROL FAMILY
<br>

### Synchronize Declination and Right Ascension.
This tells the scope what it is currently pointing at. The scope synchronizes to the current target coordinates 

**Command:**
`:CM#`

**Returns:**
- `NONE#`

**Remarks:**
Set with `:Sd#` and `:Sr#`

<br>

## DISTANCE FAMILY
<br>

### Query Mount Status
This queries the mount for its slewing status 

**Command:**
`:D#`

**Returns:**
- `|#` if slewing
- ` #` if not

<br>

## GPS FAMILY
<br>

### Set Mount Time
Attempts to set the mount time and location from the GPS for 2 minutes. This is essentially a blocking call, no other activities take place (except tracking, but only if interrupt-driven). 

**Command:**
`:gT#`

**Returns:**
- `1` if the data was set
- `0` if not (timedout)

**Remarks:**
Use `:Gt#` and `:Gg#` to retrieve Lat and Long,

<br>

### Set Mount Time w/ timeout
Attempts to set the mount time and location from the GPS with a custom timeout. This is also blocking but by using a low timeout, you can avoid long pauses and let the user know that it's not ready yet. 

**Command:**
`:gTnnn#`

**Returns:**
- `1` if the data was set
- `0` if not (timedout)

**Parameters:**
- `nnn` is an integer defining the number of milliseconds to wait for the GPS to get a bearing

<br>

## GET FAMILY
<br>

### Get the Product Name
**Command:**
`:GVP#`

**Returns:**
- `OpenAstroTracker#` if the firmware was compiled for OAT
- `OpenAstroMount#` if the firmware was compiled for OAM

<br>

### Get the Firmware Version Number
**Command:**
`:GVN#`

**Returns:**
- `V1.major.minor#` from version.h

<br>

### Get Target Declination
**Command:**
`:Gd#`

**Returns:**
- `sDD*MM'SS#`

**Parameters:**
- `s` is + or -
- `DD` is degrees
- `MM` is minutes
- `SS` is seconds

<br>

### Get Current Declination
**Command:**
`:GD#`

**Returns:**
- `sDD*MM'SS#`

**Parameters:**
- `s` is + or -
- `DD` is degrees
- `MM` is minutes
- `SS` is seconds.

<br>

### Get Target Right Ascension
**Command:**
`:Gr#`

**Returns:**
- HH:MM:SS#

**Parameters:**
- `HH` is hour
- `MM` is minutes
- `SS` is seconds

<br>

### Get Current Right Ascension
**Command:**
`:GR#`

**Returns:**
- `HH:MM:SS#`

**Parameters:**
- `HH` is hour
- `MM` is minutes
- `SS` is seconds

<br>

### Get Site Latitude
**Command:**
`:Gt#`

**Returns:**
- `sDD*MM#`

**Parameters:**
- `s` is + or -
- `DD` is the latitude in degrees
- `MM` the minutes

<br>

### Get Site Longitude
**Command:**
`:Gg#`

**Returns:**
- `sDDD*MM#`

**Parameters:**
- `s` is the sign of the longitude
- `DDD` is the degrees
- `MM` is the minutes

**Remarks:**
Note that this is the actual longitude, but east coordinates are negative (opposite of normal cartographic coordinates)

<br>

### Get current Clock format
**Command:**
`:Gc#`

**Returns:**
- `24#`

<br>

### Get offset to UTC time
**Command:**
`:GG#`

**Returns:**
- `sHH#`

**Parameters:**
- `s` is the sign
- `HH` is the number of hours

**Remarks:**
Note that this is NOT simply the timezone offset you are in (like -8 for Pacific Standard Time), it is the negative of it. So how many hours need to be added to your local time to get to UTC.

<br>

### Get local time in 12h format
**Command:**
`:Ga#`

**Returns:**
- `HH:MM:SS#`

**Parameters:**
- `HH` are hours (modulo 12)
- `MM` are minutes
- `SS` are seconds of the local time

<br>

### Get local time in 24h format
**Command:**
`:GL#`

**Returns:**
- `HH:MM:SS#`

**Parameters:**
- `HH` are hours
- `MM` are minutes
- `SS` are seconds of the local time

<br>

### Get current date
**Command:**
`:GC#`

**Returns:**
- `MM/DD/YY#`

**Parameters:**
- `MM` is the month (1-12)
- `day` is the day (1-31)
- `year` is the lower two digits of the year

<br>

### Get Site Name 1
**Command:**
`:GM#`

**Returns:**
- `OAT1#`

<br>

### Get Site Name 2
**Command:**
`:GN#`

**Returns:**
- `OAT2#`

<br>

### Get Site Name 3
**Command:**
`:GO#`

**Returns:**
- OAT2#

<br>

### Get Site Name 4
**Command:**
`:GP#`

**Returns:**
- OAT4#

<br>

### Get tracking rate
**Command:**
`:GT#`

**Returns:**
- 60.0#

<br>

## GET EXTENSIONS
<br>

### Get DEC or RA Slewing
**Command:**
`:GIS#`

**Returns:**
- `1#` if either RA or DEC is slewing
- `0#` if not

<br>

### Get Tracking
**Command:**
`:GIT#`

**Returns:**
- `1#` if tracking is on
- `0#` if not

<br>

### Get Guiding
**Command:**
`:GIG#`

**Returns:**
- `1#` if currently guiding
- `0#` if not

<br>

### Get Mount Status
String reflecting the mounts' status. The string is a comma-delimited list of statuses. 

**Command:**
`:GX#`

**Returns:**
- `Idle,--T--,11219,0,927,071906,+900000,,#`

**Parameters:**
- [0] The mount status. One of 'Idle', 'Parked', 'Parking', 'Guiding', 'SlewToTarget', 'FreeSlew', 'ManualSlew', 'Tracking', 'Homing'
- [1] The motion state (see Remarks below).
- [2] The RA stepper position
- [3] The DEC stepper position
- [4] The Tracking stepper position
- [5] The current RA coordinate
- [6] The current DEC coordinate
- [7] The FOC stepper position (if FOC enabled, else empty)

**Remarks:**
The motion state consists of 6 characters. If the character is a '-', the corresponding axis is not moving.
First character is RA slewing state ('R' is East, 'r' is West, '-' is stopped).
Second character is DEC slewing state ('d' is North, 'D' is South, '-' is stopped).
Third character is TRK slewing state ('T' is Tracking, '-' is stopped).
Fourth character is AZ slewing state ('Z' and 'z' is adjusting, '-' is stopped).
Fifth character is ALT slewing state ('A' and 'a' is adjusting, '-' is stopped).
Sixth character is FOC slewing state ('F' and 'f' is adjusting, '-' is stopped).
AZ, ALT, and FOC are only set if the corresponding axis is enabled. If not, the character is always '-'.
Since AZ/ALT rarely move, their positions are not returned here. To get the AZ and ALT stepper positions, use the `:XGAA#` command.

<br>

## SET FAMILY
<br>

### Set Target Declination
This sets the target DEC. Use a Movement command to slew there. 

**Command:**
`:SdsDD*MM:SS#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `s` is + or -
- `DD` is degrees
- `MM` is minutes
- `SS` is seconds

<br>

### Set Right Ascension
This sets the target RA. Use a Movement command to slew there. 

**Command:**
`:SrHH:MM:SS#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `HH` is hours
- `MM` is minutes
- `SS` is seconds

<br>

### Set Site Latitude
This sets the latitude of the location of the mount. 

**Command:**
`:StsDD*MM#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `s` is the sign ('+' or '-')
- `DD` is the degree (90 or less)
- `MM` is minutes

<br>

### Set Site Longitude
This sets the longitude of the location of the mount. 

**Command:**
`:SgsDDD*MM#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `s` (optional) is the sign of the longitude (see Remarks)
- `DDD` is the number of degrees
- `MM` is the minutes

**Remarks:**
When a sign is provided, longitudes are interpreted as given, with zero at Greenwich but negative coordinates going east (opposite of normal cartographic coordinates)
When a sign is not provided, longitudes are from 0 to 360 going WEST with 180 at Greenwich. So 369 is 179W and 1 is 179E. 190 would be 10W and 170 would be 10E.

<br>

### Set Site UTC Offset
This sets the offset of the timezone in which the mount is in hours from UTC. 

**Command:**
`:SGsHH#`

**Returns:**
- `1`

**Parameters:**
- `s` is the sign
- `HH` is the number of hours

<br>

### Set Site Local Time
This sets the local time of the timezone in which the mount is located. 

**Command:**
`:SLHH:MM:SS#`

**Returns:**
- `1`

**Parameters:**
- `HH` is hours
- `MM` is minutes
- `SS` is seconds

<br>

### Set Site Date
This sets the date 

**Command:**
`:SCMM/DD/YY#`

**Returns:**
- `1Updating Planetary Data#                              #`

**Parameters:**
- `MM` is the month
- `DD` is the day
- `YY` is the year since 2000

<br>

## SET Extensions
<br>

### Set HA (Hour Angle of Polaris)
This sets the scopes HA, which should be that of Polaris. 

**Command:**
`:SHHH:MM#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `HH` is hours
- `MM` is minutes

<br>

### Set Home Point
This sets the current orientation of the scope as its home point. 

**Command:**
`:SHP#`

**Returns:**
- `1`

<br>

### Set LST Time
This sets the scopes LST (and HA). 

**Command:**
`:SHLHH:MM#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `HH` is hours
- `MM` is minutes

<br>

### Synchronize Declination and Right Ascension.
This tells the scope what exact coordinates it is currently pointing at. These coordinates become the new current RA/DEC coordinates of the mount. 

**Command:**
`:SYsDD*MM:SS.HH:MM:SS#`

**Returns:**
- `1` if successfully set
- `0` otherwise

**Parameters:**
- `s` is + or -
- `DD` is degrees
- `HH` is hours
- `MM` is minutes
- `SS` is seconds

<br>

## RATE CONTROL FAMILY
<br>

### Set Slew rate
**Command:**
`:Rs#`

**Returns:**
- nothing

**Parameters:**
- `s` is one of 'S', 'M', 'C', or 'G' in order of decreasing speed

<br>

## MOVEMENT FAMILY
<br>

### Start Slew to Target (Asynchronously)
This starts slewing the scope to the target RA and DEC coordinates and returns immediately. 

**Command:**
`:MS#`

**Returns:**
- `0`

<br>

## MOVEMENT EXTENSIONS
<br>

### Run a Guide pulse
This runs the RA or DEC steppers at an increased or decreased speed (in the case of RA) or a constant speed (in the case of DEC) for a short period of time. It is used for guiding. 

**Command:**
`:MGdnnnn#`

**Returns:**
- `1`

**Parameters:**
- `d` is one of 'N', 'E', 'W', or 'S'
- `nnnn` is the duration in ms

<br>

### Set Tracking mode
This turns the scopes tracking mode on or off. 

**Command:**
`:MTs#`

**Returns:**
- `1`

**Parameters:**
- `s` is `1` to turn on Tracking and `0` to turn it off

<br>

### Start slewing
This starts slewing the mount in the given direction. You must issue a stop command (such as the corresponding `:Qc#`, where 'c' is the same direction as passed to this command) or `:Q#` (stops all steppers) to stop it. 

**Command:**
`:Mc#`

**Returns:**
- nothing

**Parameters:**
- `c` is one of 'n', 'e', 'w', or 's'

<br>

### Move stepper
This starts moving one of the steppers by the given amount of steps and returns immediately. Steps can be positive or negative. 

**Command:**
`:MXxnnnnn#`

**Returns:**
- `1` if successfully scheduled, else `0`

**Parameters:**
- `x` is the stepper to move (r for RA, d for DEC, f for FOC, z for AZ, t for ALT)
- `nnnn` is the number of steps

<br>

### Home RA stepper via Hall sensor
This attempts to find the hall sensor and to home the RA ring accordingly. 

**Command:**
`:MHRxn#`

**Returns:**
- `1` if search is started
- `0` if homing has not been enabled in the local configuration file

**Parameters:**
- `x` is either 'R' or 'L' and determines the direction in which the search starts (L is CW, R is CCW).
- `n` (Optional) is the maximum number of degrees to move while searching for the sensor location. Defaults to 30degs. Limited to the range 5degs - 75degs.

**Remarks:**
The ring is first moved 30 degrees (or the given amount) in the initial direction. If no hall sensor is encountered,
it will move twice the amount (60 degrees by default) in the opposite direction.
If a hall sensor is not encountered during that slew, the homing exits with a failure.
If the sensor is found, it will slew to the middle position of the Hall sensor trigger range and then to the offset
specified in the Home offset position (set with the `:XSHRnnnn#` command).
If the RA ring is positioned such that the Hall sensor is already triggered when the command is received, the mount will move
the RA ring off the trigger in the opposite direction specified for a max of 15 degrees before searching 30 degrees in the
specified direction.

<br>

### Home DEC stepper via Hall sensor
This attempts to find the hall sensor and to home the DEC axis accordingly. 

**Command:**
`:MHDxn#`

**Returns:**
- `1` if search is started
- `0` if homing has not been enabled in the local configuration file

**Parameters:**
- `x` is either 'U' or 'D' and determines the direction in which the search starts (U is up, D is down).
- `n` (Optional) is the maximum number of degrees to move while searching for the sensor location. Defaults to 30degs. Limited to the range 5degs - 75degs.

**Remarks:**
The ring is first moved 30 degrees (or the given amount) in the initial direction. If no hall sensor is encountered,
it will move twice the amount (60 degrees by default) in the opposite direction.
If a hall sensor is not encountered during that slew, the homing exits with a failure.
If the sensor is found, it will slew to the middle position of the Hall sensor trigger range and then to the offset
specified in the Home offset position (set with the `:XSHDnnnn#` command).
If the DEC ring is positioned such that the Hall sensor is already triggered when the command is received, the mount will move
the DEC ring off the trigger in the opposite direction specified for a max of 15 degrees before searching 30 degrees in the
specified direction.

<br>

### Move Azimuth and Altitude to home
If the scope supports automated azimuth and altitude operations, move AZ and ALT axis to their zero positions. 

**Command:**
`:MAAH#`

**Returns:**
- `1`

<br>

### Move Azimuth
If the scope supports automated azimuth operation, move azimuth by n.nn arcminutes 

**Command:**
`:MAZn.nn#`

**Returns:**
- nothing

**Parameters:**
- `n.nn` is a signed floating point number representing the number of arcminutes to move the mount left or right

<br>

### Move Altitude
If the scope supports automated altitude operation, move altitude by n.nn arcminutes 

**Command:**
`:MALn.nn#`

**Returns:**
- nothing

**Parameters:**
- `n.nn` is a signed floating point number representing the number of arcminutes to raise or lower the mount.

<br>

## HOME FAMILY
<br>

### Park Scope and stop motors
This slews the scope back to it's home position (RA ring centered, DEC at 90, basically pointing at celestial pole), then advances to the parking position (defined by the Homing offsets) and stops all movement (including tracking). 

**Command:**
`:hP#`

**Returns:**
- nothing

<br>

### Move Scope to Home position
This slews the scope back to its home position (RA ring centered, DEC at 90, basically pointing at celestial pole). Mount will keep tracking. 

**Command:**
`:hF#`

**Returns:**
- nothing

<br>

## HOME/PARK Extensions
<br>

### Unpark Scope
This currently simply turns on tracking. 

**Command:**
`:hU#`

**Returns:**
- `1`

<br>

### Set home position for AZ and ALT axes
If the mount supports AZ and ALT axes, this call sets their positions to 0 and stores this in persistent storage. 

**Command:**
`:hZ#`

**Returns:**
- `1`

<br>

## QUIT MOVEMENT FAMILY
<br>

### Stop all motors
This stops all motors, including tracking. Note that deceleration curves are still followed. 

**Command:**
`:Q#`

**Returns:**
- `1` when all motors have stopped

<br>

### Stop Slewing
Stops slew in specified direction where d is n, s, e, w, a (the first four are the cardinal directions, a stands for All). 

**Command:**
`:Qd#`

**Returns:**
- nothing

<br>

## QUIT MOVEMENT Extensions
<br>

### Disconnect, Quit Control mode
This quits Serial Control mode and starts tracking. 

**Command:**
`:Qq#`

**Returns:**
- nothing

<br>

## EXTRA OAT FAMILY - These are used by the PC control application OATControl
<br>

### Perform a Factory Reset
Clears all the EEPROM settings 

**Command:**
`:XFR#`

**Returns:**
- `1#`

<br>

### Run drift alignment (only supported if SUPPORT_DRIFT_ALIGNMENT is enabled)
This runs a drift alignment procedure where the mounts slews east, pauses, slews west and pauses. Where nnn is the number of seconds the entire alignment should take. The call is blocking and will only return once the drift alignment is complete. 

**Command:**
`:XDnnn#`

**Returns:**
- nothing

<br>

### Turn off the Digital level
**Command:**
`:XL0#`

**Returns:**
- `1#` if successful
- `0#` if there is no Digital Level

<br>

### Turn on the Digital level
**Command:**
`:XL1#`

**Returns:**
- `1#` if successful
- `0#` if there is no Digital Level

<br>

### Digital Level - Get Reference
Gets the reference pitch and roll values of the mount (Digital Level addon). These values are the values of the pitch and roll when the mount is level. 

**Command:**
`:XLGR#`

**Returns:**
- `<pitch>,<roll>#`
- `0#` if there is no Digital Level

<br>

### Digital Level - Get Values
Gets the current pitch and roll values of the mount (Digital Level addon). 

**Command:**
`:XLGC#`

**Returns:**
- `<pitch>,<roll>#`
- `0#` if there is no Digital Level

<br>

### Digital Level - Get Temperature
Get the current temperature in Celsius of the mount (Digital Level addon). 

**Command:**
`:XLGT#`

**Returns:**
- `<temp>#`
- `0#` if there is no Digital Level

<br>

### Digital Level - Set Reference Roll
Sets the reference roll value of the mount (Digital Level addon). This is the value at which the mount is level. 

**Command:**
`:XLSR#`

**Returns:**
- `1#` if successful
- `0#` if there is no Digital Level

<br>

### Digital Level - Set Reference Pitch
Sets the reference pitch value of the mount (Digital Level addon). This is the value at which the mount is level. 

**Command:**
`:XLSP#`

**Returns:**
- `1#` if successful
- `0#` if there is no Digital Level

<br>

### Get position of AZ and ALT axes
Get the current position in steps of the AZ and ALT axes if they are enabled. If an axis is not enabled, this always returns zero as the axis's value. 

**Command:**
`:XGAA#`

**Returns:**
- `azpos|altpos#` if either axis is enabled

<br>

### Get auto homing state
Get the current state of RA and DEC Autohoming status. Only valid when at least one Hall sensor based autohoming axis is enabled. 

**Command:**
`:XGAH#`

**Returns:**
- `rastate|decstate#` if either axis is enabled
- `|#` if no autohoming is enabled

**Remarks:**
While the mount status (:GX#) is 'Homing', the command returns one of these:
MOVE_OFF
MOVING_OFF
STOP_AT_TIME
WAIT_FOR_STOP
START_FIND_START
FINDING_START
FINDING_START_REVERSE
FINDING_END
RANGE_FOUND

<br>

### 
**Command:**
`None`

**Returns:**

<br>

### Get Backlash correction steps
Get the number of steps the RA stepper motor needs to overshoot and backtrack when slewing east. 

**Command:**
`:XGB#`

**Returns:**
- `integer#`

<br>

### Get stepper motor positions for target
Get the positions of stepper motors when pointed at the given coordinates. 

**Command:**
`:XGCn.nn*m.mm#`

**Returns:**
- `ralong,declong#`

**Parameters:**
- `n.nn` is the RA coordinate (0.0 - 23.999)
- `m.mm` is the DEC coordinate (-90.00 - +90.00)
- `ralong` is the stepper position of the RA stepper
- `declong` is the stepper position of the DEC stepper

<br>

### Get RA steps
Get the number of steps the RA stepper motor needs to take to rotate RA by one degree 

**Command:**
`:XGR#`

**Returns:**
- `float#`

<br>

### Get DEC steps
Get the number of steps the DEC stepper motor needs to take to rotate DEC by one degree 

**Command:**
`:XGD#`

**Returns:**
- `float#`

<br>

### Get DEC limits
Get either lower, upper or both limits for the DEC stepper motor in degrees. 

**Command:**
`:XGDLx#`

**Returns:**
- `float#` or `float|float#`

**Parameters:**
- 'x' is optional or can be 'U' or 'L'. If it is 'U' only the upper bound is returned,
- if it is 'L' only the lower bound is returned and if it is missing, both are returned.

<br>

### Get DEC parking position
Gets the number of steps from the home position to the parking position for DEC 

**Command:**
`:XGDP# (obsolete, disabled)`

**Returns:**
- `0#`

<br>

### Get Tracking speed adjustment
Get the adjustment factor used to speed up (>1.0) or slow down (<1.0) the tracking speed of the mount. 

**Command:**
`:XGS#`

**Returns:**
- `float#`

<br>

### Get Remaining Safe Time
Get the number of hours before the RA ring reaches its end. 

**Command:**
`:XGST#`

**Returns:**
- `float#`

<br>

### Get Tracking speed
Get the absolute tracking speed of the mount. 

**Command:**
`:XGT#`

**Returns:**
- `float#`

<br>

### Get HA (Hour Angle of Polaris)
Get the current HA of Polaris that the mount thinks it is. 

**Command:**
`:XGH#`

**Returns:**
- `HHMMSS#`

<br>

### Get RA Homing offset
Get the RA ring homing offset. If a Hall sensor is present this is the number of steps from the center of the sensor range to where the actual center position is located. If no Hall sensor is present this is the number of steps from the power on position of the RA axis to 

**Command:**
`:XGHR#`

**Returns:**
- `n#` - the number of steps

<br>

### Get DEC Homing offset
Get the DEC ring homing offset. If a Hall sensor is present this is the number of steps from the center of the sensor range to where the actual center position is located. If no Hall sensor is present this is the number of steps from the power on position of the DEC axis to 

**Command:**
`:XGHD#`

**Returns:**
- `n#` - the number of steps

<br>

### Get Hemisphere
Get the hemisphere that the OAT currently assumes it is operating in. This is set via setting Latitude (see `:St` command) 

**Command:**
`:XGHS#`

**Returns:**
- `N#` - for northern hemisphere
- `S#` - for southern hemisphere

<br>

### Get Mount configuration settings
**Command:**
`:XGM#`

**Returns:**
- `<board>,<RA Stepper Info>,<DEC Stepper Info>,<GPS info>,<AzAlt info>,<Gyro info>,<Display info>,(more features...)#`

**Parameters:**
- `<board>` is one of the supported boards (currently Mega, ESP32, MKS)
- `<Stepper Info>` is a pipe-delimited string of Motor type (NEMA or 28BYJ), Pulley Teeth, Steps per revolution)
- `<GPS info>` is either NO_GPS or GPS, depending on whether a GPS module is present
- `<AzAlt info>` is either NO_AZ_ALT, AUTO_AZ_ALT, AUTO_AZ, or AUTO_ALT, depending on which AutoPA stepper motors are present
- `<Gyro info>` is either NO_GYRO or GYRO depending on whether the Digital level is present
- `<Display info>` is either NO_LCD or LCD_display_type depending on whether LCD is present and if so, which one
- `<Focuser info>` is either NO_FOC or FOC depending on whether the focuser motor is enabled
- `<RAHallSensor info>` is either NO_HSAH or HSAH depending on whether the Hall sensor based auto homing for RA is enabled
- `<Endswitch info>` is either NO_ENDSW or ENDS_RA, ENDSW_DEC, or ENDSW_RA_DEC depending on which axis have end switches installed

**Remarks:**
As OAT/OAM firmware supports more features, these may be appended, separated by a comma. Any further features will
have a 'NO_xxxxx' if the feature is not supported.
To differentiate between OAT and OAM, use the Get Product Name (#GVP) command.

**Example:**
- `ESP32,28BYJ|16|4096.00,28BYJ|16|4096.00,NO_GPS,NO_AZ_ALT,NO_GYRO,NO_LCD,NO_FOC,NO_ENDSW#`

<br>

### Get Mount driver configuration
**Command:**
`:XGMS#`

**Returns:**
- `<RA driver>,<RA slewMS>,<RA trackMS>|<DEC driver>,<DEC slewMS>,<DEC guideMS>|#`

**Parameters:**
- `<driver>` is one of the supported drivers: TU=TMC2209UART, TS=TMC2209STANDALONE, A=A4983
- `<slewMS>` is the microstepping divider (1, 2, 4, 8, 15, 21, 64, 128, 256) used when slewing
- `<trackMS>` is the microstepping divider (1, 2, 4, 8, 15, 21, 64, 128, 256) used when tracking RA
- `<guideMS>` is the microstepping divider (1, 2, 4, 8, 15, 21, 64, 128, 256) used when guiding DEC

**Example:**
- `TU,8,64|TU,16,64|#`

<br>

### Get network settings
Gets the current status of the Wifi connection. Reply only available when running on ESP boards. 

**Command:**
`:XGN#`

**Returns:**
- `1,<mode>,<status>,<hostname>,<ip>:<port>,<SSID>,<OATHostname>#` if Wifi is enabled
- `0,#` if Wifi is not enabled

<br>

### Get LST
Get the current LST of the mount. 

**Command:**
`:XGL#`

**Returns:**
- `HHMMSS#`

<br>

### Set Backlash correction steps
Sets the number of steps the RA stepper motor needs to overshoot and backtrack when slewing east. 

**Command:**
`:XSBn#`

**Returns:**
- nothing

<br>

### Set homing offset for RA ring from Hall sensor center
This offset is added to the position of the RA ring when it is centered on the hall sensor triggered range after running. the RA homing command (:MHRx#) 

**Command:**
`:XSHRnnn#`

**Returns:**
- nothing

**Parameters:**
- `n` is the (positive or negative) number of steps that are needed from the center of the Hall sensor trigger range to the actual home position.

<br>

### Set homing offset for DEC ring from Hall sensor center
This offset is added to the position of the DEC ring when it is centered on the hall sensor triggered range after running. the DEC homing command (:MHDx#) 

**Command:**
`:XSHDnnn#`

**Returns:**
- nothing

**Parameters:**
- `n` is the (positive or negative) number of steps that are needed from the center of the Hall sensor trigger range to the actual home position.

<br>

### Set RA steps
Set the number of steps the RA stepper motor needs to take to rotate by one degree. 

**Command:**
`:XSRn.n#`

**Returns:**
- nothing

**Parameters:**
- `n.n` is the number of steps (only one decimal point is supported, must be positive)

<br>

### Set DEC steps
Set the number of steps the DEC stepper motor needs to take to rotate by one degree. 

**Command:**
`:XSDn.n#`

**Returns:**
- nothing

**Parameters:**
- `n.n` is the number of steps (only one decimal point is supported, must be positive)

<br>

### Set DEC upper limit
Set the upper limit for the DEC axis to the current position if no parameter is given, otherwise to the given angle (in degrees from the home position). 

**Command:**
`:XSDLUnnnnn#`

**Returns:**
- nothing

**Parameters:**
- `nnnnn` is the number of steps from home that the DEC ring can travel upwards. Passing 0 will reset it to the
- limits defined in your configuration file. Omitting this parameter sets it to the current DEC position.

<br>

### Clear DEC upper limit
Resets the upper limit for the DEC axis to the configuration-defined position. If not configured, the limit is cleared. 

**Command:**
`:XSDLu#`

**Returns:**
- nothing

<br>

### Set DEC lower limit
Set the lower limit for the DEC axis to the current position if no parameter is given, otherwise to the given angle (in degrees from the home position). 

**Command:**
`:XSDLLnnnnn#`

**Returns:**
- nothing

**Parameters:**
- `nnnnn` is the number of steps from home that the DEC ring can travel downwards. Passing 0 will reset it to the
- limits defined in your configuration file. Omitting this parameter sets it to the current DEC position.

<br>

### Clear DEC lower limit
Resets the lower limit for the DEC axis to the configuration-defined position. If not configured, the limit is cleared. 

**Command:**
`:XSDLl#`

**Returns:**
- nothing

<br>

### Set DEC parking position offset
This stores the number of steps needed to move from home to the parking position. 

**Command:**
`:XSDPnnnn# (obsolete, disabled)`

**Returns:**
- nothing

<br>

### Set Tracking speed adjustment
Set the adjustment factor used to speed up `(>1.0)` or slow down `(<1.0)` the tracking speed of the mount 

**Command:**
`:XSSn.nnn#`

**Returns:**
- nothing

**Parameters:**
- `n.nnn` is the factor to multiply the theoretical speed by

<br>

### Set Tracking motor position (no movement)
This is purely a debugging aid. It is not recommended to call this unless you know what you are doing. It simply sets the internal tracking steps to the given value. 

**Command:**
`:XSTnnnn#`

**Returns:**
- nothing

**Parameters:**
- `nnn` is the stepper steps to set

<br>

### Set Manual Slewing Mode
Toggle the manual slewing mode state where the RA and DEC motors run at a constant speed 

**Command:**
`:XSMn#`

**Returns:**
- nothing

**Parameters:**
- `n` is '1' to turn it on, otherwise turn it off

<br>

### Set RA Speed
Set RA manual slewing speed in degrees/sec immediately. Max is around 2.5 degs/s 

**Command:**
`:XSXn.nnn#`

**Returns:**
- nothing

**Remarks:**
Must be in manual slewing mode.

<br>

### Set DEC Speed
Set DEC manual slewing speed in degrees/sec immediately. Max is around 2.5 degs/s 

**Command:**
`:XSYn.nnn#`

**Returns:**
- nothing

**Remarks:**
Must be in manual slewing mode.

<br>

## FOCUS FAMILY
<br>

### Start Focuser moving inward (toward objective)
Continues pull in until stopped 

**Command:**
`:F+#`

**Returns:**
- nothing

<br>

### Pull out
Continues pull out until stopped 

**Command:**
`:F-#`

**Returns:**
- nothing

<br>

### Set speed factor
Set focuser speed to <n> where <n> is an ASCII digit 1..4. 1 is slowest, 4 is fastest 

**Command:**
`:Fn#`

**Returns:**
- nothing

<br>

### Set slowest speed factor
Set focuser to the slowest speed it can use 

**Command:**
`:FS#`

**Returns:**
- nothing

<br>

### Set fastest speed factor
Set focuser speed to the fastest speed it can use 

**Command:**
`:FF#`

**Returns:**
- nothing

<br>

### Get position
Get the current position of the focus stepper motor 

**Command:**
`:Fp#`

**Returns:**
- `nnn#` `nnn` is the current position of the stepper

<br>

### Set position
Sets the current position of the focus stepper motor 

**Command:**
`:FPnnn#`

**Returns:**
- `1`

**Parameters:**
- `nnn` is the new position of the stepper. The stepper is not moved.

<br>

### Get focuser state
Gets the state of the focuser stepper. 

**Command:**
`:FB#`

**Returns:**
- `0` if the focuser is idle
- `1` if the focuser is moving

<br>

### Stop focuser
Stops the stepper motor of the focuser. 

**Command:**
`:FQ#`

**Returns:**
- nothing

<br>



> AUTOMATICALLY GENERATED FROM FIRMWARE - DO NOT EDIT
{.is-danger}

> This documentation is current as of Firmware **V1.13.13**
{.is-warning}

