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

enable_language(OBJCXX)

vkb__register_component(
        NAME ios_platform
        HEADERS
        include/ios/context.hpp
        SRC
        src/context.mm
        src/entrypoint.cpp
        LINK_LIBS
        vkb__core
)

# attach to core
target_link_libraries(vkb__core INTERFACE vkb__ios_platform)
