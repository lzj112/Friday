

aux_source_directory(. FRIDAY_NET)

add_librbary(FNET ${FRIDAY_NET})
target_link_libraries(FNET FBASE)

install(TARGETS FNET DESTINATION lib)