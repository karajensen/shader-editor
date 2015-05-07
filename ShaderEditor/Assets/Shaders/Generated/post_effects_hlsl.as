vs_5_0
dcl_globalFlags refactoringAllowed
dcl_input v0.xyzw
dcl_input v1.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
mov o0.xyzw, v0.xyzw
mov o1.xy, v1.xyxx
ret 


ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[7], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_resource_texture2dms(4) (float,float,float,float) t2
dcl_input_ps linear v1.xy
dcl_output o0.xyz
dcl_temps 8
sample_indexable(texture2d)(float,float,float,float) r0.xyz, v1.xyxx, t0.xyzw, s0
sample_indexable(texture2d)(float,float,float,float) r1.xyzw, v1.xyxx, t1.xyzw, s0
mul r2.xy, v1.xyxx, l(800.000000, 600.000000, 0.000000, 0.000000)
ftoi r2.xy, r2.xyxx
mov r2.zw, l(0,0,0,0)
mov r0.w, l(0)
mov r3.x, l(0)
loop 
  ige r3.y, r3.x, l(4)
  breakc_nz r3.y
  ldms_indexable(texture2dms)(float,float,float,float) r3.y, r2.xyzw, t2.yxzw, r3.x
  add r0.w, r0.w, r3.y
  iadd r3.x, r3.x, l(1)
endloop 
mul r2.x, r0.w, l(0.250000)
add r2.y, cb0[2].w, -cb0[3].x
mad r2.z, r0.w, l(0.250000), -r2.y
add r2.y, -r2.y, cb0[2].w
div r2.y, l(1.000000, 1.000000, 1.000000, 1.000000), r2.y
mul_sat r2.y, r2.y, r2.z
mul r3.xyz, r1.xyzx, r2.yyyy
add r2.y, -r2.y, l(1.000000)
mad r2.yzw, r0.xxyz, r2.yyyy, r3.xxyz
add r3.w, cb0[3].z, cb0[3].y
mad r0.w, r0.w, l(0.250000), -r3.w
add r3.w, -r3.w, cb0[3].y
div r3.w, l(1.000000, 1.000000, 1.000000, 1.000000), r3.w
mul_sat r0.w, r0.w, r3.w
mul r4.xyz, r0.wwww, cb0[4].xyzx
add r0.w, -r0.w, l(1.000000)
mad r2.yzw, r2.yyzw, r0.wwww, r4.xxyz
mul r5.xyz, r1.wwww, r1.xyzx
mul r6.xyz, r5.xyzx, cb0[1].wwww
mad_sat r2.yzw, r5.xxyz, cb0[1].wwww, r2.yyzw
add r5.xyz, -cb0[5].xyzx, cb0[6].xyzx
mad r2.yzw, r2.yyzw, r5.xxyz, cb0[5].xxyz
dp3 r0.w, r2.yzwy, l(0.212600, 0.715200, 0.072200, 0.000000)
add r2.yzw, -r0.wwww, r2.yyzw
mad r2.yzw, cb0[2].yyyy, r2.yyzw, r0.wwww
add r5.xyz, r2.yzwy, l(-1.000000, -1.000000, -1.000000, 0.000000)
mul r5.xyz, r5.xyzx, cb0[2].xxxx
mul r5.xyz, r2.yzwy, r5.xyzx
add r7.xyz, r2.yzwy, l(-0.500000, -0.500000, -0.500000, 0.000000)
mad r2.yzw, -r5.xxyz, r7.xxyz, r2.yyzw
mul r0.xyz, r0.xyzx, cb0[0].yyyy
mad r0.xyz, r2.yzwy, cb0[0].xxxx, r0.xyzx
mad r0.xyz, r2.xxxx, cb0[0].zzzz, r0.xyzx
mad r0.xyz, r1.xyzx, cb0[0].wwww, r0.xyzx
mad r0.xyz, r3.xyzx, cb0[1].xxxx, r0.xyzx
mad r0.xyz, r4.xyzx, cb0[1].yyyy, r0.xyzx
mad r0.xyz, r6.xyzx, cb0[1].zzzz, r0.xyzx
mul o0.xyz, r0.xyzx, cb0[2].zzzz
ret 
