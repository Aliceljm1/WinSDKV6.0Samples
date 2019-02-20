/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    etwphils.c   

Abstract:

    Simple test program to become accustomed to the ETW APIs, such as the new activity ID
    
    This will implement the dining philosopher's problem with a slight twist. Instead of
    simply thinking and eating (with the necessary mutexes on the chopsicks to eat),
    there will also be "discussions" where a philosopher will wait for other philosophers
    to talk in a group with. 

--*/

#include "etwphils.h"
#include "etwphils.xmlEvents.h"

//
// Initialize a table and all its sub-objects
// 
TABLE* NewTable (REGHANDLE RegHandle,
                 ULONG NumPhilosophers,
                 ULONG NumMeals,
                 ULONG MealsPerDiscussion,
                 ULONG AllDiscuss,
                 ULONG NumDiscuss,
                 ULONG TimeOrdMag)
{
    TABLE *Table = (TABLE *) malloc (sizeof(TABLE));
    EVENT_DATA_DESCRIPTOR TableCreateDataDesc[1];

    ULONG i = 0;
    ULONG j = 0; 
    ULONG k = 0;

    if (Table == NULL) {
        _tprintf(_T("Out of memory for table.\n"));
        return NULL;
    }

    //
    // Setup the event data descriptor for the table creation event
    //
    EventDataDescCreate(&(TableCreateDataDesc[0]),
                        &(Table->PhilosopherCount),
                        sizeof(Table->PhilosopherCount));


    //
    // Initialize the table's fields
    //

    // Semaphores
    Table->EatLock = CreateSemaphore(NULL, 1, 1, NULL);
    Table->DiscussLock = CreateSemaphore(NULL, 1, 1, NULL);

    // Values
    Table->RegHandle = RegHandle;
    Table->MaxPhilosophers = NumPhilosophers;
    Table->MaxMeals = NumMeals;
    Table->MealsPerDiscussion = MealsPerDiscussion;
    Table->AllDiscuss = AllDiscuss;
    Table->NumDiscuss = NumDiscuss;
    Table->TimeOrdMag = TimeOrdMag;    

    Table->PhilosopherCount = 0;
    Table->MaxPhilosophers = NumPhilosophers;
    Table->TotalMeals = 0;
    Table->TotalThoughts = 0;
    Table->TotalDiscussions = 0;

    //
    // Create all Chopstick objects
    //
    for (i = 0; i < NumPhilosophers; i++) {
        Table->Chopsticks[i] = (CHOPSTICK*) malloc(sizeof(CHOPSTICK));      

        if (Table->Chopsticks[i] == NULL) {
            _tprintf(_T("Out of memory for chopstick.\n"));
            return NULL;
        }
        
        Table->Chopsticks[i]->User = NO_ONE;
        Table->Chopsticks[i]->RegHandle = RegHandle;
        Table->Chopsticks[i]->EatLock = CreateSemaphore(NULL, 1, 1, NULL);
    }
    
    //
    // Create all philosopher objects, and link to table.
    //
    for (i = 0; i < NumPhilosophers; i++) {
        Table->Philosophers[i] = (PHILOSOPHER*) malloc(sizeof(PHILOSOPHER));

        if (Table->Philosophers[i] == NULL) {
            _tprintf(_T("Out of memory for philosopher.\n"));
            return NULL;
        }
        
        Table->PhilosopherCount++;

        //
        // Initialize the philosopher's fields
        //

        Table->Philosophers[i]->Id = i;
        Table->Philosophers[i]->Meals = 0;
        Table->Philosophers[i]->Thoughts = 0;
        Table->Philosophers[i]->Discussions = 0;

        
        Table->Philosophers[i]->DiscussionLength = 0;
        Table->Philosophers[i]->DiscussionSize = 
                                    (AllDiscuss ? NumPhilosophers: NumDiscuss);
        Table->Philosophers[i]->DiscussionTimeout =
                            4 * TimeOrdMag * MealsPerDiscussion;


        strcpy_s(Table->Philosophers[i]->EatStr, 4, "Eat");
        strcpy_s(Table->Philosophers[i]->ThinkStr, 6, "Think");
        strcpy_s(Table->Philosophers[i]->DiscussStr, 8, "Discuss");
        strcpy_s(Table->Philosophers[i]->DiscussWaitStr, 12, "DiscussWait");

        Table->Philosophers[i]->RegHandle = RegHandle;
        Table->Philosophers[i]->Table = Table;
        Table->Philosophers[i]->EatLock = Table->EatLock;
        Table->Philosophers[i]->DiscussLock = Table->DiscussLock;
        Table->Philosophers[i]->Left = Table->Chopsticks[i];
        Table->Philosophers[i]->Right = Table->Chopsticks[(i + 1) % NumPhilosophers];
        Table->Philosophers[i]->DiscussEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        Table->DiscussEvents[i] = Table->Philosophers[i]->DiscussEvent;

        
        //
        // Setup the event data descriptors for the philosopher's events
        //

        
        // Eating
        EventDataDescCreate(&(Table->Philosophers[i]->EatDataDesc[0]),
                            (&Table->Philosophers[i]->Id),
                            sizeof(Table->Philosophers[i]->Id));

        EventDataDescCreate(&(Table->Philosophers[i]->EatDataDesc[1]),
                            (Table->Philosophers[i]->EatStr),
                            sizeof(Table->Philosophers[i]->EatStr));


        // Thinking
        EventDataDescCreate(&(Table->Philosophers[i]->ThinkDataDesc[0]),
                            &(Table->Philosophers[i]->Id),
                            sizeof(Table->Philosophers[i]->Id));

        EventDataDescCreate(&(Table->Philosophers[i]->ThinkDataDesc[1]),
                            &(Table->Philosophers[i]->Thoughts),
                            sizeof(Table->Philosophers[i]->Thoughts));

        EventDataDescCreate(&(Table->Philosophers[i]->ThinkDataDesc[2]),
                            (Table->Philosophers[i]->ThinkStr),
                            sizeof(Table->Philosophers[i]->ThinkStr));


        // Discussions
        EventDataDescCreate(&(Table->Philosophers[i]->DiscussDataDesc[0]),
                            &(Table->Philosophers[i]->Id),
                            sizeof(Table->Philosophers[i]->Id));

        EventDataDescCreate(&(Table->Philosophers[i]->DiscussDataDesc[1]),
                            &(Table->Philosophers[i]->DiscussionSize),
                            sizeof(Table->Philosophers[i]->DiscussionSize));

        EventDataDescCreate(&(Table->Philosophers[i]->DiscussDataDesc[2]),
                            &(Table->Philosophers[i]->DiscussionLength),
                            sizeof(Table->Philosophers[i]->DiscussionLength));

        EventDataDescCreate(&(Table->Philosophers[i]->DiscussDataDesc[3]),
                            (Table->Philosophers[i]->DiscussStr),
                            sizeof(Table->Philosophers[i]->DiscussStr));




        EventDataDescCreate(&(Table->Philosophers[i]->DiscussWaitDataDesc[0]),
                            &(Table->Philosophers[i]->Id),
                            sizeof(Table->Philosophers[i]->Id));

        EventDataDescCreate(&(Table->Philosophers[i]->DiscussWaitDataDesc[1]),
                            &(Table->Philosophers[i]->DiscussionSize),
                            sizeof(Table->Philosophers[i]->DiscussionSize));

        EventDataDescCreate(&(Table->Philosophers[i]->DiscussWaitDataDesc[2]),
                            (Table->Philosophers[i]->DiscussWaitStr),
                            sizeof(Table->Philosophers[i]->DiscussWaitStr));

        EventDataDescCreate(&(Table->Philosophers[i]->DiscussWaitDataDesc[3]),
                            &(Table->Philosophers[i]->ActivityID),
                            sizeof(Table->Philosophers[i]->ActivityID));

    }

    //
    // Finally, link all the discuss events for the other philosophers 
    //    (leave out the one for that philosopher).
    //
    for (i = 0; i < NumPhilosophers; i++) {
        k = 0;
        for (j = 0; (j < NumPhilosophers) && (k < (NumPhilosophers - 1)); j++) {
            if (j == i) {
                continue;
            }
            Table->Philosophers[i]->DiscussEvents[k] = 
                            Table->Philosophers[j]->DiscussEvent;
            k++;
        }
    }

    //
    // Write the event for the table creation
    //

    EventWrite(Table->RegHandle,
               &TableCreateEventDesc,
               1,
               TableCreateDataDesc);

    return Table;
}

