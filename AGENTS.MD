# Avionics System / Flight Computer Core

This document defines the mandatory rules that any AI agent must follow when reading, modifying, generating, or reviewing code for this avionics project.

This project is a safety-critical dual-MCU rocket avionics and recovery system for the 2026 Taiwan Rocket Competition. Its primary mission is reliable flight-state detection, parachute deployment, telemetry, logging, and sea-recovery localization.

The system does **not** perform active attitude control. It is a flight computer and recovery avionics system, not a TVC or closed-loop guidance controller.

---

## 1. Mission Context

### 1.1 Primary Mission Goals
The highest-priority mission goals are:

1. Reliable drogue parachute deployment at apogee
2. Reliable main parachute deployment at 1 km above sea level
3. Reliable post-landing GPS localization and telemetry for sea recovery
4. Preservation of flight data for post-flight analysis

### 1.2 System Overview
This project uses two independent avionics subsystems:

- **Primary Flight Computer (PFC)** based on STM32F405
- **Backup Recovery Computer (BRC)** based on STM32F103

Each system has its own battery and logic power rail.
Each system can operate independently.
Each parachute stage uses redundant ignition channels:
- Primary MCU controls the primary igniter
- Backup MCU controls the backup igniter

### 1.3 Recovery Strategy
This rocket uses dual deployment:

- **Drogue parachute** deployment at apogee (~3.8 km)
- **Main parachute** deployment at 1 km above sea level

Each parachute deployment stage has dual igniter redundancy:
- Primary igniter controlled by Primary Flight Computer
- Backup igniter controlled by Backup Recovery Computer

The Backup Recovery Computer may take over deployment responsibility only under explicitly defined takeover conditions.

---

## 2. Core Engineering Principles

### 2.1 Safety First
All code must be written as safety-critical embedded software.
Prefer conservative behavior over aggressive behavior.
No code may be introduced that increases the probability of accidental ignition, undefined behavior, deadlock, or timing nondeterminism.

### 2.2 MISRA Compliance
All code must comply with MISRA C:2012 or the project-approved safety subset.

### 2.3 No Dynamic Memory
Dynamic memory allocation is forbidden after startup.

Forbidden:
- `malloc`
- `calloc`
- `realloc`
- `free`
- `new`
- `delete`
- STL containers with hidden dynamic allocation unless explicitly proven safe and preallocated during startup only

All memory must be statically allocated or allocated during deterministic startup initialization only.

### 2.4 Deterministic Execution
All runtime behavior must be deterministic and bounded.

Rules:
- No unbounded loops
- No blocking waits without timeout
- No recursion
- No algorithm with unbounded worst-case runtime
- Every loop must have a clear upper bound

### 2.5 No Exceptions
C++ exceptions are forbidden.
Any mechanism that introduces unpredictable control flow is forbidden.

### 2.6 Fail-Safe Defaults
On boot, reset, communication loss, or fault:
- all pyro outputs must default to safe/off
- all dangerous actuations must be inhibited
- the system must prefer non-ignition over accidental ignition

---

## 3. Safety-Critical Coding Rules

### 3.1 Defensive Programming
All function inputs must be validated.
All module boundaries must check:
- pointer validity
- enum range validity
- numeric bounds
- timeout validity
- state validity

### 3.2 Explicit Status Reporting
Every operation that can fail must return explicit status information.
No silent failure is allowed in mission-critical code.

Use:
- status enums
- error flags
- fault bitmasks

Do not hide errors.

### 3.3 No Hidden Side Effects
Functions in safety-critical logic must avoid hidden state changes.
State transitions must be explicit and reviewable.

### 3.4 Division and Numeric Safety
No division is allowed without explicit zero or near-zero protection.
All control thresholds and conversions must be bounded and range-checked.
Sensor outliers must not directly trigger deployment.

### 3.5 Unit Consistency
All internal engineering calculations must use project-standard SI units unless explicitly documented otherwise.

Required internal units:
- altitude: `m`
- velocity: `m_s`
- acceleration: `m_s2`
- pressure: `Pa`
- temperature: `degC`
- angle: `deg`
- time: `ms`

