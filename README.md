**Pool Automation Unit (P.A.U.)**

P.A.U. is an ESPHome-based swimming-pool controller for monitoring pool equipment and managing selected pump, heating, lighting, and dosing functions. It connects to Home Assistant through the ESPHome Native API and includes a local touchscreen interface.
Safety notice: This project controls water-treatment equipment and pumps. It is not safety-certified. Verify wiring, interlocks, sensor calibration, and operating limits for your own installation. Never rely on firmware as the sole protection against chemical exposure, dry-running pumps, or electrical hazards.

**Features**

- Pump drive: Modbus control of drive frequency, with selectable 25, 37, and 50 Hz settings.
- Boost: Physical boost-button operation; requests 50 Hz while pressed, then restores frequency according to the selected season.
- Fairland heat pump: Modbus monitoring and control, including operating mode, setpoint, and schedule.
- Water monitoring: pH and Rx/ORP readings, temperature sensors, flow meters, and energy pulse counters.
- Chemical dosing: Acid and chlorine pump controls with an interlock, timed dosing, flow checks, startup lockouts, and calibration-aware logic.
- Pool lighting: RGB control and PLP scene selection over UART.
- Touchscreen: ILI9341 display with XPT2046 touch input, rotating status screens, equipment menus, calibration screens, and device information.
- Home Assistant: ESPHome entities, encrypted Native API, and Wi-Fi fallback hotspot.

**Hardware**

**The configuration targets:**

- ESP32-S3 DevKitC-1 running ESP-IDF
- ILI9341 320×240 display over SPI
- XPT2046 touchscreen
- RS-485/Modbus pump drive and Fairland heat pump
- I²C EZO pH and Rx/ORP sensors
- One-wire temperature sensors
- Pulse-based flow and energy meters
- Acid and chlorine dosing pump outputs
- UART-controlled RGB pool lights

GPIO assignments are defined near the top of pau.yml under substitutions:. Confirm these assignments against your board and schematic before connecting hardware. Do not assume another PCB revision has the same pinout.

**Project files:**

Keep these files together in the ESPHome configuration directory:
- pau.yml
- display_helpers.h
- plp_helper.h
- schedule_helper.h
- dosing_helpers.h
- secrets.yaml

pau.yml includes the four helper headers. The PLP scene names and Modbus command mapping are maintained in plp_helper.h; schedule-window logic is in schedule_helper.h; shared display drawing functions are in display_helpers.h; main-flow validation is in dosing_helpers.h.

**Secrets:**

Create secrets.yaml in the ESPHome configuration directory. Provide values for every secret referenced by your copy of pau.yml, for example:
- wifi_ssid: "your-wifi-ssid"
- wifi_password: "your-wifi-password"
- pool-automation-unit_ap_password: "unique-fallback-password"
- pool-automation-unit_api_key: "your-unique-api-encryption-key"

Use unique credentials. Do not commit secrets.yaml or real keys/passwords to GitHub.

**Build and install:**

With the ESPHome command-line tool:
- esphome config pau.yml
- esphome compile pau.yml
- esphome run pau.yml

Esphome run compiles and uploads firmware to the selected device. To inspect logs:
- esphome logs pau.yml

Check the configuration before flashing, especially after changing GPIO substitutions, Modbus register mappings, dosing rules, or selector options.

**Touchscreen and controls:**

The display rotates through status screens showing pool flow and chemistry, temperatures, drive status, Fairland heat-pump status, and energy readings. The menu provides pump, heat-pump, pH, Rx, season, pool-light, pool-cover placeholder, and device-information pages.

The drive frequency selector provides 25, 37, and 50 Hz. The season selector offers Fall, Spring, Summer, and Winter. Current configuration behavior uses 25 Hz for Winter boot/boost return and 37 Hz for other seasons; the Fairland schedule currently runs only in Summer.

The pool-cover page is a placeholder until pool-cover hardware and controls are configured.

**Dosing and calibration:**

