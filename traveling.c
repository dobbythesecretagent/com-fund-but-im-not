#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define main_file "traveling.csv"
#define copy_file "traveling_copy.csv"
#define deleted_file "deleted.csv"
#define max_line 512
#define max_note 200

typedef struct {
    int BookingId;
    char Name[100];
    char City[100];
    char Duration[50];
    int Day;
    int Month;
    int Year;
} note;

note records[max_note];
int note_count = 0;
int LastBookingId = 0;

char *months[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

// ===== utility =====
int current_year() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

int valid_date(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1) return 0;
    int days_in_month[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) days_in_month[2] = 29;
    if (day > days_in_month[month]) return 0;
    return 1;
}

int valid_number(const char *s) {
    if (strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

// ===== file management =====
void create_copy_csv() {
    FILE *src = fopen(main_file, "r");
    FILE *dst = fopen(copy_file, "w");
    if (!src) {
        // ถ้าไม่มี traveling.csv ให้สร้างไฟล์เปล่า
        dst = fopen(main_file, "w");
        fclose(dst);
        src = fopen(main_file, "r");
        dst = fopen(copy_file, "w");
    }
    if (!src || !dst) {
        printf("error creating copy file.\n");
        if (src) fclose(src);
        if (dst) fclose(dst);
        return;
    }
    char ch;
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, dst);
    fclose(src);
    fclose(dst);
}

void load_csv() {
    FILE *file = fopen(copy_file, "r");
    if (!file) {
        printf("no data found. starting fresh.\n");
        return;
    }
    note_count = 0;
    char line[max_line];
    while (fgets(line, sizeof(line), file)) {
        note r;
        if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d-%d-%d",
                   &r.BookingId, r.Name, r.City, r.Duration,
                   &r.Day, &r.Month, &r.Year) == 7) {
            records[note_count++] = r;
            if (r.BookingId > LastBookingId) LastBookingId = r.BookingId;
        }
    }
    fclose(file);
}

void save_csv() {
    FILE *file = fopen(copy_file, "w");
    if (!file) {
        printf("can't write to file\n");
        return;
    }
    for (int i = 0; i < note_count; i++) {
        fprintf(file, "%d,%s,%s,%s,%02d-%02d-%04d\n",
                records[i].BookingId, records[i].Name,
                records[i].City, records[i].Duration,
                records[i].Day, records[i].Month, records[i].Year);
    }
    fclose(file);
}

// ===== features =====
void display_all() {
    if (note_count == 0) {
        printf("no records found.\n");
        return;
    }
    printf("\n--- all travel records ---\n");
    for (int i = 0; i < note_count; i++) {
        printf("%d) BookingId: %d | Name: %s | City: %s | Duration: %s days | Date: %02d-%s-%04d\n",
               i + 1, records[i].BookingId, records[i].Name,
               records[i].City, records[i].Duration,
               records[i].Day, months[records[i].Month], records[i].Year);
    }
}

int search(char *term) {
    int found = 0;
    printf("\n--- search results for \"%s\" ---\n", term);
    for (int i = 0; i < note_count; i++) {
        char date_str[20];
        sprintf(date_str, "%02d-%02d-%04d", records[i].Day, records[i].Month, records[i].Year);

        if (strstr(records[i].Name, term) ||
            strstr(records[i].City, term) ||
            strstr(records[i].Duration, term) ||
            strstr(date_str, term)) {
            printf("%d) BookingId: %d | Name: %s | City: %s | Duration: %s days | Date: %02d-%s-%04d\n",
                   i + 1, records[i].BookingId, records[i].Name,
                   records[i].City, records[i].Duration,
                   records[i].Day, months[records[i].Month], records[i].Year);
            found++;
        }
    }
    if (!found) printf("no records found\n");
    return found;
}

