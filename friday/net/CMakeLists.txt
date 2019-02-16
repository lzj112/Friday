

# aux_source_directory(. FRIDAY_NET)

set(NET_SRCS
    Connector.cc
    EpollBase.cc
    EpollEventLoop.cc
    FServer.cc
    HeartBeat.cc
    MyEvent.cc
    SocketTCP.cc
    )

# add_librbary(FNET ${FRIDAY_NET})
add_librbary(FNET ${NET_SRCS})
target_link_libraries(FNET FBASE)

install(TARGETS FNET DESTINATION lib)

set(HEADERS
    Connector.h
    EpollBase.h
    EpollEventLoop.h 
    FServer.h 
    HeartBeat.h 
    MyEvent.h 
    SocketTCP.h 
    )
install(TARGETS ${HEADERS} DESTINATION include/Friday/net)