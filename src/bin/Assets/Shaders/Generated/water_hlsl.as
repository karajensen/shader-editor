vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[6], immediateIndexed
dcl_constantbuffer cb2[16], dynamicIndexed
dcl_input v0.xyzw
dcl_input v1.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xyz
dcl_output o1.w
dcl_output o2.xy
dcl_output o2.zw
dcl_output o3.xyz
dcl_output o4.xyz
dcl_output o5.xyz
dcl_output o6.xy
dcl_output o7.xyz
dcl_temps 5
mul r0.x, cb0[5].y, cb2[5].x
mov r1.xy, l(0,0,0,0)
mov r0.y, v0.y
mov r0.z, l(0)
loop 
  ige r0.w, r0.z, l(2)
  breakc_nz r0.w
  mov r2.x, cb2[r0.z + 12].x
  mov r2.y, cb2[r0.z + 14].x
  dp2 r0.w, r2.xyxx, v0.xzxx
  mad r0.w, -cb2[r0.z + 6].x, r0.x, r0.w
  add r0.w, r0.w, cb2[r0.z + 10].x
  sincos r3.x, r4.x, r0.w
  mad r0.y, cb2[r0.z + 8].x, r3.x, r0.y
  mul r0.w, cb2[r0.z + 6].x, cb2[r0.z + 8].x
  mul r0.w, r4.x, r0.w
  mad r1.xy, r0.wwww, r2.xyxx, r1.xyxx
  iadd r0.z, r0.z, l(1)
endloop 
mul r0.xyzw, r0.yyyy, cb2[1].xyzw
mad r0.xyzw, cb2[0].xyzw, v0.xxxx, r0.xyzw
mad r0.xyzw, cb2[2].xyzw, v0.zzzz, r0.xyzw
mad r0.xyzw, cb2[3].xyzw, v0.wwww, r0.xyzw
mul r2.xyzw, r0.yyyy, cb0[1].xyzw
mad r2.xyzw, cb0[0].xyzw, r0.xxxx, r2.xyzw
mad r2.xyzw, cb0[2].xyzw, r0.zzzz, r2.xyzw
mad r2.xyzw, cb0[3].xyzw, r0.wwww, r2.xyzw
mul r3.xy, v1.xyxx, cb2[4].xyxx
mul o2.zw, r3.xxxy, cb2[4].zzzz
mul o6.xy, r3.xyxx, cb2[4].wwww
mov o1.xz, -r1.xxyx
add o7.xyz, -r0.xyzx, cb0[4].xyzx
add r0.w, r2.z, -cb0[4].w
add r3.z, -cb0[4].w, cb0[5].x
div r3.z, l(-1.000000), r3.z
mad o1.w, r0.w, r3.z, l(1.000000)
mov o0.xyzw, r2.xyzw
mov o1.y, l(1.000000)
mov o2.xy, r3.xyxx
mov o3.xyz, r0.xyzx
mov r1.zw, l(0,0,1.000000,0)
mov o4.xyz, r1.zxwz
mov o5.xyz, r1.wyzw
ret 


ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb1[4], immediateIndexed
dcl_constantbuffer cb3[4], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_resource_texturecube (float,float,float,float) t2
dcl_input_ps linear v1.xyz
dcl_input_ps linear v1.w
dcl_input_ps linear v2.xy
dcl_input_ps linear v2.zw
dcl_input_ps linear v3.xyz
dcl_input_ps linear v4.xyz
dcl_input_ps linear v5.xyz
dcl_input_ps linear v6.xy
dcl_input_ps linear v7.xyz
dcl_output o0.xyzw
dcl_output o1.xyzw
dcl_temps 3
sample_indexable(texture2d)(float,float,float,float) r0.xy, v2.xyxx, t1.xyzw, s0
add r0.xy, r0.xyxx, l(-0.500000, -0.500000, 0.000000, 0.000000)
sample_indexable(texture2d)(float,float,float,float) r0.zw, v2.zwzz, t1.zwxy, s0
add r0.xy, r0.xyxx, r0.zwzz
sample_indexable(texture2d)(float,float,float,float) r0.zw, v6.xyxx, t1.zwxy, s0
add r0.xy, r0.xyxx, r0.zwzz
add r0.xy, r0.xyxx, l(-1.000000, -1.000000, 0.000000, 0.000000)
mul r0.xy, r0.xyxx, cb3[3].wwww
dp3 r0.z, v5.xyzx, v5.xyzx
rsq r0.z, r0.z
mul r1.xyz, r0.zzzz, v5.xyzx
mul r0.xzw, r0.xxxx, r1.xxyz
dp3 r1.x, v1.xyzx, v1.xyzx
rsq r1.x, r1.x
mad r0.xzw, v1.xxyz, r1.xxxx, r0.xxzw
dp3 r1.x, v4.xyzx, v4.xyzx
rsq r1.x, r1.x
mul r1.xyz, r1.xxxx, v4.xyzx
mad r0.xyz, r0.yyyy, r1.xyzx, r0.xzwx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul r0.xyz, r0.wwww, r0.xyzx
add r1.xyz, -v3.xyzx, cb1[0].xyzx
dp3 r0.w, r1.xyzx, r1.xyzx
sqrt r1.w, r0.w
div r1.xyz, r1.xyzx, r1.wwww
mad r1.w, cb1[1].y, r1.w, cb1[1].x
mad r0.w, r0.w, cb1[1].z, r1.w
div r0.w, l(1.000000, 1.000000, 1.000000, 1.000000), r0.w
dp3 r1.x, r1.xyzx, r0.xyzx
add r1.x, r1.x, l(1.000000)
mul r1.x, r1.x, cb1[2].x
mul r0.w, r0.w, r1.x
mul r0.w, r0.w, cb1[3].x
sample_indexable(texture2d)(float,float,float,float) r1.x, v2.xyxx, t0.xyzw, s0
mul r0.w, r0.w, r1.x
mul r1.y, r0.w, l(0.500000)
dp3 r0.w, v7.xyzx, v7.xyzx
rsq r0.w, r0.w
mul r2.xyz, r0.wwww, v7.xyzx
dp3_sat r0.w, r2.xyzx, r0.xyzx
add r1.xw, cb3[0].xxxw, -cb3[1].xxxw
mad r1.xw, r0.wwww, r1.xxxw, cb3[1].xxxw
mov r1.z, l(1.000000)
mul r1.yz, r1.xxwx, r1.yyzy
dp3 r0.w, -r2.xyzx, r0.xyzx
add r2.w, r0.w, r0.w
add r0.w, r0.w, l(1.000000)
log r0.w, r0.w
mul r0.w, r0.w, cb3[3].z
exp r0.w, r0.w
mad_sat r0.w, cb3[3].y, r0.w, cb3[3].x
mad r0.xyz, r0.xyzx, -r2.wwww, -r2.xyzx
sample_indexable(texturecube)(float,float,float,float) r0.x, r0.xyzx, t2.xyzw, s0
mul r0.x, r0.x, cb3[2].x
mul r0.x, r0.x, cb3[2].w
mad r1.x, r0.x, r0.w, r1.y
mov r1.w, l(0)
mov o0.xyzw, r1.xwwz
mov o1.xyz, v1.wwww
mov o1.w, l(1.000000)
ret 
