#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LINE_LENGTH 256

// ฟังก์ชันสำหรับตรวจสอบว่าข้อความค้นหามีอยู่ในบรรทัดหรือไม่
int contains_substring(const char *line, const char *search_term) {
    // ใช้ฟังก์ชัน strstr เพื่อค้นหาสตริงย่อย
    return strstr(line, search_term) != NULL;
} //งงบรรทัดนี้

void main(){
    FILE *file;
        char line[MAX_LINE_LENGTH];
        char *token;
        char search_term[MAX_LINE_LENGTH];
        // เปิดไฟล์ CSV
        file = fopen("traveling.csv", "r");
        if (file == NULL) {
        perror("ไม่สามารถเปิดไฟล์ traveling.csv ได้");
        return 1;
        }
            // อ่านข้อมูลทีละบรรทัดจนกว่าจะถึงจุดสิ้นสุดไฟล์
            while (fgets(line, sizeof(line), file)) {
                    printf("อ่านได้: %s", line);
                // อ่านทีละบรรทัด
                while (fgets(line, sizeof(line), file)) {
                    // ลบอักขระขึ้นบรรทัดใหม่
                    line[strcspn(line, "\n")] = 0;

                        // แยกข้อมูลด้วยเครื่องหมายจุลภาค
                        token = strtok(line, ",");
                        while (token != NULL) {
                            printf("  คอลัมน์: %s\n", token);
                            token = strtok(NULL, ","); // อ่านโทเคนถัดไป
                        }
                }
            }
        int c ; //เก็บตัวเลือกของผู้ใช้
    do {
        printf("---menu---\n1.search\n2.add information\n3.edit information\n4.exit\n---------------------------\nplease select your option\n");
        scanf("%d",&c);
        switch(c){
            case 1 : 
                 printf("-----seach information-----\n");
                    printf("กรุณาใส่คำที่ต้องการค้นหา: ");
                    
                    break;

            case 2 : 
                 printf("-----add information-----\n");
                //โค้ดรับค่าข้อมูลที่เพิ่ม
            break;
            case 3 : 
                 printf("-----edit information-----\n");
                //โค้ดแก้ไขข้อมูล
            break;
            case 4 : 
                 printf("-------exit program-------\n");
                 printf("thank you");
            break;
            default : 
                 printf(" invalid choice\n please select again\n");
            break;
        }
    }
    while (c != 4);
    fclose(file);
   
}