void add_record() {
    if (note_count >= max_note) {
        printf("can't add more records\n");
        return;
    }
    note r;
    int cy = current_year();
    r.BookingId = ++LastBookingId;
    printf("enter name: "); scanf(" %[^\n]", r.Name);
    printf("enter city: "); scanf(" %[^\n]", r.City);

    do {
        printf("enter duration (days): ");
        scanf(" %[^\n]", r.Duration);
        if (!valid_number(r.Duration)) {
            printf("invalid duration. please enter a number only.\n");
        } else break;
    } while (1);

    do {
        printf("enter date: "); scanf("%d", &r.Day);
        printf("enter month: "); scanf("%d", &r.Month);
        printf("enter year (can only record for the past 10 years or later): "); scanf("%d", &r.Year);

        if (r.Year > 2400) r.Year -= 543;

        if (!valid_date(r.Day, r.Month, r.Year)) {
            printf("invalid date. try again.\n");
        } else if (r.Year < cy - 10 || r.Year > cy + 10) {
            printf("year must be within %d to %d (AD). try again.\n", cy - 10, cy + 10);
        } else {
            break;
        }
    } while (1);

    printf("\n--- please review your entry ---\n");
    printf("name: %s\ncity: %s\nduration: %s days\ndate: %02d-%s-%04d\n",
           r.Name, r.City, r.Duration, r.Day, months[r.Month], r.Year);
    char confirm;
    printf("save this record? (y/n): ");
    scanf(" %c", &confirm);
    if (tolower(confirm) != 'y') {
        printf("record discarded.\n");
        return;
    }

    records[note_count++] = r;
    save_csv();
    printf("record added successfully (Booking Id = %d)\n", r.BookingId);
}

void save_deleted(note r) {
    FILE *file = fopen(deleted_file, "a");
    if (!file) return;
    fprintf(file, "%d,%s,%s,%s,%02d-%02d-%04d\n",
            r.BookingId, r.Name, r.City, r.Duration,
            r.Day, r.Month, r.Year);
    fclose(file);
}

void delete_record() {
    char term[100];
    printf("search record to delete: ");
    scanf(" %[^\n]", term);
    int found = search(term);
    if (!found) return;
    int idx;
    printf("enter index to delete: "); scanf("%d", &idx);
    idx--;
    if (idx < 0 || idx >= note_count) return;
    char confirm;
    printf("confirm delete Booking Id %d? (y/n): ", records[idx].BookingId);
    scanf(" %c", &confirm);
    if (tolower(confirm) == 'y') {
        save_deleted(records[idx]);
        for (int i = idx; i < note_count - 1; i++)
            records[i] = records[i + 1];
        note_count--;
        save_csv();
        printf("record deleted.\n");
    } else {
        printf("delete canceled.\n");
    }
}

void restore_record() {
    int bid;
    printf("enter Booking Id to restore: "); scanf("%d", &bid);
    FILE *file = fopen(deleted_file, "r");
    if (!file) { printf("no deleted records found.\n"); return; }
    note r;
    char line[max_line];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d-%d-%d",
                   &r.BookingId, r.Name, r.City, r.Duration, &r.Day, &r.Month, &r.Year) == 7) {
            if (r.BookingId == bid) { found = 1; break; }
        }
    }
    fclose(file);
    if (!found) { printf("record not found.\n"); return; }
    char confirm;
    printf("restore record %d (%s, %s, %s days, %02d-%s-%04d)? (y/n): ",
           r.BookingId, r.Name, r.City, r.Duration, r.Day, months[r.Month], r.Year);
    scanf(" %c", &confirm);
    if (tolower(confirm) == 'y') {
        records[note_count++] = r;
        save_csv();
        printf("record restored.\n");
    } else {
        printf("restore canceled.\n");
    }
}

// ===== main =====
int main() {
    create_copy_csv();
    load_csv();
    int choice;
    while (1) {
        printf("\n===== travel record system =====\n");
        printf("1. display all records\n");
        printf("2. search records\n");
        printf("3. add record\n");
        printf("4. delete record\n");
        printf("5. restore deleted record\n");
        printf("6. exit\n");
        printf("enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: display_all(); break;
            case 2: {
                char term[100];
                do {
                    printf("enter search term (type 'x' to stop): ");
                    scanf(" %[^\n]", term);
                    if (strcmp(term, "x") == 0) break;
                    search(term);
                } while (1);
                break;
            }
            case 3: add_record(); break;
            case 4: delete_record(); break;
            case 5: restore_record(); break;
            case 6: {
                char confirm;
                printf("are you sure you want to exit? (y/n): ");
                scanf(" %c", &confirm);
                if (tolower(confirm) == 'y') {
                    remove(copy_file);
                    remove(deleted_file);
                    printf("thank you for using our traveling record service!\n");
                    return 0;
                }
                break;
            }
            default: printf("invalid choice\n");
        }
    }
}
