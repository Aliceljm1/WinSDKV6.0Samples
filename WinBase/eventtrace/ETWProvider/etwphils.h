#ifndef ETWPHILS
#define ETWPHILS

#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <strsafe.h>
#include <guiddef.h>

#include <wmistr.h>
#include <initguid.h>


#include <evntrace.h>
#include <evntprov.h>


#define NUM_PHILOSOPHERS (7)    // Total number of philosophers
#define EAT_COUNT (1000)        // Number of times to eat
#define EATS_PER_DISCUSS (100)  // Number of eating periods before stopping to discuss
#define NO_ONE ((ULONG) -1)     // Nobody

#define THINK ((ULONG) 0)       // Flag for thinking
#define EAT ((ULONG) 1)         // Flag for eating
#define DISCUSS ((ULONG) -1)    // Flag for discussing

#define ALL_DISCUSS (FALSE)     // Bool to wait for all philosophers to have a discussion
#define NUM_DISCUSS (2)         // How many philosophers (only 2 is currently supported)
#define DISCUSS_WAIT (8000L)    // How long to wait for a discussion before aborting 
                                //     (ignored if ALL_DISCUSS == TRUE)

#define TIME_ORD_MAG (100)      // Order of magnitude limit otime for events (in ms)
#define TIME_DELAY (2000)       // Time to wait before starting and stopping

// Provider GUID 7d89fdc9-974b-4cc8-a8d9-eec0724216ed - In manifest, copied from there
/* static const GUID ETWPhilsGUID = {0x7d89fdc9,
                                    0x974b,
                                    0x4cc8,
                                    {0xa8, 0xd9, 0xee, 0xc0, 0x72, 0x42, 0x16, 0xed}};
*/

//
// Chopstick object, containing a semaphore for locking, and who's currently using it.
//
typedef struct chopstick {
    ULONG User;                                 // Id for user
    REGHANDLE RegHandle;                        // Handle for logging 
    HANDLE EatLock;                             // Semaphore for locking
} CHOPSTICK;

//
// Philosopher object, containing an id, a semaphore for locking, and chopsticks to use.
//
typedef struct philosopher {
    ULONG Id;
    ULONG Meals;                                  // Number of meals eaten
    ULONG Thoughts;                               // Number of thoughts thought
    ULONG Discussions;                            // Number of discussions taken part in

    ULONG DiscussionLength;                       // How long to wait for this discussion 
    ULONG DiscussionSize;                         // How big is the discussion group?
    ULONG DiscussionTimeout;                       // How long to wait for a timeout

    char EatStr[4];                               // String for logging
    char ThinkStr[6];                             // String for logging
    char DiscussStr[8];                           // String for logging  
    char DiscussWaitStr[12];                      // String for logging  

    REGHANDLE RegHandle;                          // Handle for logging
    GUID ActivityID;                              // Activity ID - Log it for now, as
                                                  //    the tools don't report it yet

    EVENT_DATA_DESCRIPTOR EatDataDesc[2];         // Data Descriptor for eating events
    EVENT_DATA_DESCRIPTOR ThinkDataDesc[3];       // Data Descriptor for thinking events
    EVENT_DATA_DESCRIPTOR DiscussDataDesc[4];     // Data Descriptor for discuss events
    EVENT_DATA_DESCRIPTOR DiscussWaitDataDesc[4]; // Descriptor for discuss wait events
    HANDLE EatLock;                               // Semaphore for locking chopsticks   
    HANDLE DiscussLock;                           // Semaphore for locking discussions
    HANDLE DiscussEvent;                          // Event for discussing
    HANDLE DiscussEvents[NUM_PHILOSOPHERS-1];     // All the other philosopher's events
    CHOPSTICK *Left, *Right;                      // Two Chopsticks for eating
    struct table *Table;                          // Pointer back to table object
} PHILOSOPHER;


// Table keeps track of things that are available, chopsticks, philosophers, and locks.
typedef struct table {
    ULONG MaxPhilosophers;                        // Maximum number of philosophers
    ULONG MaxMeals;                               // Maximum number of meals
    ULONG MealsPerDiscussion;                     // Meals per discussion
    ULONG AllDiscuss;                             // Bool for all philosophers to discuss
    ULONG NumDiscuss;                             // Num of philosophers per discussion
    ULONG TimeOrdMag;                             // Order of mag for time to sleep (ms)

    ULONG PhilosopherCount;                       // Number of philosophers at the table
    ULONG TotalMeals;                             // Total number of meals
    ULONG TotalThoughts;                          // Total number of thouoghts
    ULONG TotalDiscussions;                       // Discussions (not participants)

    REGHANDLE RegHandle;                          // Handle for logging


    HANDLE EatLock;                               // For atomic pick-up of both chopsticks
    HANDLE DiscussLock;                           // Semaphore for locking discussions
    HANDLE DiscussEvents[NUM_PHILOSOPHERS];       // Events for discussing
    CHOPSTICK* Chopsticks[NUM_PHILOSOPHERS];      // Set of chopsticks
    PHILOSOPHER* Philosophers[NUM_PHILOSOPHERS];  // Set of philosophers
} TABLE;


#endif  // ETWPHILS