The controller checks sensor availability and flow before dosing, enforces pump interlocking and timed run periods, and suspends dosing during calibration and boost operation. These are firmware safeguards—not substitutes for correctly installed, independently verified physical protections.

An independent dosing watchdog runs every second, including when automatic dosing decisions are disabled or clock synchronization is unavailable. `flowmeter_water` must measure circulation through the chemical injection line. A published, finite, positive main-flow reading is required; readings older than 75 seconds are unsafe. Fairland flow cannot substitute for main circulation flow. The watchdog stops both dosing pumps on missing, invalid, zero, or stale main flow, or when calibration or boost starts. Flow failure resets the circulation warmup counter, including while dosing is idle. The watchdog never starts or resumes a dose; later automatic doses must pass the normal pre-dose checks again, including the 600-second circulation wait after a flow failure. Existing dose durations and cooldowns are unchanged.

Flow-meter updates remain every 60 seconds. Pulse counts represent an interval average, so detecting zero flow after circulation stops can take approximately 120 seconds plus watchdog scheduling latency. A failed meter that continues publishing a positive reading cannot be detected by this freshness check. Public dosing requests are checked before internal relay activation. The internal GPIO `on_turn_on` guards remain reactive defense in depth, not a hardware interlock. Use independently verified physical flow protection; do not rely on this firmware for immediate shutdown.

Entering a blocked safety state also sends OFF to both dosing switches once, even if both already report OFF. This cancels pending GPIO interlock activations so a queued dose cannot start automatically after flow recovery or after leaving calibration/boost. Repeated idle watchdog checks do not repeat these OFF commands.

pH and Rx expire to NaN after 75 seconds without raw sensor updates. The timeout runs before averaging, so normal 60-second readings keep the existing 10-sample pH and 5-sample Rx averages available between publications. Invalid/non-finite readings, including Rx above 1000 mV, immediately invalidate the value and reset its averaging window. Recovery requires a complete fresh window; calibration still displays valid readings directly. Scheduled sensor sleep also expires old values, so waking sensors must collect fresh averages again.

Unavailable pH or Rx blocks automatic dosing and both dosing-switch entry guards. The independent watchdog stops active dosing and cancels queued interlock starts when either chemistry value becomes unavailable. A chemistry fault does not reset the circulation runtime when main flow remains valid. Timeout detection depends on the sensor-filter scheduler, followed by the next one-second watchdog check; these firmware checks do not provide an immediate physical interlock.

The exposed "Acid Pump" and "Chlorine Pump" switches are checked request switches, not direct GPIO controls. Manual and automatic ON requests use the same final gate: valid main flow and chemistry readings, no calibration/boost, at least 600 seconds on the circulation warmup counter, pH no higher than 8.5, no active dosing pump, the selected pump's startup lockout cleared, and at least 60 seconds since either dose started. Acid requires pH above its setpoint, 600 seconds since pH calibration, and 3600 seconds since the previous acid dose. Chlorine requires pH below 7.8, Rx at or below its setpoint, 600 seconds since Rx calibration, and 1800 seconds since the previous chlorine dose. Calibration/dose waits apply when a previous timestamp exists. Requests never advance the warmup counter. A rejected ON request cannot energize a relay or interrupt the other chemical pump. Repeated ON requests do not restart or extend an active dose.

OFF remains unconditional and cancels pending relay activation. Public names and IDs are unchanged, and public states mirror the internal relay states without optimistic ON reporting. Internal relays retain ALWAYS_OFF startup, GPIO interlocking, and the existing 180-second acid / 120-second chlorine time limits.

Calibrate pH and Rx/ORP probes using the appropriate solutions before enabling automatic dosing. Confirm thresholds and timing for your pool, sensors, and chemical pumps. Test outputs safely before connecting chemicals.
Contributing

Document hardware revisions, GPIO changes, Modbus register changes, and control-policy changes alongside configuration updates. Keep credentials and installation-specific secrets private.
