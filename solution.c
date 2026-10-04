#include <stdio.h>
#define SEATS 24

//creating the structure for seat
struct seat {
    int seat_id;
    int assignment;
    char last_name[50];
    char first_name[50];
};

//making arrays
struct seat outbound[SEATS]; 
struct seat inbound[SEATS];

//clear leftover characters
void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//giving each seat a number
void initailize(void) {
    for (int i = 0; i < SEATS; i++) {
        outbound[i].seat_id = i + 1;
        outbound[i].assignment = 0;

        inbound[i].seat_id = i + 1;
        inbound[i].assignment = 0;
    }
}

//Begining assignment 4
//verifying that the flight data isnt currupted
int validate_flight(struct seat flight[]) {
    for (int i = 0; i < SEATS; i++) {

        //checking seat number data
        if (flight[i].seat_id < 1 || flight[i].seat_id > SEATS) {
            return 0;
        }

        //checking assignment data
        if (flight[i].assignment != 0 && flight[i].assignment != 1) {
            return 0;
        }

        //checking first name ends with \0
        int first_valid = 0;
        for (int j = 0; j < 50; j++) {
            if (flight[i].first_name[j] == '\0') {
                first_valid = 1;
                break;
            }
        }

        if (!first_valid) {
            return 0;
        }

        //checking last name ends with \0
        int last_valid = 0;
        for (int j = 0; j < 50; j++) {
            if (flight[i].last_name[j] == '\0') {
                last_valid = 1;
                break;
            }
        }

        if (!last_valid) {
            return 0;
        }

        //checking for invalide characters in the first name
        for (int j = 0; flight[i].first_name[j] != '\0'; j++) {
            if (flight[i].first_name[j] < 32 ||
                flight[i].first_name[j] > 126) {
                return 0;
            }
        }

        //checking for invalid characters in the last name
        for (int j = 0; flight[i].last_name[j] != '\0'; j++) {
            if (flight[i].last_name[j] < 32 ||
                flight[i].last_name[j] > 126) {
                return 0;
            }
        }
    }

    return 1;
}

