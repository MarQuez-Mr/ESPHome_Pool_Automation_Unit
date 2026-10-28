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
- secrets.yaml

pau.yml includes the three helper headers. The PLP scene names and Modbus command mapping are maintained in plp_helper.h; schedule-window logic is in schedule_helper.h; shared display drawing functions are in display_helpers.h.

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

Calibrate pH and Rx/ORP probes using the appropriate solutions before enabling automatic dosing. Confirm thresholds and timing for your pool, sensors, and chemical pumps. Test outputs safely before connecting chemicals.
Contributing

Document hardware revisions, GPIO changes, Modbus register changes, and control-policy changes alongside configuration updates. Keep credentials and installation-specific secrets private.
