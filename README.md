# 🚦 STM32 Traffic Light Control System (Lab 6 - VXL & VĐK)

> **Dự án Bài Tập Lớn (Lab 6):** Thiết kế và hiện thực hóa hệ thống điều khiển đèn giao thông ngã tư (Road A & Road B) trên vi điều khiển **STM32F103C6**, kết hợp đèn báo người đi bộ, màn hình đếm ngược 4 LED 7 đoạn, nút nhấn chuyển chế độ hoạt động và giám sát/điều khiển từ xa qua giao tiếp **UART** trên phần mềm mô phỏng **Proteus**.

* **Thời gian thực hiện:** `31/10/2026` – `14/11/2026` (02 tuần)
* **Môi trường phát triển:** STM32CubeIDE & Proteus 8 Professional

---

## 👥 1. Danh Sách Thành Viên & Phân Công Nhiệm Vụ

| STT | Họ và Tên | Vai trò | Nhánh Git (Branch) | Module & File Phụ trách | Trọng tâm Công việc |
| :-: | :--- | :--- | :--- | :--- | :--- |
| **1** | **Dương Quốc Anh** *(Leader)* | System Architect & Timer Core | `feat/quoc-anh-base-timer` | `TrafficLight.pdsprj`, `.ioc`, `main.c`, `global.c/.h`, `software_timer.c/.h` | Vẽ mạch Proteus, cấu hình CubeMX, viết thư viện Timer mềm 10ms, điều phối tích hợp & duyệt PR (Part 1 & Part 5). |
| **2** | **Nguyễn Quang Dũng** | Display & Hardware Driver | `feat/quang-dung-7seg` | `led_display.c`, `led_display.h` | Điều khiển 6 LED giao thông + 2 LED đi bộ; quét đa hợp 4 LED 7 đoạn qua transistor PNP và đồng bộ số đếm ngược (Part 1 & Part 2). |
| **3** | **Nguyễn Huỳnh Gia Bửu** | Control & FSM Engineer | `feat/gia-buu-fsm` | `input_reading.c/.h`, `fsm_traffic.c/.h` | Vẽ sơ đồ FSM; chống dội 2 nút nhấn `PC13`, `PC14`; lập trình `fsm_for_mode()` (3 mode) và `fsm_for_light_cycle()` (pha S2 cộng dồn) (Part 3). |
| **4** | **Trần Ngọc Hùng Anh** | UART Telemetry & QA Lead | `feat/hung-anh-uart` | `uart_comm.c/.h`, `Docs_and_Reports/` | Nhận ngắt UART, viết `command_parser_fsm()` xử lý lệnh `MODE=n`, hàm `log_state_transition()`, tổng hợp báo cáo & quay video (Part 4 & Part 5). |

---

## ⚙️ 2. Thông Số Kỹ Thuật Đã Thống Nhất (Technical Specifications)

Toàn bộ thành viên trong nhóm tuân thủ nghiêm ngặt các quy ước kỹ thuật sau khi lập trình:

### 2.1. Quy tắc Chu kỳ Đèn & Pha Đi Bộ Cộng Dồn (`S0` -> `S4`)
* Khi có tín hiệu nhấn nút đi bộ (`PC13`) trong lúc Road A đang Xanh (`S0`) hoặc Vàng (`S1`), hệ thống bật cờ `pedestrian_flag = 1`.
* Khi chuyển sang pha Đỏ của Road A, nếu `pedestrian_flag == 1`, hệ thống đi vào trạng thái **`S2` (5 giây)** trước khi sang **`S3` (8 giây)** và **`S4` (2 giây)**.
* **Thời gian cộng dồn:** Khi có người đi bộ, tổng thời gian đèn Đỏ của Road A là **$5 + 8 + 2 = 15\text{ giây}$** (và tổng thời gian đèn Xanh của Road B là **$5 + 8 = 13\text{ giây}$**). Nếu không có người đi bộ (`pedestrian_flag == 0`), hệ thống bỏ qua `S2`, chuyển thẳng từ `S1` sang `S3` (tổng Đỏ Road A = 10s, Xanh Road B = 8s).

