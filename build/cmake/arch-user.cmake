﻿arch("user" "arch/user")

add_definitions(
  -DARCH_USER
)

if(NOT WIN32)
  target_link_libraries(kernel dl)
else()
  kernel_cflags(
    /ZW:nostdlib
    /MT
    /nodefaultlib
    /GS-
    /Oi-
  )

  string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

  ldflags(
    /FORCE:MULTIPLE
  )
endif()