Do not mix:
- feet and meters
- g and m/s^2
- raw sensor counts and engineering units

Sensor conversion must happen in drivers or conversion layers, not in high-level mission logic.

---

## 4. Naming and Documentation Rules

### 4.1 Naming
Physical units must be reflected in variable names.

Examples:
- `altitude_m`
- `vertical_velocity_m_s`
- `pressure_pa`
- `deployment_altitude_m`
- `heartbeat_timeout_ms`

Global variables must use the `g_` prefix, though global variables should be minimized.

### 4.2 Function Documentation
Every function must include:
- purpose
- inputs
- outputs
- failure behavior
- DAL or project criticality level

Example tags:
- `DAL-A-like` for recovery-critical logic
- `Mission-critical`
- `Non-critical`

### 4.3 Formula Traceability
Any nontrivial mathematical formula must cite:
- requirement ID
- design document
- sensor datasheet
- filtering derivation source

No unexplained "magic formulas" are allowed in flight logic.

---

## 5. System Architecture Rules

### 5.1 Primary Flight Computer (STM32F405)
The Primary Flight Computer is responsible for:
- primary flight-state estimation
- primary recovery decision logic
- primary sensor fusion and event detection
- primary drogue igniter control
- primary main igniter control
- primary telemetry
- primary CSV logging
- heartbeat transmission to backup system

Primary sensors:
- BMI088
- LIS3MDL
- BMP390
- H3LIS331DL
- NEO-M9N
- E22-900T22S
- MPXV2202DPT1 (experiment only)

### 5.2 Backup Recovery Computer (STM32F103)
The Backup Recovery Computer is responsible for:
- backup altitude and position estimation
- backup deployment decision logic during takeover mode
- backup drogue igniter control
- backup main igniter control
- backup telemetry
- backup CSV logging
- heartbeat monitoring of primary system

Backup sensors:
- BMP390
- NEO-M9N
- E22-400T22S

### 5.3 HAL Boundary
Direct register access is forbidden in business logic and flight-state logic.
All hardware access must go through `src/hal/` or approved low-level driver interfaces.

Mission logic must never directly manipulate:
- GPIO registers
- UART registers
- SPI/I2C registers
- timer registers
- interrupt controller registers

---

## 6. Flight State Machine

All recovery logic must be driven by an explicit state machine.

Minimum required states:

- `BOOT`
- `PAD_IDLE`
- `ARMED`
- `ASCENT_POWERED`
- `ASCENT_COAST`
- `APOGEE_CONFIRMED`
- `DROGUE_DEPLOYED`
- `DESCENT_DROGUE`
- `MAIN_DEPLOYED`
- `DESCENT_MAIN`
- `LANDED`
- `FAULT`

### 6.1 State Rules
- Transitions must be explicit
- Illegal transitions must be rejected
- Pyro events must only occur in valid states
- State transition logging is mandatory

### 6.2 Deployment Rules
Allowed deployment actions:
- Drogue deployment only after `APOGEE_CONFIRMED`
- Main deployment only during `DESCENT_DROGUE` and below configured main deployment altitude
- No deployment is allowed in `PAD_IDLE`, `ARMED`, or `ASCENT_POWERED`

### 6.3 Launch Detection
Launch detection must use bounded and filtered criteria.
Single-sample launch detection is forbidden.

Launch detection should use:
- acceleration threshold over multiple samples
- optional altitude trend confirmation
- debounce / persistence logic

### 6.4 Apogee Detection
Apogee detection must not use a single noisy altitude sample.
It must use persistence and trend reversal logic over bounded windows.

### 6.5 Landing Detection
Landing detection must require stable low vertical speed and minimal altitude change over a bounded duration.

---

## 7. Real-Time Execution Model

The project may use RTOS, but all mission-critical tasks must remain deterministic.

### 7.1 Primary MCU Recommended Task Rates
Suggested baseline:
- IMU fast loop: 500 Hz
- State estimation and recovery logic: 100 Hz
- Barometer update: 50 Hz
- GPS processing: 5-10 Hz
- Inter-MCU heartbeat: 10 Hz
- Telemetry: 10 Hz
- CSV logging flush/scheduling: 10-50 Hz

