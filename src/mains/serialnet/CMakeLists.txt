
file(GLOB serialnet_SRC
    "*.h"
    "*.cpp"
)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(serialnet ${serialnet_SRC})
target_link_libraries (serialnet core)
target_link_libraries (serialnet master)
target_link_libraries (serialnet interfaces)
target_link_libraries (serialnet boost_program_options)
target_link_libraries (serialnet ev)
target_link_libraries (serialnet serial)
target_link_libraries (serialnet stdstream)
target_link_libraries (serialnet utility)
target_link_libraries (serialnet factories)
target_link_libraries (serialnet tun)
target_link_libraries (serialnet pty)
target_link_libraries (serialnet tap)
target_link_libraries (serialnet hal)
target_link_libraries (serialnet eventwrapper)

