

aux_source_directory(. FRIDAY_BASE)

add_librbary(FBASE ${FRIDAY_BASE})
target_link_libraries(FNET FNET)

install(TARGETS FBASE DESTINATION lib)