//loading previous data and verifying that the file is the correct size
void load_data(void) {
    FILE *file;
    long expected_size;
    long actual_size;


    file = fopen("flight_data.bin", "rb");

    if (file == NULL) {
        initailize();
        return;
    }

    expected_size = sizeof(struct seat) * SEATS * 2;

    //checking file size
    if (fseek(file, 0, SEEK_END) !=0) {
        printf("Error checking file size.\n");
        fclose(file);
        initailize();
        return;
    }

    actual_size = ftell(file);

    if (actual_size != expected_size) {
        printf("Error loading file. File size is incorrect or currupted.\n");
        fclose(file);
        initailize();
        return;
    }

    rewind(file);

    //reading outbound seat data
    if (fread(outbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Error loading outbound flight data, clearing seats.\n");
        fclose(file);
        initailize();
        return;
    }

    //reading inbound seat data
    if (fread(inbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Error loading inbound data, clearing seats\n");
        fclose(file);
        initailize();
        return;
    }

    //verifying all the data is valid
    if (!validate_flight(outbound) || !validate_flight(inbound)) {
        printf("File contains invalid or currupted data.\n");
        fclose(file);
        initailize();
        return;
    }

    if (fclose(file) != 0) {
        printf("Error closing data.\n");
    }

    printf("Previous flight log data loaded!\n");
}


//saving seat information
void save_data(void) {
    FILE *file = fopen("flight_data.bin", "wb");

    if (file == NULL) {
        printf("Unable to load data\n");
        return;
    }

    if (fwrite(outbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Unable to save outbound flight data\n");
        fclose(file);
        return;
    }

    if (fwrite(inbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Unable to save inbound flight data\n");
        fclose(file);
        return;
    }

    if (fclose(file) != 0) {
        printf("Error closing data\n");
        return;
    }

    printf("Flight log data saved!\n");
}


//showing empty seats
void empty_seats(struct seat flight[]) {
    int count = 0; 
    for (int i = 0; i < SEATS; i++) 
        if (!flight[i].assignment) 
            count++; 
    printf("Number of empty seats: %d\n", count);
}

//showing list of empty seat
void empty_seats_listed(struct seat flight[]) {
    printf("empty seats: ");
    for (int i = 0; i < SEATS; i++)
        if (!flight[i].assignment)
            printf("%d ", flight[i].seat_id); 
    printf("\n");
}

//assigning a passenger to a seat
void add_passenger(struct seat flight[]) {
    char first_name[50];
    char last_name[50];
    int seat;
    printf("Please enter a seat number you would like, or enter 25 to cancel: ");
    if (scanf("%d", &seat) != 1) {
        printf("Not a number.\n");
        clear_buffer();
        return;
    }

    if (seat == 25) {
        clear_buffer();
        return;
    }

    if (seat < 1 || seat > 24) { 
        printf("Invalid seat.\n");
        clear_buffer();
        return;
    }

    if (flight[seat - 1].assignment == 1) {
        printf("That seat is taken.\n");
        return;
    }

    printf("Enter your first name: ");
    scanf(" %49[^\n]", flight[seat - 1].first_name);
    if (getchar() != '\n') {
        printf("name is too long. Shorten it down to below 50 characters");
        clear_buffer();
    }

    if (flight[seat - 1].first_name[0] == '-' && 
        flight[seat - 1].first_name[1] == '1' && 
        flight[seat - 1].first_name[2] == '\0') { 
            printf("Entry aborted.\n"); 
            return;
    }

    printf("Enter your last name: ");
    scanf(" %49[^\n]", flight[seat - 1].last_name);
    if (getchar() != '\n') {
        printf("name is too long. Shorten it down to below 50 characters");
        clear_buffer();
    }

    if (flight[seat - 1].last_name[0] == '-' && 
        flight[seat - 1].last_name[1] == '1' && 
        flight[seat - 1].last_name[2] == '\0') { 
            printf("Entry aborted.\n"); 
            return;
    }

    flight[seat - 1].assignment = 1;
    printf("the seat has been assigned to you!\n");
    
}

//removing a passenger from the list
void remove_passenger(struct seat flight[]) {
    int seat;
    printf("Please enter a seat number you would like, or enter 25 to cancel: ");
    if (scanf("%d", &seat) != 1){
        printf("Invalid selection.\n"); 
        clear_buffer(); 
        return;
    }

    if (seat == 25) {
        clear_buffer();
        return;
    }

    if (seat < 1 || seat > 24) {
        printf("Invalid seat.\n");
        clear_buffer();
        return;
    }

    if (!flight[seat - 1].assignment) {
        printf("That seat is empty");
        return;
    }

    flight[seat - 1].assignment = 0; 
    flight[seat - 1].first_name[0] = '\0'; 
    flight[seat - 1].last_name[0] = '\0'; 
    printf("Seat assignment deleted.\n"); 
}

//alphabetical list of names
void names_listed(struct seat flight[]) {
    int i, j, y, temp;
    int order[SEATS];
    for (i = 0; i < SEATS; i++) 
        order[i] = i;
    for (i = 0; i < SEATS - 1; i++) {
        for (j = i + 1; j < SEATS; j++) {
            if (flight[order[i]].last_name[0] > flight[order[j]].last_name[0]) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }   
        }
    }

    printf("Alphabetical list of names:\n");
    for (i = 0; i < SEATS; i++) {
        int y = order[i];
        if (flight[y].assignment)
            printf("%s, %s - Seat %d\n",
                flight[y].last_name,
                flight[y].first_name,
                flight[y].seat_id);
            }
}   

//making outbound menu
void O_menu(void) {
    char choice;

    while (1) {
        printf(" A) Show number of seats that are empty \n");
        printf(" B) Show list of the seats that are empty \n");
        printf(" C) Show list of seats in alphabetical order \n");
        printf(" D) Assign a passenger to a seat \n");
        printf(" E) Remove an assigned seat \n");
        printf(" F) Return to the main menu \n");

        printf("Please select an option: ");
        if (scanf(" %c", &choice) != 1) 
            return;
        

        switch(choice) {
            case 'a':
                empty_seats(outbound);
                break;
            
            case 'b':
                empty_seats_listed(outbound);
                break;

            case 'c':
                names_listed(outbound);
                break;
            
            case 'd':
                add_passenger(outbound);
                break;

            case 'e':
                remove_passenger(outbound);
                break;

            case 'f':
                return;

            default: 
                printf("Error: you can only choose a-f.");
                break;
            
        }
    }
}

//making inbound menu
void I_menu(void) {
    char choice;

    while (1) {
        printf(" A) Show number of seats that are empty \n");
        printf(" B) Show list of the seats that are empty \n");
        printf(" C) Show list of seats in alphabetical order \n");
        printf(" D) Assign a passenger to a seat \n");
        printf(" E) Remove an assigned seat \n");
        printf(" F) Return to the main menu \n");

        printf("Please select an option: ");
        if (scanf(" %c", &choice) != 1) 
            return;
        

        switch(choice) {
            case 'a':
                empty_seats(inbound);
                break;

            case 'b':
                empty_seats_listed(inbound);
                break;

            case 'c':
                names_listed(inbound);
                break;

            case 'd':
                add_passenger(inbound);
                break;

            case 'e':
                remove_passenger(inbound);
                break;

            case 'f':
                return;

            default: 
                printf("Error: you can only choose a-f.");
                break;
            
        }
    }
}

//making the first menu
void first_menu(void) {
    char choice;

    while (1) {

        printf("First-Level Menu: \n");
        printf(" A) Outbound Flight \n");
        printf(" B) Inbound Flight \n");
        printf(" C) Quit \n");

        printf("Please select an option: ");
        if (scanf(" %c", &choice) != 1) {
            return;
        }

        switch(choice) {
            case 'a':
                O_menu();
                break;

            case 'b':
                I_menu();
                break;

            case 'c':
                save_data();
                printf("Quiting!\n");
                return;

            default: {
                printf("Error: you can only choose a, b, or c.\n");
                break;
            }
        }
    }
}

//running the program
int main() {
    load_data();
    first_menu();
    return 0;
}
