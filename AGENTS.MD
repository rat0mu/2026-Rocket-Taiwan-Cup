# 2026 Rocket Taiwan Cup - Avionics System

## Overview
This repository contains the avionics system for our 2026 Rocket Taiwan Cup project.

The system is designed with a modular and fault-tolerant architecture, including:
- Dual flight computer (Primary + Secondary)
- Real-time state estimation
- Telemetry communication
- Data logging and post-flight analysis
- Safety-critical parachute deployment system

## System Architecture
The avionics system is divided into several major components:

- **Primary Flight Computer (PFC)**  
  Handles main flight logic, sensor fusion, and telemetry.

- **Secondary Flight Computer (SFC)**  
  Backup system for redundancy and failover.

- **Ground Station**  
  Receives telemetry and monitors mission status.

- **Simulation**  
  Used for testing flight dynamics and sensor models.

## Project Structure
```text
2026-Rocket-Taiwan-Cup/
├── firmware/          # Embedded flight software
├── hardware/          # PCB and electronics design
├── ground-station/    # Telemetry receiver and UI
├── simulation/        # Flight simulation and models
├── test/              # Unit / integration / HIL testing
├── docs/              # System documentation
└── tools/             # Development utilities
```

## Key Features
- Telemetry system (LoRa / RF communication)
- GPS-based positioning
- Sensor fusion (IMU + Barometer + GPS)
- Fail-safe parachute deployment
- State machine-based flight logic
- Onboard data logging

## Hardware
The hardware design includes multiple PCB layers:

- Avionics upper layer
- Middle layer
- Payload layer
- Lower layer

All PCB designs are located in `hardware/pcb/`.

## Testing Strategy
- Unit testing (algorithm-level)
- Integration testing (module-level)
- HIL (Hardware-in-the-loop)
- Simulation-based validation

## Communication
The system uses a custom telemetry protocol for:

- Sensor data transmission
- Flight state updates
- Command handling

See `docs/communication_protocol.md`.

## Flight States
The system operates using a state machine:

- Idle
- Launch Detection
- Ascent
- Apogee Detection
- Descent
- Recovery

See `docs/flight_state_machine.md`.

## Future Work
- Advanced navigation algorithm
- Improved fault detection
- Ground station UI enhancement
- Flight data visualization tools

## Contributors
- Your Team Name / Members

## License
MIT License (or your choice)

---

# 2026 火箭台灣盃 - 航電系統

## 專案總覽
本儲存庫包含我們 2026 火箭台灣盃專案的航電系統。

本系統採用模組化且具備容錯能力的架構設計，包含：
- 雙飛行電腦（主飛控 + 備援飛控）
- 即時狀態估測
- 遙測通訊
- 資料記錄與飛行後分析
- 安全關鍵的降落傘部署系統

## 系統架構
航電系統可分為以下幾個主要組成部分：

- **主飛行電腦（PFC）**  
  負責主要飛行邏輯、感測器融合與遙測通訊。

- **次飛行電腦（SFC）**  
  作為備援系統，提供冗餘與故障切換能力。

- **地面站**  
  接收遙測資料並監控任務狀態。

- **模擬系統**  
  用於測試飛行動力學與感測器模型。

## 專案結構
```text
2026-Rocket-Taiwan-Cup/
├── firmware/          # 嵌入式飛行軟體
├── hardware/          # PCB 與電子硬體設計
├── ground-station/    # 遙測接收與使用者介面
├── simulation/        # 飛行模擬與模型
├── test/              # 單元 / 整合 / HIL 測試
├── docs/              # 系統文件
└── tools/             # 開發工具
```

## 核心功能
- 遙測系統（LoRa / RF 通訊）
- 以 GPS 為基礎的定位能力
- 感測器融合（IMU + 氣壓計 + GPS）
- 故障保護式降落傘部署
- 以狀態機為基礎的飛行邏輯
- 機載資料記錄

## 硬體設計
硬體設計包含多個 PCB 層板：

- 航電上層
- 中層
- 酬載層
- 下層

所有 PCB 設計檔皆位於 `hardware/pcb/`。

## 測試策略
- 單元測試（演算法層級）
- 整合測試（模組層級）
- HIL（硬體迴圈測試）
- 基於模擬的驗證

## 通訊
本系統使用自訂遙測協定，負責：

- 感測器資料傳輸
- 飛行狀態更新
- 指令處理

請參閱 `docs/communication_protocol.md`。

## 飛行狀態
本系統以狀態機方式運作，包含以下狀態：

- Idle（待機）
- Launch Detection（發射偵測）
- Ascent（上升）
- Apogee Detection（最高點偵測）
- Descent（下降）
- Recovery（回收）

請參閱 `docs/flight_state_machine.md`。

## 未來工作
- 進階導航演算法
- 改善故障偵測能力
- 強化地面站 UI
- 飛行資料視覺化工具

## 貢獻者
- 你的團隊名稱 / 成員

## 授權
MIT License（或依你的需求選擇其他授權）
