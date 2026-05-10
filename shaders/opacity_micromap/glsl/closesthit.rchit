/* Copyright (c) 2019-2024, Sascha Willems
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

layout(location = 0) rayPayloadInEXT vec3 hitValue;
hitAttributeEXT vec3 attribs;

layout(binding = 2, set = 0) uniform UniformData
{
    mat4 viewInverse;
    mat4 projInverse;
    uint64_t vertexBufferAddress;
    uint64_t indexBufferAddress;
} uniformData;

layout(binding = 3, set = 0) uniform sampler2D maskTexture;

struct Vertex
{
    vec3 pos;
    vec2 uv;
};

layout(buffer_reference, scalar) buffer Vertices { Vertex v[]; };
layout(buffer_reference, scalar) buffer Indices { uint i[]; };

void main()
{
    Vertices vertices = Vertices(uniformData.vertexBufferAddress);
    Indices indices = Indices(uniformData.indexBufferAddress);

    const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);
    const uint indexOffset = 3 * gl_PrimitiveID;
    const uint i0 = indices.i[indexOffset];
    const uint i1 = indices.i[indexOffset + 1];
    const uint i2 = indices.i[indexOffset + 2];
    const vec2 uv = vertices.v[i0].uv * barycentricCoords.x +
                    vertices.v[i1].uv * barycentricCoords.y +
                    vertices.v[i2].uv * barycentricCoords.z;
    hitValue = mix(vec3(uv, 0.0), vec3(1.0), texture(maskTexture, uv).r);
}