### 7.2 Backup MCU Recommended Task Rates
Suggested baseline:
- Barometer and recovery estimator: 50 Hz
- Heartbeat monitoring: 10 Hz
- GPS processing: 5 Hz
- Backup telemetry: 5-10 Hz
- CSV logging: 10-20 Hz

### 7.3 RTOS Rules
If RTOS is used:
- recovery logic tasks must have higher priority than telemetry and logging
- no blocking I/O in high-priority tasks
- mutexes and queues must use bounded timeouts
- ISR handlers must remain short
- no heavy parsing or file writes in ISR context

### 7.4 Deadline Awareness
Any code added to mission-critical loops must document:
- expected execution frequency
- worst-case runtime expectation
- failure behavior on deadline miss

---

## 8. Pyrotechnic Safety Rules

This section is mandatory and non-negotiable.

### 8.1 Safe Defaults
All pyro outputs must default to OFF during:
- power-up
- reset
- boot
- fault
- disarmed mode
- undefined state

### 8.2 Separate Channels
Each parachute stage has redundant igniters:
- primary igniter controlled by Primary Flight Computer
- backup igniter controlled by Backup Recovery Computer

These channels must remain logically independent.

### 8.3 One-Shot Rule
Each igniter channel may be fired at most once.

Required flags:
- `drogue_primary_fired`
- `drogue_backup_fired`
- `main_primary_fired`
- `main_backup_fired`

Once a channel is marked fired, no code path may re-trigger it.

### 8.4 Deployment Conditions
Deployment must require multiple conditions, not a single sensor reading.

Drogue deployment requires:
- state = `APOGEE_CONFIRMED`
- deployment not previously fired
- system armed
- deployment inhibit not active

Main deployment requires:
- state = `DESCENT_DROGUE`
- altitude below configured main deployment threshold
- deployment not previously fired
- system armed
- deployment inhibit not active

### 8.5 Fixed Firing Pulse
Ignition pulse width must be defined by a fixed constant.
No runtime logic may arbitrarily change pulse duration.

Example:
- `PYRO_FIRE_PULSE_MS`

### 8.6 Event Logging
Before and after each pyro event, log:
- timestamp
- source MCU
- state
- altitude
- deployment type
- channel fired
- health flags
- takeover status

### 8.7 Fault Behavior
No single software fault may directly cause accidental ignition.
In ambiguous cases, the software must inhibit ignition and record the fault.

---

## 9. Primary / Backup Takeover Rules

### 9.1 Normal Mode
In normal operation:
- Primary Flight Computer owns recovery decision authority
- Backup Recovery Computer monitors and logs only

### 9.2 Takeover Preconditions
Backup takeover is allowed only if all of the following are true:

1. Primary heartbeat timeout has occurred
2. Backup system is healthy
3. Backup system is armed and flight-valid
4. Backup system has not already fired the relevant channel
5. Backup system has not received a valid indication that the primary system already completed the relevant deployment
6. Local backup deployment conditions are satisfied

### 9.3 Heartbeat Policy
Inter-MCU heartbeat must be sent periodically over UART.

Recommended:
- heartbeat rate: 10 Hz
- timeout: 300-500 ms

Timeout values must be explicit constants and justified.

### 9.4 Takeover State
When takeover occurs, the backup MCU must enter an explicit takeover mode such as:
- `BACKUP_TAKEOVER_ACTIVE`

Takeover must be logged with:
- timestamp
- takeover reason
- last known primary heartbeat counter
- local state estimate
- local altitude

### 9.5 Double-Fire Prevention
Backup must not fire its igniter if:
- a valid primary deployment event has already been confirmed for that parachute stage
- the system state indicates the stage is already completed
- local logic indicates deployment is no longer appropriate

---

## 10. UART Inter-MCU Communication Rules

Primary and backup systems communicate via UART.

### 10.1 UART Data Contract
Inter-MCU packets should include:
- frame header
- message type
- length
- timestamp_ms
- heartbeat_counter
- system_state
- altitude_m
- vertical_velocity_m_s
- gps_fix_valid
- pyro_status_flags
- health_flags
- CRC

