#ifndef TRAVELING_H
#define TRAVELING_H

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

extern note records[max_note];
extern int note_count;
extern int LastBookingId;

void create_copy_csv();
void load_csv();
void save_csv();
void display_all();
int search(char *term);
void add_note();
void delete_note();
void restore_note();
void save_deleted(note r);
void input_string(const char *prompt, char *dest, int size);

// test functions
void run_unit_tests();
void run_e2e_test();

#endif