//
// Free a table object
//
void FreeTable(TABLE* Table) 
{
    ULONG i = 0;
    EVENT_DATA_DESCRIPTOR TableDestroyDataDesc[4];

    //
    // Setup the event data descriptor for the table destruction event
    //

    EventDataDescCreate(&(TableDestroyDataDesc[0]),
                        &(Table->MaxPhilosophers),
                        sizeof(Table->MaxPhilosophers));

    EventDataDescCreate(&(TableDestroyDataDesc[1]),
                        &(Table->TotalMeals),
                        sizeof(Table->TotalMeals));
    
    EventDataDescCreate(&(TableDestroyDataDesc[2]),
                        &(Table->TotalThoughts),
                        sizeof(Table->TotalThoughts));

    EventDataDescCreate(&(TableDestroyDataDesc[3]),
                       &(Table->TotalDiscussions),
                       sizeof(Table->TotalDiscussions));

    // Close the table's semaphores
    CloseHandle(Table->EatLock);  
    CloseHandle(Table->DiscussLock);  
    

    //
    // Free all of the table's chopsticks and philosophers
    //

    for (i = 0; i < Table->MaxPhilosophers; i++) {
        CloseHandle(Table->Chopsticks[i]->EatLock);
        CloseHandle(Table->Philosophers[i]->DiscussEvent);
        free(Table->Chopsticks[i]);
        free(Table->Philosophers[i]);
    }
    
    //
    // Write the table destruction event.
    //

    EventWrite(Table->RegHandle,
               &TableDestroyEventDesc,
               4, TableDestroyDataDesc);

    free(Table);
}


