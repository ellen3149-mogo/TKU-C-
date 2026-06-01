# 學生資料管理系統 (Student Management System)

一個使用 C 語言實作的文字介面學生資料管理系統，採用**鏈結串列 (Linked List)** 結構動態管理學生資料。

## 🚀 功能特點
- **跨平台編碼支援**：針對 Windows 環境自動切換終端機編碼為 UTF-8，避免中文亂碼。
- **動態記憶體管理**：使用 Linked List 實作，並包含完整的記憶體釋放機制。
- **完整 CRUD 功能**：
  - 新增學生資料
  - 刪除學生資料
  - 查詢學生資料
  - 顯示所有學生
- **資料持久化**：支援將學生資料儲存至檔案中。

## 🛠️ 資料結構 (Data Structure)
系統核心節點結構如下：
```c
typedef struct Student {
    char id[20];          // 學號
    char name[50];        // 英文姓名
    struct Student* next; // 指向下一個節點的指標
} Student;
