#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "starslib.h"

void test_starslib_universe_allocation(void)
{
	struct universe* u;
	new_universe(&u, 640, 480, 128);
	CU_ASSERT_PTR_NOT_NULL(u);
	CU_ASSERT_EQUAL(u->width, 640);
	CU_ASSERT_EQUAL(u->height, 480);
	CU_ASSERT_EQUAL(u->depth, 128);
	CU_ASSERT_PTR_NULL(u->iterator);
	CU_ASSERT_PTR_NULL(u->points);
	if (u) free(u);
}

void AddTests(void)
{
	CU_pSuite test_suite;

	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());

	test_suite = CU_add_suite("starslib_universe", NULL, NULL);
	assert(test_suite != NULL);

	CU_add_test(test_suite, "allocation", test_starslib_universe_allocation);
}

int main(int argc, char* argv[])
{
	CU_BOOL Run = CU_FALSE;
	struct CU_FailureRecord* failure_record;

	setvbuf(stdout, NULL, _IONBF, 0);

	if (argc > 1) {
		if (!strcmp("-i", argv[1])) {
			Run = CU_TRUE;
			CU_set_error_action(CUEA_IGNORE);
		}
		else if (!strcmp("-f", argv[1])) {
			Run = CU_TRUE ;
			CU_set_error_action(CUEA_FAIL);
		}
		else if (!strcmp("-A", argv[1])) {
			Run = CU_TRUE ;
			CU_set_error_action(CUEA_ABORT);
		}
		else if (!strcmp("-e", argv[1])) {
			//print_example_results();
		}
		else {
			printf("\nUsage:  AutomatedTest [option]\n\n"
				"        Options: -i  Run, ignoring framework errors [default].\n"
				"                 -f  Run, failing on framework error.\n"
				"                 -A  Run, aborting on framework error.\n"
				"                 -e  Print expected test results and exit.\n"
				"                 -h  Print this message.\n\n");
		}
	}
	else {
		Run = CU_TRUE;
		CU_set_error_action(CUEA_IGNORE);
	}

	if (CU_TRUE == Run) {
		if (CU_initialize_registry()) {
			printf("\nInitialization of Test Registry failed.");
		}
		else {
			AddTests();
			CU_set_output_filename("test_results/starslib");
			CU_list_tests_to_file();
			CU_automated_run_tests();
			failure_record = CU_get_failure_list();
			if (failure_record == NULL) {
				printf("Success\n");
			} else {
				printf("Failures:\n");
				struct CU_FailureRecord* p=failure_record;
				while (p != NULL) {
					printf(" %s::%s %s:%d %s\n", p->pSuite->pName, p->pTest->pName, p->strFileName, p->uiLineNumber, p->strCondition);
					p = p->pNext;
				}
			}
			CU_cleanup_registry();
		}
	}
	if (failure_record == NULL) {
		return 0;
	} else {
		return 1;
	}
}