//
// Pick up a chopstick for eating.
//

void TakeChopstick(CHOPSTICK *Chopstick,
                   ULONG User) 
{
    //
    // Wait for chopstick semaphore to become available, then assign to philosopher.
    //
    WaitForSingleObject(Chopstick->EatLock,
                        INFINITE);
    Chopstick->User = User;
}


//
// Put a chopstick back down on the table.
//

void ReleaseChopstick(CHOPSTICK *Chopstick) 
{
    //
    // Remove the assignment from the chopstick, and release the semaphore. 
    //
    Chopstick->User = NO_ONE;
    ReleaseSemaphore(Chopstick->EatLock,
                     1, NULL);
}

//
// Wait a random amount of time, based on the value of the flag.
//

void Wait(PHILOSOPHER *Philosopher, ULONG Flag) 
{
    static ULONG DiscussLeft = 0;
    static ULONG DiscussTime = 0;   
    static LPCRITICAL_SECTION DiscussPeriod = 0;
    

    //
    // Based on the flag, log a start-event, sleep, then log a stop-event.
    //
    switch (Flag) {
        case EAT:

            EventWrite(Philosopher->RegHandle,
                       &EatBeginEventDesc,
                       2, Philosopher->EatDataDesc);

            Sleep(rand() % (Philosopher->Table->TimeOrdMag) + 1);

            EventWrite(Philosopher->RegHandle,
                       &EatEndEventDesc,
                       2, Philosopher->EatDataDesc);

            break;

        case THINK:

            EventWrite(Philosopher->RegHandle,
                       &ThinkBeginEventDesc,
                       3, Philosopher->ThinkDataDesc);

            Sleep(rand() % (2 * Philosopher->Table->TimeOrdMag) + 1);

            EventWrite(Philosopher->RegHandle,
                       &ThinkEndEventDesc,
                       3, Philosopher->ThinkDataDesc);

            break;
        
        case DISCUSS:


            //
            // For a discussion, check to make sure there's one going on
            // (If not, start one), then "discuss" (sleep) for the duration
            //
            WaitForSingleObject(Philosopher->DiscussLock,
                                INFINITE);

            if (DiscussLeft == 0) {
                srand((unsigned) time(NULL));
                DiscussTime = rand() % (3 * Philosopher->Table->TimeOrdMag) + 1;
                DiscussLeft = Philosopher->DiscussionSize;

                EventWrite(Philosopher->Table->RegHandle,
                        &DiscussionBeginEventDesc,
                        4, Philosopher->DiscussDataDesc);
            }

            ReleaseSemaphore(Philosopher->DiscussLock,
                             1, NULL);  

            Philosopher->DiscussionLength = DiscussTime;

            EventWrite(Philosopher->RegHandle,
                       &DiscussionJoinEventDesc,
                       4, Philosopher->DiscussDataDesc);

            Sleep(DiscussTime);

            EventWrite(Philosopher->RegHandle,
                       &DiscussionLeaveEventDesc,
                       4, Philosopher->DiscussDataDesc);
            
            DiscussLeft--;

            //
            // Log an event if the last person to leave.
            //
            if (DiscussLeft == 0) {
                Philosopher->Table->TotalDiscussions++;

                EventWrite(Philosopher->Table->RegHandle,
                           &DiscussionEndEventDesc,
                           4, Philosopher->DiscussDataDesc);

            }
            break;
    }
}


