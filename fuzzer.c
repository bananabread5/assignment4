
#include <stdio.h>
#include <string.h>

#define SEATS 24

// Same structure used in the main program
struct seat {
    int seat_id;
    int assignment;
    char last_name[50];
    char first_name[50];
};

// Create a normal flight data file
void create_normal_file(void) {
    struct seat outbound[SEATS];
    struct seat inbound[SEATS];

    FILE *file;
    int i;

    for (i = 0; i < SEATS; i++) {
        outbound[i].seat_id = i + 1;
        outbound[i].assignment = 0;
        outbound[i].first_name[0] = '\0';
        outbound[i].last_name[0] = '\0';

        inbound[i].seat_id = i + 1;
        inbound[i].assignment = 0;
        inbound[i].first_name[0] = '\0';
        inbound[i].last_name[0] = '\0';
    }

    file = fopen("flight_data.bin", "wb");

    if (file == NULL) {
        printf("Error creating flight_data.bin\n");
        return;
    }

    fwrite(outbound, sizeof(struct seat), SEATS, file);
    fwrite(inbound, sizeof(struct seat), SEATS, file);

    fclose(file);

    printf("Created normal flight_data.bin\n");
}


// 1. Create a truncated file
void create_truncated_file(void) {
    struct seat seats[10];
    FILE *file;
    int i;

    for (i = 0; i < 10; i++) {
        seats[i].seat_id = i + 1;
        seats[i].assignment = 0;
        seats[i].first_name[0] = '\0';
        seats[i].last_name[0] = '\0';
    }

    file = fopen("corrupt_truncated.bin", "wb");

    if (file == NULL) {
        printf("Error creating truncated file.\n");
        return;
    }

    // Only write 10 seats instead of 48
    fwrite(seats, sizeof(struct seat), 10, file);

    fclose(file);

    printf("Created corrupt_truncated.bin\n");
}


// 2. Create an oversized file
void create_oversized_file(void) {
    FILE *original;
    FILE *corrupt;
    int c;

    original = fopen("flight_data.bin", "rb");

    if (original == NULL) {
        printf("Could not open flight_data.bin.\n");
        return;
    }

    corrupt = fopen("corrupt_oversized.bin", "wb");

    if (corrupt == NULL) {
        printf("Could not create oversized file.\n");
        fclose(original);
        return;
    }

    // Copy the normal file
    while ((c = fgetc(original)) != EOF) {
        fputc(c, corrupt);
    }

    // Add extra garbage bytes
    fputc(0xFF, corrupt);
    fputc(0xFF, corrupt);
    fputc(0xAA, corrupt);
    fputc(0xBB, corrupt);
    fputc(0xCC, corrupt);

    fclose(original);
    fclose(corrupt);

    printf("Created corrupt_oversized.bin\n");
}


// 3. Create garbage data inside a passenger name
void create_garbage_file(void) {
    struct seat outbound[SEATS];
    struct seat inbound[SEATS];

    FILE *file;

    // Start with normal data
    int i;

    for (i = 0; i < SEATS; i++) {
        outbound[i].seat_id = i + 1;
        outbound[i].assignment = 0;
        outbound[i].first_name[0] = '\0';
        outbound[i].last_name[0] = '\0';

        inbound[i].seat_id = i + 1;
        inbound[i].assignment = 0;
        inbound[i].first_name[0] = '\0';
        inbound[i].last_name[0] = '\0';
    }

    // Assign a passenger to the first seat
    outbound[0].assignment = 1;
    strcpy(outbound[0].first_name, "John");

    // Put non-printable characters in the last name
    outbound[0].last_name[0] = 'S';
    outbound[0].last_name[1] = 'm';
    outbound[0].last_name[2] = 'i';
    outbound[0].last_name[3] = 't';
    outbound[0].last_name[4] = '\xFF';
    outbound[0].last_name[5] = '\x01';
    outbound[0].last_name[6] = '\xFE';
    outbound[0].last_name[7] = '\0';

    file = fopen("corrupt_garbage.bin", "wb");

    if (file == NULL) {
        printf("Error creating garbage file.\n");
        return;
    }

    fwrite(outbound, sizeof(struct seat), SEATS, file);
    fwrite(inbound, sizeof(struct seat), SEATS, file);

    fclose(file);

    printf("Created corrupt_garbage.bin\n");
}


// 4. Create invalid seat numbers and assignment values
void create_invalid_values_file(void) {
    struct seat outbound[SEATS];
    struct seat inbound[SEATS];

    FILE *file;
    int i;

    for (i = 0; i < SEATS; i++) {
        outbound[i].seat_id = i + 1;
        outbound[i].assignment = 0;
        outbound[i].first_name[0] = '\0';
        outbound[i].last_name[0] = '\0';

        inbound[i].seat_id = i + 1;
        inbound[i].assignment = 0;
        inbound[i].first_name[0] = '\0';
        inbound[i].last_name[0] = '\0';
    }

    // Corrupt the first outbound seat
    outbound[0].seat_id = 999;
    outbound[0].assignment = 999;

    file = fopen("corrupt_values.bin", "wb");

    if (file == NULL) {
        printf("Error creating invalid values file.\n");
        return;
    }

    fwrite(outbound, sizeof(struct seat), SEATS, file);
    fwrite(inbound, sizeof(struct seat), SEATS, file);

    fclose(file);

    printf("Created corrupt_values.bin\n");
}


// Main function
int main(void) {

    printf("Colossus Airlines File Fuzzer\n");
    printf("------------------------------\n");

    create_normal_file();
    create_truncated_file();
    create_oversized_file();
    create_garbage_file();
    create_invalid_values_file();

    printf("\nAll test files have been created.\n");

    return 0;
}

