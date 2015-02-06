CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(TIZENID "tizen_id")
SET(SRCS
		${CMAKE_SOURCE_DIR}/src/tizenid/tizenid.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE(FindPkgConfig)
pkg_check_modules(tizenid_pkgs REQUIRED
		dlog
		openssl
		cryptsvc
)

FOREACH(flag ${tizenid_pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -g -fno-omit-frame-pointer -finstrument-functions -fpie")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

ADD_EXECUTABLE(${TIZENID} ${SRCS})
TARGET_LINK_LIBRARIES(${TIZENID} ${tizenid_pkgs_LDFLAGS})

INSTALL(TARGETS ${TIZENID} DESTINATION bin)
