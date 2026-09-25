# Arduino Taiko Game

以 Arduino、ILI9341 TFT 螢幕與按鍵製作的太鼓節奏遊戲。專案將音符頻率與節奏資料寫入程式，搭配顯示器即時呈現遊戲畫面與輸入回饋；主要程式為 `Taiko_no_Tatsujin.ino`。

## 技術實作

- **TFT 圖形輸出**：使用 `Adafruit_GFX` 與 `Adafruit_ILI9341` 函式庫，經由 SPI 介面驅動 ILI9341 螢幕，並在程式中配置 CLK、MISO、MOSI、DC、CS、RST 腳位。
- **節奏資料**：以 C/C++ 常數定義多組音高頻率，讓 Arduino 可依序播放音符並控制節奏流程。
- **互動邏輯**：整合顯示更新、按鍵輸入與時間控制，讓玩家依節奏操作並取得遊戲回饋；此架構可延伸為判定、分數與關卡機制。

## 使用方式

1. 在 Arduino IDE 安裝 `Adafruit GFX`、`Adafruit ILI9341` 函式庫。
2. 依程式內腳位設定接上 TFT 螢幕與輸入元件。
3. 開啟並上傳 `Taiko_no_Tatsujin.ino`。

本專案用於練習嵌入式系統、SPI 顯示控制、節奏資料設計與互動遊戲程式邏輯。
