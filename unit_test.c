#include "traveling.h"

void run_unit_tests() {
    printf("\n===== running unit tests =====\n");

    // test search()
    printf("\n[TEST] search() function\n");
    note test = {999, "Alice", "Paris", "3", 1, 1, 2024};
    records[note_count++] = test;
    int found = search("Paris");
    printf("expected: found >= 1 | got: %d\n", found);
    if (found >= 1) printf("search() test passed!\n");
    else printf("search() test failed!\n");

    // test restore_note()
    printf("\n[TEST] restore_note() function\n");
    FILE *df = fopen(deleted_file, "w");
    fprintf(df, "%d,%s,%s,%s,%02d-%02d-%04d\n",
            1000, "Bob", "London", "5", 10, 10, 2024);
    fclose(df);

    // จำลองการ restore
    FILE *file = fopen(deleted_file, "r");
    if (file) {
        note r;
        char line[max_line];
        int found2 = 0;
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d-%d-%d",
                       &r.BookingId, r.Name, r.City, r.Duration,
                       &r.Day, &r.Month, &r.Year) == 7) {
                if (r.BookingId == 1000) {
                    records[note_count++] = r;
                    found2 = 1;
                    break;
                }
            }
        }
        fclose(file);
        if (found2) printf("restore_note() test passed!\n");
        else printf("restore_note() test failed!\n");
    }

    printf("\n===== unit tests finished =====\n");
}