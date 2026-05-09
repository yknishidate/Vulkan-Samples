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

layout(location = 0) rayPayloadInEXT vec3 hitValue;
hitAttributeEXT vec3 attribs;

layout(binding = 3, set = 0) uniform sampler2D maskTexture;

struct Vertex
{
    vec4 pos;
    vec4 uv;
};

layout(binding = 4, set = 0) readonly buffer VertexBuffer
{
    Vertex vertices[];
} vertexBuffer;

layout(binding = 5, set = 0) readonly buffer IndexBuffer
{
    uint indices[];
} indexBuffer;

void main()
{
    const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);
    const uint indexOffset = 3 * gl_PrimitiveID;
    const uint i0 = indexBuffer.indices[indexOffset];
    const uint i1 = indexBuffer.indices[indexOffset + 1];
    const uint i2 = indexBuffer.indices[indexOffset + 2];
    const vec2 uv = vertexBuffer.vertices[i0].uv.xy * barycentricCoords.x +
                    vertexBuffer.vertices[i1].uv.xy * barycentricCoords.y +
                    vertexBuffer.vertices[i2].uv.xy * barycentricCoords.z;
    hitValue = mix(vec3(uv, 0.0), vec3(1.0), texture(maskTexture, uv).r);
}
