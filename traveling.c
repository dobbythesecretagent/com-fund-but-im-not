#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define main_file "traveling.csv"
#define deleted_file "deleted.csv"
#define backup_file "backup.csv"
#define max_line 512
#define max_rec 200

typedef struct {
    int bookingid;
    char name[100];
    char city[100];
    char duration[50];
    int day;
    int month;
    int year;
} record;

record records[max_rec];
int record_count = 0;
int lastbookingid = 0;

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

// ===== file management =====
void backup_csv() {
    FILE *src = fopen(main_file, "r");
    FILE *dst = fopen(backup_file, "w");
    if (!src || !dst) return;
    char ch;
    while ((ch = fgetc(src)) != EOF) fputc(ch, dst);
    fclose(src);
    fclose(dst);
}

void restore_csv() {
    FILE *src = fopen(backup_file, "r");
    FILE *dst = fopen(main_file, "w");
    if (!src || !dst) return;
    char ch;
    while ((ch = fgetc(src)) != EOF) fputc(ch, dst);
    fclose(src);
    fclose(dst);
    remove(backup_file);
}

// ===== core I/O =====
void load_csv() {
    FILE *file = fopen(main_file, "r");
    if (!file) {
        printf("no existing data file found (%s). starting fresh.\n", main_file);
        return;
    }
    record_count = 0;
    char line[max_line];
    while (fgets(line, sizeof(line), file)) {
        record r;
        if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d-%d-%d",
                   &r.bookingid, r.name, r.city, r.duration,
                   &r.day, &r.month, &r.year) == 7) {
            records[record_count++] = r;
            if (r.bookingid > lastbookingid) lastbookingid = r.bookingid;
        }
    }
    fclose(file);
}

void save_csv() {
    FILE *file = fopen(main_file, "w");
    if (!file) {
        printf("cannot write to file\n");
        return;
    }
    for (int i = 0; i < record_count; i++) {
        fprintf(file, "%d,%s,%s,%s,%02d-%02d-%04d\n",
                records[i].bookingid, records[i].name,
                records[i].city, records[i].duration,
                records[i].day, records[i].month, records[i].year);
    }
    fclose(file);
}

// ===== features =====
void display_all() {
    if (record_count == 0) {
        printf("no records found.\n");
        return;
    }
    printf("\n--- all travel records ---\n");
    for (int i = 0; i < record_count; i++) {
        printf("%d) bookingid: %d | name: %s | city: %s | duration: %s | date: %02d-%s-%04d\n",
               i + 1, records[i].bookingid, records[i].name,
               records[i].city, records[i].duration,
               records[i].day, months[records[i].month], records[i].year);
    }
}

int search(char *term) {
    int found = 0;
    printf("\n--- search results for \"%s\" ---\n", term);
    for (int i = 0; i < record_count; i++) {
        char date_str[20];
        sprintf(date_str, "%02d-%02d-%04d", records[i].day, records[i].month, records[i].year);

        if (strstr(records[i].name, term) ||
            strstr(records[i].city, term) ||
            strstr(records[i].duration, term) ||
            strstr(date_str, term)) {
            printf("%d) bookingid: %d | name: %s | city: %s | duration: %s | date: %02d-%s-%04d\n",
                   i + 1, records[i].bookingid, records[i].name,
                   records[i].city, records[i].duration,
                   records[i].day, months[records[i].month], records[i].year);
            found++;
        }
    }
    if (!found) printf("no records found\n");
    return found;
}

void add_record() {
    if (record_count >= max_rec) {
        printf("cannot add more records\n");
        return;
    }
    record r;
    int cy = current_year();
    r.bookingid = ++lastbookingid;
    printf("enter name: "); scanf(" %[^\n]", r.name);
    printf("enter city: "); scanf(" %[^\n]", r.city);
    printf("enter duration: "); scanf(" %[^\n]", r.duration);

    do {
        printf("enter day: "); scanf("%d", &r.day);
        printf("enter month: "); scanf("%d", &r.month);
        printf("enter year (BE or AD): "); scanf("%d", &r.year);
        if (r.year > 2500) r.year -= 543;
        if (!valid_date(r.day, r.month, r.year))
            printf("invalid date. try again.\n");
        else if (r.year < cy - 10 || r.year > cy + 10)
            printf("year must be within %d to %d. try again.\n", cy - 10, cy + 10);
        else break;
    } while (1);

    records[record_count++] = r;
    save_csv();
    printf("record added successfully (bookingid = %d)\n", r.bookingid);
}

