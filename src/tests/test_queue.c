#include <stdio.h>
#include <stdlib.h>

#include "../queue.h"
#include <check.h>

START_TEST (testQueueInitialize) 
{
    Queue q;
    queueInitialize (&q, 10);

    fail_unless (queueElements (&q) == 0, "Elements test failed");
    fail_unless (queueSize (&q) == 10, "Size test failed");
}
END_TEST

START_TEST (testQueueDestroy) 
{
    Queue q;
    queueInitialize (&q, 10);

    queueDestroy (&q);
    
    fail_unless (queueElements (&q) == 0, "Elements test failed");
    fail_unless (queueSize (&q) == 0, "Size test failed");
    fail_unless (q.queue == NULL, "queue pointer test failed");
    
}
END_TEST

START_TEST (testQueuePush) 
{
    Queue q;
    queueInitialize (&q,5);

    //insert some items
    queuePush(&q,'a');
    queuePush(&q,'r');
    queuePush(&q,'u');
    queuePush(&q,'b');
    queuePush(&q,'a');

    printf ("The array is: ");
    int i;
    for (i = 0; i < 5; i++) {
        printf ("%c", q.queue[i]);
    }
    printf ("\n");
    

    fail_unless (q.queue[0] == 'a', "Failed array check test 0");
    fail_unless (q.queue[1] == 'r', "Failed array check test 1");
    fail_unless (q.queue[2] == 'u', "Failed array check test 2");
    fail_unless (q.queue[3] == 'b', "Failed array check test 3");
    fail_unless (q.queue[4] == 'a', "Failed array check test 4");
}
END_TEST

START_TEST (testQueuePop) 
{
    Queue q;
    queueInitialize (&q,5);

    //checking for first item
    fail_unless (queuePop(&q) == 0, "failed NULL pop");

    //insert some items
    queuePush(&q,'a');
    queuePush(&q,'r');
    queuePush(&q,'u');
    queuePush(&q,'b');
    queuePush(&q,'a');

    fail_unless (queuePop(&q) == 'a', "Failed pop test 0");
    fail_unless (queuePop(&q) == 'r', "Failed pop test 1");
    fail_unless (queuePop(&q) == 'u', "Failed pop test 2");
    fail_unless (queuePop(&q) == 'b', "Failed pop test 3");
    fail_unless (queuePop(&q) == 'a', "Failed pop test 4");
    
}
END_TEST

START_TEST (testQueueExpand) 
{
    Queue q;
    queueInitialize (&q,5);

    //insert some items
    queuePush(&q,'a');
    queuePush(&q,'r');
    queuePush(&q,'u');
    queuePush(&q,'b');
    queuePush(&q,'a');

    printf ("The array is: ");
    int i;
    for (i = 0; i < queueElements(&q); i++) {
        printf ("%c", q.queue[i]);
    }
    printf ("\n");

    //trying the auto expand mode
    queuePush(&q,'D');
    queuePush(&q,'u');
    queuePush(&q,'s');
    queuePush(&q,'h');
    queuePush(&q,'i');

    fail_unless (queueElements(&q) == 10, "Failed expansion elemements test");
    fail_unless (queueSize(&q) == 10, "Failed expansion size test");
    

    printf ("The array is: ");
    for (i = 0; i < queueElements(&q); i++) {
        printf ("%c", q.queue[i]);
    }
    printf ("\n");

    fail_unless (q.queue[5] == 'D', "Failed array check test 5");
    fail_unless (q.queue[6] == 'u', "Failed array check test 6");
    fail_unless (q.queue[7] == 's', "Failed array check test 7");
    fail_unless (q.queue[8] == 'h', "Failed array check test 8");
    fail_unless (q.queue[9] == 'i', "Failed array check test 9");

    
}
END_TEST

//creata a suite with the test cases
Suite *queue_suite (void) 
{
    Suite *s = suite_create("Queue");
    TCase *tc_core = tcase_create ("Core");

    //add the test cases
    tcase_add_test (tc_core, testQueueInitialize);
    tcase_add_test (tc_core, testQueueDestroy);
    tcase_add_test (tc_core, testQueuePush);
    tcase_add_test (tc_core, testQueuePop);
    tcase_add_test (tc_core, testQueueExpand);

    //add the test cases to a suite
    suite_add_tcase (s, tc_core);
    return s;
}

