# Copyright (c) 2023-2025, Thomas Atkinson
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 the "License";
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

vkb__register_component(
    NAME filesystem
    HEADERS
        include/filesystem/filesystem.hpp
        include/filesystem/legacy.h
        # private
        src/std_filesystem.hpp
    SRC
        src/legacy.cpp
        src/filesystem.cpp
        src/std_filesystem.cpp
    LINK_LIBS
        vkb__core
        stb
)

# GCC 9.0 and later has std::filesystem in the stdc++ library
# Earlier versions require linking against stdc++fs
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
   target_link_libraries(vkb__filesystem PRIVATE stdc++fs)
endif()
