find_package( Threads REQUIRED )

set( DATE_INCLUDE_DIRECTORY "include" )
set( SOURCE_FOLDER "src" )

set( HEADER_FILES
	${DATE_INCLUDE_DIRECTORY}/date/chrono_io.h
	${DATE_INCLUDE_DIRECTORY}/date/date.h
	${DATE_INCLUDE_DIRECTORY}/date/ios.h
	${DATE_INCLUDE_DIRECTORY}/date/islamic.h
	${DATE_INCLUDE_DIRECTORY}/date/iso_week.h
	${DATE_INCLUDE_DIRECTORY}/date/julian.h
	${DATE_INCLUDE_DIRECTORY}/date/tz.h
	${DATE_INCLUDE_DIRECTORY}/date/tz_private.h
)


add_library( date ${HEADER_FILES} ${SOURCE_FOLDER}/tz.cpp )
target_link_libraries( date ${CMAKE_THREAD_LIBS_INIT} )
target_include_directories(date PUBLIC include)

unset(HEADER_FILES)
unset(SOURCE_FOLDER)
unset(DATE_INCLUDE_DIRECTORY)
