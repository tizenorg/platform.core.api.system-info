CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(UPDATE_DB "system_info_update_db")

SET(SRCS ${CMAKE_SOURCE_DIR}/src/update_db/system_info_db_update.c)

INCLUDE_DIRECTORIES(include)

INCLUDE(FindPkgConfig)
#pkg_check_modules(update_db_pkgs REQUIRED
#		dlog
#		glib-2.0
#)

#FOREACH(flag ${update_db_pkgs_CFLAGS})
#	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
#ENDFOREACH(flag)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -g -fno-omit-frame-pointer -finstrument-functions -fpie")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

ADD_EXECUTABLE(${UPDATE_DB} ${SRCS})
#TARGET_LINK_LIBRARIES(${UPDATE_DB} ${update_db_pkgs_LDFLAGS} "-lgdbm")
TARGET_LINK_LIBRARIES(${UPDATE_DB})

INSTALL(TARGETS ${INIT_DB} DESTINATION bin)
