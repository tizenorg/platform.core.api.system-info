CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(INIT_DB "system_info_init_db")

SET(SRCS ${CMAKE_SOURCE_DIR}/src/init_db/system_info_db_init.c)

INCLUDE_DIRECTORIES(include)

INCLUDE(FindPkgConfig)
pkg_check_modules(init_db_pkgs REQUIRED
		dlog
		glib-2.0
		iniparser
		libxml-2.0
)

FOREACH(flag ${init_db_pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -g -fno-omit-frame-pointer -finstrument-functions -fpie")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

ADD_EXECUTABLE(${INIT_DB} ${SRCS})
TARGET_LINK_LIBRARIES(${INIT_DB} ${init_db_pkgs_LDFLAGS} "-lgdbm")

INSTALL(TARGETS ${INIT_DB} DESTINATION bin)
#INSTALL(FILES ${CMAKE_SOURCE_DIR}/packaging/system_info_init_db.service DESTINATION lib/systemd/system)