| Trạng thái | Đèn Road A | Đèn Road B | Đèn Đi Bộ (`PED_A`) | Thời lượng | Đếm ngược Khởi tạo (Road A / Road B) |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **`S0`** | 🟢 Xanh (Green) | 🔴 Đỏ (Red) | Tắt | `8s` | `08` / `10` |
| **`S1`** | 🟡 Vàng (Amber) | 🔴 Đỏ (Red) | Tắt | `2s` | `02` / `02` |
| **`S2`** *(Nếu `ped_flag=1`)* | 🔴 Đỏ (Red) | 🟢 Xanh (Green) | 🚶 **Sáng (ON)** | `5s` | `15` / `13` *(Xóa cờ `ped_flag = 0`)* |
| **`S3`** | 🔴 Đỏ (Red) | 🟢 Xanh (Green) | Tắt | `8s` | `10` / `08` |
| **`S4`** | 🔴 Đỏ (Red) | 🟡 Vàng (Amber) | Tắt | `2s` | `02` / `02` *(Hết S4 quay về S0)* |

### 2.2. Bảng Phân Bổ Chân Vi Điều Khiển (Pin Assignment)
* Giữ nguyên cấu hình chân theo đúng thiết kế chuẩn của đề bài:

| Tín hiệu Phần cứng | Chân STM32F103C6 | Nhãn CubeMX (User Label) | Ghi chú Thiết kế |
| :--- | :--- | :--- | :--- |
| **Road A: Đỏ, Vàng, Xanh** | `PA0`, `PA1`, `PA2` | `ROADA_RED`, `ROADA_AMBER`, `ROADA_GREEN` | GPIO Output |
| **Road B: Đỏ, Vàng, Xanh** | `PA3`, `PA4`, `PA5` | `ROADB_RED`, `ROADB_AMBER`, `ROADB_GREEN` | GPIO Output |
| **Đèn báo đi bộ (Road A, B)** | `PA6`, `PA7` | `PED_A`, `PED_B` | GPIO Output |
| **LED 7 đoạn (Các đoạn a–g)** | `PB0` -> `PB6` | `SEG_A` -> `SEG_G` | Anode chung (Mức `0` sáng) |
| **Chân chọn LED 7 đoạn (4 số)** | `PB12` -> `PB15` | `EN0`, `EN1`, `EN2`, `EN3` | Kích qua Transistor PNP (Mức `0` dẫn) |
| **Nút nhấn (Đi bộ, Mode, Set)** | `PC13`, `PC14`, `PC15` | `BTN_PED`, `BTN_MODE`, `BTN_SET` | GPIO Input Pull-up (Nhấn = `0`) |
| **UART Truyền / Nhận** | `PA9` (TX), `PA10` (RX) | `UART_TX`, `UART_RX` | 9600 baud, 8 data bits, No parity, 1 stop bit |

---

## 📂 3. Cấu Trúc Thư Mục Workspace