void edit_record() {
    char term[100];
    printf("search record to edit: ");
    scanf(" %[^\n]", term);
    int found = search(term);
    if (!found) {
        int choice;
        printf("no record found. do you want to (1)add new or (2)search again? : ");
        scanf("%d", &choice);
        if (choice == 1) add_record();
        else edit_record();
        return;
    }
    int idx;
    printf("enter index to edit: ");
    scanf("%d", &idx);
    idx--;
    if (idx < 0 || idx >= record_count) return;

    int opt;
    printf("choose field: 1) date 2) name 3) city 4) duration : ");
    scanf("%d", &opt);
    switch (opt) {
        case 1: {
            int cy = current_year();
            do {
                printf("enter new day: "); scanf("%d", &records[idx].day);
                printf("enter new month: "); scanf("%d", &records[idx].month);
                printf("enter new year (BE or AD): "); scanf("%d", &records[idx].year);
                if (records[idx].year > 2500) records[idx].year -= 543;
                if (!valid_date(records[idx].day, records[idx].month, records[idx].year))
                    printf("invalid date.\n");
                else if (records[idx].year < cy - 10 || records[idx].year > cy + 10)
                    printf("year must be within %d to %d.\n", cy - 10, cy + 10);
                else break;
            } while (1);
            break;
        }
        case 2: printf("new name: "); scanf(" %[^\n]", records[idx].name); break;
        case 3: printf("new city: "); scanf(" %[^\n]", records[idx].city); break;
        case 4: printf("new duration: "); scanf(" %[^\n]", records[idx].duration); break;
        default: printf("invalid choice\n"); return;
    }
    save_csv();
    printf("record updated successfully\n");
}

void save_deleted(record r) {
    FILE *file = fopen(deleted_file, "a");
    if (!file) return;
    fprintf(file, "%d,%s,%s,%s,%02d-%02d-%04d\n",
            r.bookingid, r.name, r.city, r.duration,
            r.day, r.month, r.year);
    fclose(file);
}

void delete_record() {
    char term[100];
    printf("search record to delete: ");
    scanf(" %[^\n]", term);
    int found = search(term);
    if (!found) return;
    int idx, confirm;
    printf("enter index to delete: "); scanf("%d", &idx);
    idx--;
    if (idx < 0 || idx >= record_count) return;
    printf("confirm delete bookingid %d? (1=yes/0=no): ", records[idx].bookingid);
    scanf("%d", &confirm);
    if (confirm) {
        save_deleted(records[idx]);
        for (int i = idx; i < record_count - 1; i++)
            records[i] = records[i + 1];
        record_count--;
        save_csv();
        printf("record deleted.\n");
    }
}

void restore_record() {
    int bid;
    printf("enter bookingid to restore: "); scanf("%d", &bid);
    FILE *file = fopen(deleted_file, "r");
    if (!file) { printf("no deleted records found.\n"); return; }
    record r;
    char line[max_line];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d-%d-%d",
                   &r.bookingid, r.name, r.city, r.duration, &r.day, &r.month, &r.year) == 7) {
            if (r.bookingid == bid) { found = 1; break; }
        }
    }
    fclose(file);
    if (!found) { printf("record not found.\n"); return; }
    printf("restore record %d (%s, %s, %s, %02d-%s-%04d)? (1=yes/0=no): ",
           r.bookingid, r.name, r.city, r.duration, r.day, months[r.month], r.year);
    int confirm; scanf("%d", &confirm);
    if (confirm) {
        records[record_count++] = r;
        save_csv();
        printf("record restored.\n");
    }
}

// ===== main =====
int main() {
    backup_csv();
    load_csv();
    int choice;
    while (1) {
        printf("\n===== travel record system =====\n");
        printf("1. display all records\n");
        printf("2. search records\n");
        printf("3. add record\n");
        printf("4. edit record\n");
        printf("5. delete record\n");
        printf("6. restore deleted record\n");
        printf("7. exit\n");
        printf("enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: display_all(); break;
            case 2: {
                char term[100];
                do {
                    printf("enter search term (type exit to stop): ");
                    scanf(" %[^\n]", term);
                    if (strcmp(term, "exit") == 0) break;
                    search(term);
                } while (1);
                break;
            }
            case 3: add_record(); break;
            case 4: edit_record(); break;
            case 5: delete_record(); break;
            case 6: restore_record(); break;
            case 7: {
                int confirm;
                printf("are you sure you want to exit? (1=yes / 0=no): ");
                scanf("%d", &confirm);
                if (confirm) {
                    restore_csv();
                    printf("all changes reverted — goodbye!\n");
                    return 0;
                }
                break;
            }
            default: printf("invalid choice\n");
        }
    }
}
