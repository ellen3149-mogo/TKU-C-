#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 如果是 Windows 系統，引入編碼控制所需的標頭檔
#ifdef _WIN32
#include <windows.h>
#endif

// 任務 1：以 Student 結構作為鏈結序列中的 node 節點
typedef struct Student {
    char id[20];            // 學號
    char name[50];          // 英文姓名
    struct Student* next;   // 指向下一個節點的指標
} Student;

// 函式宣告
Student* createNode(char* id, char* name);
void insertStudent(Student** head);
void deleteStudent(Student** head);
void searchStudent(Student* head);
void displayAll(Student* head);
void saveToFile(Student* head);
void freeList(Student* head);

int main() {
    // 【關鍵修正】如果是 Windows 環境，強制將終端機輸出與輸入編碼設定為 UTF-8 (65001)
    #ifdef _WIN32
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);
    #endif

    Student* head = NULL;
    int choice;

    // 任務 3：實作命令列介面
    while (1) {
        printf("\n=== 學生資料管理系統 ===\n");
        printf("1. 新增學生資料\n");
        printf("2. 刪除學生資料\n");
        printf("3. 查詢學生資料\n");
        printf("4. 顯示所有學生\n");
        printf("5. 儲存檔案並離開\n");
        printf("請輸入選項 (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("輸入錯誤，請輸入數字！\n");
            while (getchar() != '\n'); // 清除輸入緩衝區，防止死迴圈
            continue;
        }

        switch (choice) {
            case 1:
                insertStudent(&head); // 任務 2：動態新增
                break;
            case 2:
                deleteStudent(&head); // 任務 2：動態刪除
                break;
            case 3:
                searchStudent(head);  // 任務 3：查詢
                break;
            case 4:
                displayAll(head);
                break;
            case 5:
                saveToFile(head);     // 任務 3：儲存檔案
                freeList(head);       // 釋放記憶體
                printf("感謝使用，系統已安全關閉。\n");
                return 0;
            default:
                printf("無效的選項，請重新輸入。\n");
        }
    }
    return 0;
}

// 建立新節點 (使用 malloc 動態配置)
Student* createNode(char* id, char* name) {
    Student* newNode = (Student*)malloc(sizeof(Student));
    if (newNode == NULL) {
        printf("記憶體配置失敗！\n");
        exit(1);
    }
    strcpy(newNode->id, id);
    strcpy(newNode->name, name);
    newNode->next = NULL;
    return newNode;
}

// 新增學生（插入至串列尾端）
void insertStudent(Student** head) {
    char id[20], name[50];
    printf("請輸入學號: ");
    scanf("%s", id);
    printf("請輸入英文姓名: ");
    scanf("%s", name);

    Student* newNode = createNode(id, name);

    if (*head == NULL) {
        *head = newNode;
    } else {
        Student* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    printf("學生資料 [學號: %s, 姓名: %s] 新增成功！\n", id, name);
}

// 刪除學生（依學號刪除，並使用 free 釋放記憶體）
void deleteStudent(Student** head) {
    if (*head == NULL) {
        printf("目前系統內無資料可刪除。\n");
        return;
    }

    char targetId[20];
    printf("請輸入要刪除的學生學號: ");
    scanf("%s", targetId);

    Student* temp = *head;
    Student* prev = NULL;

    // 如果頭節點就是要刪除的目標
    if (strcmp(temp->id, targetId) == 0) {
        *head = temp->next;
        free(temp);
        printf("學號 %s 的資料已成功刪除。\n", targetId);
        return;
    }

    // 尋找目標節點
    while (temp != NULL && strcmp(temp->id, targetId) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // 找不到資料
    if (temp == NULL) {
        printf("找不到學號為 %s 的學生資料。\n", targetId);
        return;
    }

    // 重新連結串列並釋放記憶體
    prev->next = temp->next;
    free(temp);
    printf("學號 %s 的資料已成功刪除。\n", targetId);
}

// 查詢學生資料
void searchStudent(Student* head) {
    if (head == NULL) {
        printf("目前系統內無資料。\n");
        return;
    }

    char targetId[20];
    printf("請輸入要查詢的學生學號: ");
    scanf("%s", targetId);

    Student* current = head;
    while (current != NULL) {
        if (strcmp(current->id, targetId) == 0) {
            printf("\n--- 查詢結果 ---\n");
            printf("學號: %s\n", current->id);
            printf("英文姓名: %s\n", current->name);
            return;
        }
        current = current->next;
    }
    printf("未找到學號為 %s 的學生。\n", targetId);
}

// 顯示所有資料
void displayAll(Student* head) {
    if (head == NULL) {
        printf("目前系統內無任何學生資料。\n");
        return;
    }

    printf("\n=== 目前學生資料清單 ===\n");
    Student* current = head;
    int count = 1;
    while (current != NULL) {
        printf("[%d] 學號: %s | 英文姓名: %s\n", count++, current->id, current->name);
        current = current->next;
    }
}

// 儲存至檔案
void saveToFile(Student* head) {
    FILE* file = fopen("student_records.csv", "w");
    if (file == NULL) {
        printf("無法建立/開啟檔案！\n");
        return;
    }

    // 寫入 CSV 欄位名稱（加入 UTF-8 BOM，防止 Excel 開啟 CSV 時中文變亂碼）
    fprintf(file, "\xEF\xBB\xBF學號,英文姓名\n");

    Student* current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s\n", current->id, current->name);
        current = current->next;
    }

    fclose(file);
    printf("資料已成功儲存至 'student_records.csv'。\n");
}

// 程式結束前釋放所有動態配置的記憶體
void freeList(Student* head) {
    Student* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}