```text
📦 STM32-Traffic-Light-Lab6
 ┣ 📂 Proteus_Simulation/
 ┃ ┗ 📜 TrafficLight_System.pdsprj      # File mô phỏng Proteus (Chỉ Dương Quốc Anh chỉnh sửa)
 ┣ 📂 STM32CubeIDE_Project/
 ┃ ┣ 📂 Core/
 ┃ ┃ ┣ 📂 Inc/
 ┃ ┃ ┃ ┣ 📜 main.h                      # Sinh tự động bởi CubeMX
 ┃ ┃ ┃ ┣ 📜 global.h                    # Khai báo hằng số, enum, biến toàn cục dùng chung
 ┃ ┃ ┃ ┣ 📜 software_timer.h            # Header bộ định thời mềm (Dương Quốc Anh)
 ┃ ┃ ┃ ┣ 📜 led_display.h               # Header điều khiển LED đơn & 7 đoạn (Nguyễn Quang Dũng)
 ┃ ┃ ┃ ┣ 📜 input_reading.h             # Header chống dội nút nhấn (Nguyễn Huỳnh Gia Bửu)
 ┃ ┃ ┃ ┣ 📜 fsm_traffic.h               # Header Mode FSM & Light Cycle FSM (Nguyễn Huỳnh Gia Bửu)
 ┃ ┃ ┃ ┗ 📜 uart_comm.h                 # Header UART Parser & Logging (Trần Ngọc Hùng Anh)
 ┃ ┃ ┗ 📂 Src/
 ┃ ┃   ┣ 📜 main.c                      # Vòng lặp chính & Callback ngắt (Dương Quốc Anh quản lý)
 ┃ ┃   ┣ 📜 global.c                    # Định nghĩa biến toàn cục
 ┃ ┃   ┣ 📜 software_timer.c            # Phụ trách: Dương Quốc Anh
 ┃ ┃   ┣ 📜 led_display.c               # Phụ trách: Nguyễn Quang Dũng
 ┃ ┃   ┣ 📜 input_reading.c             # Phụ trách: Nguyễn Huỳnh Gia Bửu
 ┃ ┃   ┣ 📜 fsm_traffic.c               # Phụ trách: Nguyễn Huỳnh Gia Bửu
 ┃ ┃   ┗ 📜 uart_comm.c                 # Phụ trách: Trần Ngọc Hùng Anh
 ┃ ┗ 📜 TrafficLight.ioc                # File cấu hình CubeMX (KHÔNG tự ý bấm Generate Code)
 ┣ 📂 Docs_and_Reports/                 # Chứa sơ đồ FSM, ảnh chụp mạch, báo cáo & video demo
 ┗ 📜 README.md                         # Hướng dẫn làm việc nhóm & Bảng theo dõi tiến độ
```

---

## 🌿 4. Quy Trình Tương Tác Git & Phòng Chống Xung Đột (Conflict Prevention)

### 4.1. Nguyên tắc Phân nhánh
1. **`main`**: Nhánh chứa sản phẩm hoàn chỉnh cuối cùng. **Nghiêm cấm push trực tiếp lên `main`**.
2. **`develop`**: Nhánh tích hợp của cả nhóm. Mọi module sau khi viết xong sẽ tạo Pull Request (PR) vào nhánh này để kiểm thử chung.
3. **Nhánh cá nhân (`feat/...`)**: Mỗi thành viên làm việc trên nhánh riêng của mình đã được phân công ở Bảng 1.

### 4.2. Các Lệnh Git Thao Tác Hằng Ngày
* **Bước 1: Đồng bộ code mới nhất từ nhánh `develop` trước khi làm việc**
  ```bash
  git checkout develop
  git pull origin develop
  git checkout feat/<nhanh-cua-ban>
  git merge develop
  ```
* **Bước 2: Lưu thay đổi (Commit) và đẩy lên nhánh cá nhân**
  ```bash
  git add Core/Src/<file-cua-ban>.c Core/Inc/<file-cua-ban>.h README.md
  git commit -m "feat(ten-module): mo ta ngan gon cong viec da lam"
  git push origin feat/<nhanh-cua-ban>
  ```
* **Bước 3: Tạo Pull Request & Review**
  * Lên GitHub tạo **Pull Request** từ nhánh `feat/<nhanh-cua-ban>` vào nhánh `develop`.
  * Nhắn vào nhóm để **Dương Quốc Anh (Leader)** và các thành viên khác vào Review code. Cần tối thiểu **2 lượt Approve** trước khi bấm **Merge**.

