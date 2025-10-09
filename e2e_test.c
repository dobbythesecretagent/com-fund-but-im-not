#include "traveling.h"

void run_e2e_test() {
    printf("\n===== running E2E test =====\n");

    note_count = 0;
    LastBookingId = 0;
    remove(copy_file);
    remove(deleted_file);
    create_copy_csv();

    note new_rec = {++LastBookingId, "TestUser", "Tokyo", "5", 10, 10, 2025};
    records[note_count++] = new_rec;
    save_csv();
    printf("[E2E] added note  (BookingId=%d)\n", new_rec.BookingId);

    save_deleted(new_rec);
    note_count--;
    save_csv();
    printf("[E2E] deleted note (BookingId=%d)\n", new_rec.BookingId);

    FILE *file = fopen(deleted_file, "r");
    if (file) {
        char line[max_line];
        note r;
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d-%d-%d",
                       &r.BookingId, r.Name, r.City, r.Duration,
                       &r.Day, &r.Month, &r.Year) == 7) {
                records[note_count++] = r;
                break;
            }
        }
        fclose(file);
    }
    save_csv();
    printf("[E2E] restored note\n");

    int found = search("Tokyo");
    printf("[E2E] search result: %d record(s) found\n", found);

    if (found > 0)
        printf("E2E test PASSED!\n");
    else
        printf("E2E test FAILED!\n");

    printf("===== end of E2E test =====\n");
}