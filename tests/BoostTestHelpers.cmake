
function(boost_discover_tests test_command)

    set(test_sources ${ARGV})
    list(POP_FRONT test_sources)

    set(test_suite_regex "BOOST_AUTO_TEST_SUITE\\( *([A-Za-z_0-9]+) *\\)")
    set(test_case_regex "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)")
    set(test_suite_end_regex "BOOST_AUTO_TEST_SUITE_END\\( *\\)")

    set(
        tests_regex
        "(${test_suite_regex})|(${test_case_regex})|(${test_suite_end_regex})"
    )

    foreach (source_file_name ${test_sources})

        file(READ ${source_file_name} file_content)
        string(REGEX MATCHALL ${tests_regex} found_tests ${file_content})

        foreach(hit ${found_tests})

            set(test_name_regex ".*\\( *([A-Za-z_0-9]+) *\\).*")

            if(${hit} MATCHES ${test_suite_regex})

                string(REGEX REPLACE ${test_name_regex} "\\1" test_suite_name ${hit})

                list(APPEND test_path ${test_suite_name})

            elseif(${hit} MATCHES ${test_case_regex})

                string(REGEX REPLACE ${test_name_regex} "\\1" test_case_name ${hit})
                
                list(APPEND test_path ${test_case_name})
                list(JOIN test_path "/" test_path_str)
                list(POP_BACK test_path)

                add_test(
                    NAME "${test_command}:${test_path_str}" 
                    COMMAND ${test_command}
                    --run_test=${test_path_str} --catch_system_error=yes
                )

            elseif(${hit} MATCHES ${test_suite_end_regex})
                list(POP_BACK test_path)
            endif()

        endforeach()

    endforeach()

endfunction()
