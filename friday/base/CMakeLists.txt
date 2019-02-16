

# aux_source_directory(. FRIDAY_BASE)

set(BASE_SRCS
    IOBuffer.cc
    Message.cc
    Mutex.cc
    Thread.cc
    ThreadPool.cc
    Timer.cc
    TimerWheel.cc
    )

# add_librbary(FBASE ${FRIDAY_BASE})
add_librbary(FBASE ${BASE_SRCS})

target_link_libraries(FBASE FNET pthread)

install(TARGETS FBASE DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/Friday/base)