### 4.3. 4 Quy Tắc Vàng Để Không Bao Giờ Bị Merge Conflict
1. **Không chạm vào file của người khác:** Ai phụ trách cặp file `.c/.h` nào thì chỉ sửa đúng file đó.
2. **Không tự ý mở `TrafficLight.ioc` để sinh lại code:** Mọi yêu cầu đổi cấu hình chân hoặc Timer phải báo cho **Dương Quốc Anh** thực hiện tập trung.
3. **Không sửa trực tiếp `main.c`:** Khi viết xong hàm mới (ví dụ `update7SEG()` hay `fsm_for_mode()`), hãy báo cho **Dương Quốc Anh** để gắn lời gọi hàm vào vòng lặp `while(1)` hoặc hàm ngắt `HAL_TIM_PeriodElapsedCallback()` trong `main.c`.
4. **Tuyệt đối không dùng `HAL_Delay()`:** Mọi tác vụ định thời bắt buộc phải gọi hàm `setTimer(id, ms)` và kiểm tra cờ `timer_flag[id] == 1` từ module `software_timer`.

---

## 📅 5. Kế Hoạch 5 Giai Đoạn (Milestones: 31/10/2026 – 14/11/2026)

| Giai đoạn | Thời gian | Trọng số | Nội dung Công việc Chính (Deliverables) | Phụ trách chính | Trạng thái |
| :--- | :---: | :---: | :--- | :--- | :---: |
| **Giai đoạn 1**<br>*(Part 1)* | `31/10` – `02/11/2026` | **15%** | Vẽ hoàn chỉnh mạch Proteus, cấu hình CubeMX, code `software_timer` 10ms và chạy chu kỳ đèn cơ bản Road A & B. | Dương Quốc Anh,<br>Nguyễn Quang Dũng | ⏳ Đang thực hiện |
| **Giai đoạn 2**<br>*(Part 2)* | `03/11` – `05/11/2026` | **15%** | Quét đa hợp 4 LED 7 đoạn trong ngắt Timer (tần số >= 50Hz), hiển thị số đếm ngược khớp chính xác thời điểm đổi đèn. | Nguyễn Quang Dũng | 📅 Chờ khởi động |
| **Giai đoạn 3**<br>*(Part 3)* | `06/11` – `09/11/2026` | **30%** | Vẽ sơ đồ FSM; chống dội 2 nút nhấn `PC13`, `PC14`; hoàn thiện 3 Mode hoạt động và pha đi bộ `S2` cộng dồn (15s Đỏ). | Nguyễn Huỳnh Gia Bửu | 📅 Chờ khởi động |
| **Giai đoạn 4**<br>*(Part 4)* | `10/11` – `12/11/2026` | **20%** | Nhận ngắt UART, phân tích lệnh `MODE=0/1/2` ép chuyển mode từ xa, bắn log `!STATE:A=...,B=...,T=...#` và quay video Part 4. | Trần Ngọc Hùng Anh | 📅 Chờ khởi động |
| **Giai đoạn 5**<br>*(Final)* | `13/11` – `14/11/2026` | **20%** | Tích hợp toàn bộ vào nhánh `main`, kiểm thử tổng thể, hoàn thiện báo cáo PDF và quay video demo toàn hệ thống. | Cả 4 thành viên | 📅 Chờ khởi động |

---

## 📊 6. Bảng Theo Dõi Tiến Độ Từng Module (Module Progress Tracker)

| Module / Hạng mục | Phụ trách | Nhánh làm việc | Tiến độ (%) | Trạng thái PR | Người Review | Ghi chú |
| :--- | :--- | :--- | :---: | :---: | :--- | :--- |
| Sơ đồ mạch `TrafficLight.pdsprj` | Dương Quốc Anh | `feat/quoc-anh-base-timer` | `0%` | Chưa tạo | Dũng, Bửu, Hùng Anh | Đầy đủ LED, 7SEG, Button, UART |
| Cấu hình `.ioc` & `global.c/.h` | Dương Quốc Anh | `feat/quoc-anh-base-timer` | `0%` | Chưa tạo | Dũng, Bửu, Hùng Anh | Giữ chuẩn chân PA9/PA10 |
| Module `software_timer.c/.h` | Dương Quốc Anh | `feat/quoc-anh-base-timer` | `0%` | Chưa tạo | Quang Dũng, Gia Bửu | Hỗ trợ 4 kênh timer độc lập |
| Module `led_display.c/.h` | Nguyễn Quang Dũng | `feat/quang-dung-7seg` | `0%` | Chưa tạo | Quốc Anh, Gia Bửu | Quét 4 LED 7SEG + 8 LED đơn |
| Module `input_reading.c/.h` | Nguyễn Huỳnh Gia Bửu | `feat/gia-buu-fsm` | `0%` | Chưa tạo | Quốc Anh, Hùng Anh | Chống dội 2 nút nhấn PC13, PC14 |
| Module `fsm_traffic.c/.h` | Nguyễn Huỳnh Gia Bửu | `feat/gia-buu-fsm` | `0%` | Chưa tạo | Quốc Anh, Dũng, Hùng Anh | S2 cộng dồn 5s (Tổng Đỏ = 15s) |
| Module `uart_comm.c/.h` | Trần Ngọc Hùng Anh | `feat/hung-anh-uart` | `0%` | Chưa tạo | Quốc Anh, Gia Bửu | Parser `MODE=n` & hàm ghi log |
| Báo cáo & Video Demo | Trần Ngọc Hùng Anh | `feat/hung-anh-uart` | `0%` | Chưa tạo | Quốc Anh, Dũng, Bửu | Video Part 4 & Video Final |