//
// Eat something
//

void Eat(PHILOSOPHER *Philosopher)
{

    EventWrite(Philosopher->RegHandle,
               &EatWaitEventDesc,
               2, Philosopher->EatDataDesc);

    //
    // Lock the table to pick up both chopsticks atomically.
    //
    WaitForSingleObject(Philosopher->EatLock,
                        INFINITE);

    TakeChopstick(Philosopher->Left, 
                  Philosopher->Id);
    TakeChopstick(Philosopher->Right,
                  Philosopher -> Id);
    ReleaseSemaphore(Philosopher->EatLock,
                     1, NULL);  
    
    Wait(Philosopher, EAT);
    ReleaseChopstick(Philosopher->Left);
    ReleaseChopstick(Philosopher->Right);

    Philosopher->Meals++;
    Philosopher->Table->TotalMeals++;

}

//
// Think for a while
//

void Think(PHILOSOPHER *Philosopher) 
{

    Wait(Philosopher, THINK);
    Philosopher->Thoughts++;
    Philosopher->Table->TotalThoughts++;

}

//
// Wait to have a discussion. If no one else is ready after a while, give up.
//

void Discuss(PHILOSOPHER *Philosopher) 
{
    DWORD WaitResult;

    EventWrite(Philosopher->RegHandle,
               &DiscussWaitBeginEventDesc,
               4, Philosopher->DiscussWaitDataDesc);

    //
    // If we need all the philosophers to have a discussion, then wait for them.
    // Otherwise, we just wait for the number we need. (only 2 supported, currently)
    //
    if (Philosopher->Table->AllDiscuss == TRUE) {

        //
        // Signal that the philosopher is ready for a discussion.
        // Note that this is an API for windows event objects (for synchronization).
        // It is NOT an API for event tracing.
        //
        SetEvent(Philosopher->DiscussEvent);

        //
        // Wait until all the other philosophers are ready
        //
        WaitResult = WaitForMultipleObjects(Philosopher->Table->MaxPhilosophers - 1,
                                            Philosopher->DiscussEvents,
                                            TRUE, INFINITE);
    } else if (Philosopher->Table->NumDiscuss == 2) {
        
        //
        // Signal that the philosopher is ready for a discussion.
        // Note that this is an API for windows event objects (for synchronization).
        // It is NOT an API for event tracing.
        //
        SetEvent(Philosopher->DiscussEvent);
        
        //
        // Wait until one other philosopher is ready
        //

        WaitResult = WaitForMultipleObjects(Philosopher->Table->MaxPhilosophers - 1,
                                            Philosopher->DiscussEvents,
                                            FALSE, Philosopher->DiscussionTimeout);
        //
        // Reset the event so no one else enters a discussion because of it.
        // Again, this is an API for windows event objects (for synchronization).
        // It is NOT an API for event tracing.
        //
        ResetEvent(Philosopher->DiscussEvent);
    } else {
        WaitResult = WAIT_TIMEOUT;
    }

    //
    // If no one else was ready, there was a timeout, and write the event
    // Otherwise, have the discussion.
    //

    if (WaitResult == WAIT_TIMEOUT) {
        strcpy_s(Philosopher->DiscussWaitStr, 12, "DiscussAbrt");
        EventWrite(Philosopher->RegHandle,
                   &DiscussWaitAbortEventDesc,
                   4, Philosopher->DiscussWaitDataDesc);
        strcpy_s(Philosopher->DiscussWaitStr, 12, "DiscussWait");

    } else {
        EventWrite(Philosopher->RegHandle,
                &DiscussWaitEndEventDesc,
                4, Philosopher->DiscussWaitDataDesc);
        Wait(Philosopher, DISCUSS);
        Philosopher->Discussions++;
    }

    //
    // If we had an all-group discussion, clear the events once the discussion is over.
    //    Clearing it earlier does not work, as all the threads might not wake in time.
    //
    if (Philosopher->Table->AllDiscuss == TRUE) {
        //
        // Again, this is an API for windows event objects (for synchronization).
        // It is NOT an API for event tracing.
        //
        ResetEvent(Philosopher->DiscussEvent);
    }

}

 
//
// The life of a Philosopher
//