### 10.2 Packet Integrity
All packets must include CRC or equivalent integrity protection.
Mission-critical logic must never trust unchecked UART data.

### 10.3 Loss Handling
UART loss must not immediately imply deployment.
Loss handling must go through takeover rules and bounded timeout logic.

### 10.4 No Dynamic Parsing
UART packet parsing must use fixed-size bounded buffers.
No unbounded string parsing is allowed.

---

## 11. Sensor Usage Rules

### 11.1 Primary Sensor Roles
- `BMI088`: primary high-rate inertial sensing
- `H3LIS331DL`: high-g acceleration event backup
- `BMP390`: primary barometric altitude source
- `LIS3MDL`: auxiliary low-dynamic orientation reference only
- `NEO-M9N`: position, time, recovery localization
- `MPXV2202DPT1`: competition experiment only, not recovery-critical

### 11.2 Backup Sensor Roles
- `BMP390`: backup altitude estimation
- `NEO-M9N`: backup localization and timing

### 11.3 Forbidden Sensor Usage
The following are forbidden as sole deployment triggers:
- raw magnetometer readings
- raw GPS altitude
- single raw pressure sample
- single raw acceleration spike

### 11.4 Filtered Decision Inputs
Recovery decisions must use filtered and range-checked engineering values, not raw sensor values.

---

## 12. Logging and microSD Policy

### 12.1 CSV Logging Required
Flight data must be logged to microSD in CSV format.

### 12.2 CSV Requirements
CSV logs must include headers and consistent units.
Required columns should include at minimum:
- timestamp_ms
- system_id
- state
- altitude_m
- vertical_velocity_m_s
- pressure_pa
- accel_x_m_s2
- accel_y_m_s2
- accel_z_m_s2
- gps_lat_deg
- gps_lon_deg
- gps_alt_m
- gps_fix_valid
- drogue_primary_fired
- drogue_backup_fired
- main_primary_fired
- main_backup_fired
- health_flags
- fault_flags

### 12.3 Logging Safety
Logging must never block or delay mission-critical deployment logic.
If logging fails:
- record fault flag
- continue mission-critical recovery logic
- do not crash or hang

### 12.4 Write Policy
Use buffered, bounded writes.
No unbounded formatting or filesystem retries are allowed in critical loops.

---

## 13. Verification and Testing

### 13.1 Unit Tests
Every new function must include or update unit tests.
Target:
- 100% statement coverage
- 100% decision coverage
for recovery-critical logic where feasible

### 13.2 State-Machine Tests
State transition logic must be covered by tests:
- legal transitions
- illegal transitions
- launch detection
- apogee detection
- main deployment logic
- landing detection
- takeover logic
- double-fire prevention

### 13.3 Hardware Abstraction Tests
Recovery logic must be testable without real hardware by mocking HAL interfaces.

### 13.4 Static Analysis
After modifications, run:
- `make lint`

### 13.5 Mandatory Validation Sequence
After editing, run in order:
- `make format`
- `make check-misra`
- `make test-suite`

No generated code may be considered complete until these checks are addressed.

---

## 14. Configuration Rules

System parameters are defined in:
- `config/system_params.toml`

Any modification to system parameters requires:
- parameter review
- regeneration of generated headers
- execution of:
  - `just update-config`

Critical parameters include:
- apogee detection thresholds
- main deployment altitude
- heartbeat timeout
- pyro pulse duration
- launch detection thresholds
- landing detection thresholds

No hard-coded mission thresholds may be introduced directly in logic files.

---

## 15. File Context

- Safety-critical mission logic: `src/logic/`
- Hardware abstraction: `src/hal/`
- Drivers and sensor interfaces: `src/drivers/`
- Inter-MCU communication: `src/comms/`
- Configuration source: `config/system_params.toml`

### 15.1 Modification Sensitivity
Files under `src/logic/` must be treated as highly sensitive.
Any change to deployment conditions, state machine logic, or takeover logic requires extra review and tests.

---