---

## 📝 7. Nhật Ký Báo Cáo Tiến Độ Hằng Ngày (Daily Standup Log)

> **Quy định bắt buộc:** Trước **22:00 mỗi ngày**, từng thành viên cập nhật kết quả làm việc trong ngày của mình vào bảng dưới đây.

| Ngày | Thành viên | Công việc đã hoàn thành trong ngày | Kế hoạch ngày tiếp theo | Vướng mắc (Blockers) |
| :---: | :--- | :--- | :--- | :--- |
| **31/10/2026** | Dương Quốc Anh | Khởi tạo GitHub Repo, thống nhất đặc tả kỹ thuật, vẽ mạch Proteus cơ sở. | Hoàn tất cấu hình `.ioc` và viết `global.h`, `software_timer.c`. | Không có |
| **31/10/2026** | Nguyễn Quang Dũng | Nghiên cứu sơ đồ chân LED đơn và mạch transistor PNP quét 7 đoạn. | Viết các hàm bật/tắt đèn giao thông cho Road A và Road B. | Chờ file `global.h` từ Quốc Anh |
| **31/10/2026** | Nguyễn Huỳnh Gia Bửu | Phân tích bảng trạng thái S0–S4 (pha S2 cộng dồn 15s) và 3 Mode. | Vẽ sơ đồ FSM hoàn chỉnh bằng phần mềm để đưa vào báo cáo. | Không có |
| **31/10/2026** | Trần Ngọc Hùng Anh | Xem lại cấu trúc ngắt nhận UART và hàm `sprintf` từ bài Lab 5. | Thiết kế bộ đệm vòng (buffer) và sơ đồ trạng thái cho Command Parser. | Không có |
| **01/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **02/11/2026** | *Cả nhóm cập nhật* | **[Mốc nghiệm thu Giai đoạn 1 - Part 1]** | ... | ... |
| **03/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **04/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **05/11/2026** | *Cả nhóm cập nhật* | **[Mốc nghiệm thu Giai đoạn 2 - Part 2]** | ... | ... |
| **06/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **07/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **08/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **09/11/2026** | *Cả nhóm cập nhật* | **[Mốc nghiệm thu Giai đoạn 3 - Part 3]** | ... | ... |
| **10/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **11/11/2026** | *Cả nhóm cập nhật* | ... | ... | ... |
| **12/11/2026** | *Cả nhóm cập nhật* | **[Mốc nghiệm thu Giai đoạn 4 - Part 4]** | ... | ... |
| **13/11/2026** | *Cả nhóm cập nhật* | Merge toàn bộ vào nhánh `develop`, chạy kiểm thử tích hợp toàn hệ thống. | Quay video tổng kết và đóng gói báo cáo. | ... |
| **14/11/2026** | *Cả nhóm cập nhật* | **[Hạn chót nộp BTL - Merge vào nhánh `main` & Submit]** | Hoàn thành dự án. | Không có |
