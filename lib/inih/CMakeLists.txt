
project(inih)

add_library(inih STATIC source/ini.c)
target_include_directories(${PROJECT_NAME} PUBLIC source)

#install(TARGETS inih DESTINATION lib)
#install(FILES ini.h DESTINATION include)


