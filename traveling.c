#include "traveling.h"

int main() {
    create_copy_csv();
    load_csv();
    int choice;
    while (1) {
        printf("\n===== travel note system =====\n");
        printf("1. display all notes\n");
        printf("2. search notes\n");
        printf("3. add note\n");
        printf("4. delete note\n");
        printf("5. restore deleted note\n");
        printf("6. exit\n");
        printf("7. run unit tests\n");
        printf("8. run E2E test\n");
        printf("enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: display_all(); break;
            case 2: {
                char term[100];
                input_string("enter search term: ", term, sizeof(term));
                search(term);
                break;
            }
            case 3: add_note(); break;
            case 4: delete_note(); break;
            case 5: restore_note(); break;
            case 6: return 0;
            case 7: run_unit_tests(); break;
            case 8: run_e2e_test(); break;
            default: printf("invalid choice\n");
        }
    }
}
