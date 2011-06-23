#include <stdio.h>
#include <stdlib.h>
#include <check.h>



int main (int argc, char* argv[]) {
       
        int number_failed;
        
        //add the player suite tests
        Suite *s = (Suite *)player_suite();
        SRunner *sr = srunner_create (s);
        
        //add the board suite tests
        srunner_add_suite (sr,(Suite *) boardSuite());

        //add the queue suite
        srunner_add_suite (sr, (Suite *) queue_suite());
        
        
        srunner_run_all (sr, CK_NORMAL);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
         
}
