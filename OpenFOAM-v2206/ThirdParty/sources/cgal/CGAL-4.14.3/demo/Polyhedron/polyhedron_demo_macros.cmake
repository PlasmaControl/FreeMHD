include(AddFileDependencies)
include(${CGAL_MODULES_DIR}/CGAL_add_test.cmake)

  macro(polyhedron_demo_plugin plugin_name plugin_implementation_base_name)
    cmake_parse_arguments(ARG "" "" "KEYWORDS" ${ARGN})
    list_split(option ARGN_TAIL ${ARG_UNPARSED_ARGUMENTS} )
    if(NOT ${option} STREQUAL "EXCLUDE_FROM_ALL")
      if(NOT ${option} STREQUAL "NO_MOC")
        set(other_sources ${ARG_UNPARSED_ARGUMENTS})
        set(option "")
      else()
        set(other_sources ${ARGN_TAIL})
      endif()
    else()
      set(other_sources ${ARGN_TAIL})
    endif()
    if("${option}" STREQUAL "NO_MOC") 
      set(option "")
      set(moc_file_name "")
    else()
      set(moc_file_name ${plugin_implementation_base_name}.moc )
      qt5_generate_moc( ${plugin_implementation_base_name}.cpp "${CMAKE_CURRENT_BINARY_DIR}/${moc_file_name}" )
      add_file_dependencies( ${moc_file_name} "${CMAKE_CURRENT_SOURCE_DIR}/${plugin_implementation_base_name}.cpp" )
    endif()

    add_library(${plugin_name} MODULE ${option} ${moc_file_name} ${plugin_implementation_base_name}.cpp ${other_sources})
    set_property(TARGET ${plugin_name}
      PROPERTY LIBRARY_OUTPUT_DIRECTORY
      "${CGAL_POLYHEDRON_DEMO_PLUGINS_DIR}")
    cgal_add_compilation_test(${plugin_name})
    if(BUILD_TESTING)
      set_property(TEST compilation_of__${plugin_name}
        APPEND PROPERTY DEPENDS compilation_of__test_items)
    endif()
    add_to_cached_list( CGAL_EXECUTABLE_TARGETS ${plugin_name} )
    # Link with Qt
    target_link_libraries( ${plugin_name} PUBLIC ${QT_LIBRARIES} )
    # Link with the demo_framework
    if(TARGET demo_framework)
      target_link_libraries( ${plugin_name} PUBLIC demo_framework)
    else()
      target_link_libraries( ${plugin_name} PUBLIC Polyhedron_demo_framework)
    endif()
    # Link with CGAL
    target_link_libraries( ${plugin_name} PUBLIC ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
    if(TARGET Polyhedron_3)
      add_dependencies( ${plugin_name} Polyhedron_3 )
    endif()
    #metadata management
    #create "${plugin_implementation_base_name}.json" in BINARY_DIR
    STRING(TOLOWER "${plugin_implementation_base_name}.json" base_name)
    SET(filename "${CMAKE_CURRENT_BINARY_DIR}/${base_name}")
    LIST(LENGTH ARG_KEYWORDS size)
    if(${size} GREATER 0)
      SET(keywords )
      FILE(WRITE ${filename} "{ \"Keywords\" : [")
      foreach(keyword ${ARG_KEYWORDS})
        LIST(APPEND keywords "\"${keyword}\", ")
        if(NOT TARGET ${keyword})
          add_custom_target(${keyword})
        endif()
        add_dependencies( ${keyword} ${plugin_name})
      endforeach()
      LIST(LENGTH keywords size)
      math(EXPR size "${size} - 1") 
      LIST(GET keywords -1 last_element)
      LIST(REMOVE_AT keywords ${size})
      STRING(LENGTH ${last_element} size)
      math(EXPR size "${size} - 2") 
      STRING(SUBSTRING ${last_element} 0 ${size} last_element)
      LIST(APPEND keywords ${last_element})
      foreach(keyword ${keywords})
        file(APPEND ${filename} ${keyword})
      endforeach()
      file(APPEND ${filename} "], \n")
      string(TIMESTAMP VERSION "%Y-%m-%d %H:%M")
      file(APPEND ${filename} "\"ConfigDate\" : \"${VERSION}\" }")
    endif()
  endmacro(polyhedron_demo_plugin)