## 16. What the AI Agent Must Never Do

The AI agent must never:
- add dynamic memory to flight logic
- bypass the HAL
- introduce unbounded loops
- remove safety interlocks
- weaken deployment checks
- add direct deployment triggers from raw sensors
- make logging higher priority than recovery
- allow double ignition
- assume UART data is always valid
- hard-code mission thresholds outside config
- silently ignore faults
- change deployment timing logic without tests
- write code that can energize pyro outputs during boot or disarmed mode

---

## 17. Preferred AI Behavior

When modifying this repository, the AI agent should:
- prefer minimal, reviewable changes
- preserve deterministic behavior
- preserve separation between sensing, estimation, decision, and actuation
- explicitly document assumptions
- add tests alongside logic changes
- treat deployment and takeover logic as the highest criticality code in the repository

---

## 18. Development Workflow and Repository Rules

### 18.1 Do Not Push Directly
The AI agent must not push directly to `main`, `master`, or other protected branches unless the user explicitly instructs it to do so.

Preferred workflow:
1. inspect the repository state
2. create or use a feature branch
3. make minimal changes
4. run formatting, checks, and tests
5. summarize changes clearly
6. only commit or push if explicitly requested

### 18.2 No Silent Git Actions
The AI agent must not:
- push without explicit instruction
- force-push unless explicitly approved
- rewrite history unless explicitly approved
- delete branches unless explicitly requested
- discard user changes without explicit confirmation

### 18.3 Respect Existing Work
Before editing, the AI agent should inspect:
- current branch
- modified files
- uncommitted changes
- generated files
- configuration files related to the target module

The AI agent must avoid overwriting unrelated user work.

### 18.4 Minimal and Reviewable Changes
Changes should be:
- small
- localized
- easy to review
- traceable to a specific requirement or bug

Avoid large refactors unless explicitly requested.

### 18.5 Do Not Change Safety Logic Casually
The AI agent must not casually modify:
- parachute deployment thresholds
- launch detection thresholds
- apogee detection rules
- heartbeat timeout values
- pyro firing pulse durations
- fault handling logic
- interlock logic

Such changes require:
- explicit user request or requirement
- updated tests
- updated documentation
- configuration review

### 18.6 Update Tests With Code
If the AI agent adds or changes logic, it must also update:
- unit tests
- integration tests if applicable
- state-machine tests if transitions are affected

### 18.7 Update Documentation With Behavior Changes
If behavior changes, the AI agent must also update the relevant documentation, including:
- `AGENTS.md`
- state machine docs
- recovery logic docs
- protocol docs
- config documentation

### 18.8 Preserve Repository Structure
The AI agent must not arbitrarily reorganize folders, rename files, or move modules unless explicitly requested.

### 18.9 Generated Files
If a file is generated from configuration or scripts, the AI agent must not hand-edit generated output unless explicitly instructed.
The source file or generator should be modified instead.

Example:
- if `config/system_params.toml` changes, run `just update-config`
- do not manually patch generated headers

### 18.10 Commit Style
If asked to commit, prefer small and descriptive commits.

Good examples:
- `add bounded apogee detection filter`
- `add backup heartbeat timeout handling`
- `fix CSV logger buffer overflow risk`

Bad examples:
- `update code`
- `misc fixes`

### 18.11 Branch Naming
If asked to create a branch, prefer descriptive names such as:
- `feature/backup-takeover-logic`
- `fix/main-deploy-threshold-check`
- `test/state-machine-coverage`

### 18.12 Never Assume Git Permission
The AI agent must not assume it is allowed to:
- push to remote
- create pull requests
- merge branches
- tag releases

These actions require explicit user intent.

### 18.13 Protect Critical Files
Extra caution is required when modifying:
- `src/logic/`
- deployment logic
- takeover logic
- ignition control code
- config thresholds
- inter-MCU protocol definitions

These files must be treated as high-review areas.

### 18.14 Do Not Mix Unrelated Changes
The AI agent should avoid combining:
- logic changes
- formatting-only changes
- refactors
- documentation updates
- test rewrites

into one large opaque patch unless explicitly requested.