void Philosophize (PHILOSOPHER *Philosopher) 
{
    ULONG i = 0;

    EVENT_DATA_DESCRIPTOR PhilArriveDataDesc[1];
    EVENT_DATA_DESCRIPTOR PhilLeaveDataDesc[4];

    //
    // Set up the event desciptors.
    //

    EventDataDescCreate(&(PhilArriveDataDesc[0]),
                        &(Philosopher->Id),
                        sizeof(Philosopher->Id));

    EventDataDescCreate(&(PhilLeaveDataDesc[0]),
                        &(Philosopher->Id),
                        sizeof(Philosopher->Id));

    EventDataDescCreate(&(PhilLeaveDataDesc[1]),
                        &(Philosopher->Meals),
                        sizeof(Philosopher->Meals));

    EventDataDescCreate(&(PhilLeaveDataDesc[2]),
                        &(Philosopher->Thoughts),
                        sizeof(Philosopher->Thoughts));

    EventDataDescCreate(&(PhilLeaveDataDesc[3]),
                        &(Philosopher->Discussions),
                        sizeof(Philosopher->Discussions));


    EventWrite(Philosopher->RegHandle,
               &PhilArriveEventDesc,
               1, PhilArriveDataDesc);

    
    //
    // Set the ActivityID to something distinct, and record it, as the
    //    current reporting tools don't include it in the logs.
    //
    EventActivityIdControl(EVENT_ACTIVITY_CTRL_CREATE_SET_ID,
                           &(Philosopher->ActivityID));

    EventActivityIdControl(EVENT_ACTIVITY_CTRL_GET_ID,
                           &(Philosopher->ActivityID));

    //
    // [[Eat-Think] (repeat)- Discuss] (repeat) cycle
    //
    for (i = 0; i < Philosopher->Table->MaxMeals; i++) {
        Eat(Philosopher);
        
        Think(Philosopher);
        if (((i+1) % Philosopher->Table->MealsPerDiscussion) == 0) {
            Discuss(Philosopher);
            //
            // After a discussion, set the ActivityID to something new.
            //
            EventActivityIdControl(EVENT_ACTIVITY_CTRL_CREATE_SET_ID,
                                   &(Philosopher->ActivityID));

            EventActivityIdControl(EVENT_ACTIVITY_CTRL_GET_ID,
                                   &(Philosopher->ActivityID));

        }
    }

    EventWrite(Philosopher->RegHandle,
               &PhilLeaveEventDesc,
               4, PhilLeaveDataDesc);
}



//
// Simple callback function as an example of uses.
//
void NTAPI
EnableCallback(
    __in_opt LPCGUID                    SourceId,
    __in ULONG                          IsEnabled,
    __in UCHAR                          Level,
    __in ULONGLONG                      OrKeyword,
    __in ULONGLONG                      AndKeyword,
    __in_opt PEVENT_FILTER_DESCRIPTOR   FilterData,
    __in_opt PVOID                      CallbackContext
    )
{
    char GuidStr[64] = "N/A";


    if (SourceId) {
        sprintf_s(GuidStr, 64,
                  "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                  SourceId->Data1, SourceId->Data2,
                  SourceId->Data3,
                  SourceId->Data4[0], SourceId->Data4[1],
                  SourceId->Data4[2], SourceId->Data4[3],
                  SourceId->Data4[4], SourceId->Data4[5],
                  SourceId->Data4[6], SourceId->Data4[7]);
    }

    _tprintf(_T("Enable Callback: SourceId = %s\n\
                 IsEnabled = %s\n\
                 Level = %d\n\
                 OrKeyword = %I64u\n\
                 AndKeyword = %I64u\n\n"),
                  GuidStr, (IsEnabled? "TRUE" : "FALSE"), 
                  (ULONG)Level, OrKeyword, AndKeyword);
}


//
// Main routine; creates philosophers and runs them.
//

