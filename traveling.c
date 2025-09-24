#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//macro ฟีลคล้ายๆสรรพนาม เอาไว้ใช้แทนคำที่รู้กันเพื่อที่จะไม่ต้องเรียกชื่อซ้ำๆ
#define csv_file "traveling.csv" //อย่างอันนี้จะใช้csv_file แทนการพิมพ์ชื่อไฟล์ยาวๆเต็มๆทุกครั้งที่ต้องการเรียกใช้
#define MAX_LINE_LENGTH 256

// ฟังก์ชันสำหรับตรวจสอบว่าข้อความค้นหามีอยู่ในบรรทัดหรือไม่
int contains_substring(const char *line, const char *search_term) {
    // ใช้ฟังก์ชัน strstr เพื่อค้นหาสตริงย่อย
    return strstr(line, search_term) != NULL;
} 

int main(){
    FILE *file;
        char line[MAX_LINE_LENGTH];
        char user_search[MAX_LINE_LENGTH];
        
        // เปิดไฟล์ CSV
        file = fopen("traveling.csv", "r");
        if (file == NULL) {
         perror("can't open file traveling.csv ");
         return 1;
        }
    
        int c ; //เก็บตัวเลือกของผู้ใช้
    do {
        printf("---menu---\n1.search\n2.add information\n3.edit information\n4.exit\n---------------------------\nplease select your option\n");
        scanf("%d",&c);
        while(getchar( ) != '\n'
        );
        switch(c){
            case 1 : 
            // ทำความเข้าใจก่อน
            while(1) {
                 printf("\n\n\n-----seach information-----\n\n\nPlease type to search (or type 'back' to return to menu):");
                 // fgets ทำงานคล้ายๆกับ scanf เป็นฟังก์ชันใช้อ่านสตริงจากสตรีมอินพุต
                 fgets(user_search, sizeof(user_search), stdin);
                 user_search[strcspn(user_search, "\n")] = 0;

                 //ตรวจสอบเงื่อนไขการออกจากโปรแกรม
                 if (strcmp(user_search, "back") == 0) {
                    break;
                 }
                 printf("\n--- result ---\n\n\n");

                 rewind(file); //เป็นฟังก์ชั่นที่ใช้ในการตั้งค่าไฟล์กลับไปยังจุดเริ่มต้น เหมือนการกรอเทป
                 //เหมือนสำหรับกรณีที่ต้องการเริ่มอ่านไฟล์ใหม่หมด อย่างการค้นหาอะไรสักอย่างในไฟล์ซ้ำๆ
                 

                 fgets(line, sizeof(line), file);

                 // อ่านข้อมูลทีละบรรทัดจนกว่าจะถึงจุดสิ้นสุดไฟล์
                 while (fgets(line, sizeof(line), file)) {
                    // ลบอักขระขึ้นบรรทัดใหม่ที่ fgets เพิ่มเข้ามา
                    line[strcspn(line, "\n")] = 0;

                    // ตรวจสอบว่าบรรทัดปัจจุบันมีคำที่ค้นหาหรือไม่
                    if (contains_substring(line, user_search)) {
                        
                        printf("%s\n", line);//ถ้ามีก็แสดงทั้งบรรทัด
                    }
                 }
            }
             
        

            break;

            case 2 : 
                 printf("\n-----add information-----\n");
                //โค้ดรับค่าข้อมูลที่เพิ่ม
            break;
            case 3 : 
                 printf("\n-----edit information-----\n");
                //โค้ดแก้ไขข้อมูล
            break;
            case 4 : 
                 printf("\n\n\n-------exit program-------\n\n\nthank you\n");
                 
            break;
            default : 
                 printf(" \n\n\n\ninvalid choice\n\n please select again\n\n");
            break;
        }
    }
    while (c != 4);   

 fclose(file);
 return 0;
}
    