int __cdecl main(int argc,
             char **argv) 
{
    TABLE *Table;
    HANDLE* Threads;
    ULONG ThreadId;
    REGHANDLE RegHandle;
    ULONG Status, i = 0;
    
    ULONG NumPhilosophers = NUM_PHILOSOPHERS;
    ULONG NumMeals = EAT_COUNT;
    ULONG MealsPerDiscussion = EATS_PER_DISCUSS;
    ULONG AllDiscuss = ALL_DISCUSS;
    ULONG NumDiscuss = NUM_DISCUSS;
    ULONG TimeOrdMag = TIME_ORD_MAG;
    ULONG TimeDelay = TIME_DELAY;

    LPTSTR *targv = NULL, *utargv = NULL;

#ifdef UNICODE
    if ((targv = CommandLineToArgvW(
                      GetCommandLineW(),    // pointer to a command-line string
                      &argc                 // receives the argument count
                      )) == NULL)
    {
        return(GetLastError());
    };
    utargv = targv;
#else
    targv = argv;
#endif  //UNICODE

    //
    // Process command line arguments to override defaults
    //
    while (--argc > 0)
    {
        targv ++;
        if (**targv == '-' || **targv == '/')
        {
            if (**targv == '/' ) {
                **targv = '-';
            }

            if (!_tcsicmp(targv[0],_T("-delay")) || !_tcsicmp(targv[0],_T("-d"))) {
                if (argc > 1) {
                    targv++; --argc;
                    TimeDelay = _ttoi(targv[0]);
                }
            } else if (!_tcsicmp(targv[0],_T("-phils")) || !_tcsicmp(targv[0],_T("-p"))) {
                if (argc > 1) {
                    targv++; --argc;
                    NumPhilosophers = _ttoi(targv[0]);
                }
            } else if (!_tcsicmp(targv[0],_T("-meals")) || !_tcsicmp(targv[0],_T("-m"))) {
                if (argc > 1) {
                    targv++; --argc;
                    MealsPerDiscussion = _ttoi(targv[0]);
                }
            } else if (!_tcsicmp(targv[0],_T("-sleep")) || !_tcsicmp(targv[0],_T("-s"))) {
                if (argc > 1) {
                    targv++; --argc;
                    TimeOrdMag = _ttoi(targv[0]);
                }
            
            } else if (!_tcsicmp(targv[0],_T("-all")) || !_tcsicmp(targv[0],_T("-a"))) {
                AllDiscuss = TRUE;
            } else {
                printf("Usage: etwphils [options] [number of eat-think cycles]\n");
                printf("\t-d[elay] [n]              delay before starting and stopping (2000)\n");
                printf("\t-p[hils] [n]              number of philosophers (7)\n");
                printf("\t-m[eals] [n]              number of meals per discussion (100)\n");
                printf("\t-s[leep] [n]              order of magnitude for cycles in ms (100)\n");
                printf("\t-a[ll]                    all philosophers must be present in discussion\n");
                printf("\t[cycles]                  number of eat-think cycles (1000)\n");

                return 0;
            }
        } else if (** targv >= '0' && ** targv <= '9') {
            NumMeals = _ttoi(targv[0]);
        }
    }

    if (utargv != NULL) {
        GlobalFree(utargv);
    }


    //
    // Register the process as a provider.
    //
    Status = EventRegister(&ETWPhilsGUID,
                           EnableCallback,
                           NULL,
                           &RegHandle);

    if (Status != ERROR_SUCCESS) {
        _tprintf(_T("ERROR: EventRegisterProvider failed with Status = %d\n"),
                 Status);
        return Status;
    }

    //
    // Wait TimeDelay - allows user to enable logging
    //
    printf("Waiting %d before beginning\n", TimeDelay);
    Sleep(TimeDelay);
    printf("Started.\n\n");

    //
    // Allocate an array for the threads
    //
    Threads = (HANDLE *) malloc(sizeof(HANDLE)*NumPhilosophers);

    //
    // Create a new table.
    //
    Table = NewTable(RegHandle,
                     NumPhilosophers,
                     NumMeals,
                     MealsPerDiscussion,
                     AllDiscuss,
                     NumDiscuss,
                     TimeOrdMag);    
 
    if (Table == NULL) {
        return 1;
    }
    
    //
    // Spawn all of the philosophers as new threads.
    //
    for (i = 0; i < NumPhilosophers; i++) {
        Threads[i] = CreateThread(NULL, 0,
                                  (LPTHREAD_START_ROUTINE)Philosophize,
                                  Table->Philosophers[i], 0,
                                  (PULONG)&ThreadId);
    }

    //
    // Wait for them all to be finished.
    //
    WaitForMultipleObjects(NumPhilosophers,
                           Threads,
                           TRUE, INFINITE);

    //
    // Free the array of threads
    //
    free(Threads);

    //
    // Free the table, then unregister the process.
    //
    FreeTable(Table);
    Status = EventUnregister(RegHandle);
    if (Status != ERROR_SUCCESS) {
        _tprintf(_T("Provider unregistered with Status = %d\n"),
                 Status);
        return Status;
    }

    //
    // Wait TimeDelay - allows user to disable logging
    //
    printf("Finished. Waiting %d before terminating\n", TimeDelay);
    Sleep(TimeDelay);

    return ERROR_SUCCESS;
}
