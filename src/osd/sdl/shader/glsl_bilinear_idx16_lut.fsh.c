const char glsl_bilinear_idx16_lut_fsh_src[] =
"\n"
"#pragma optimize (on)\n"
"#pragma debug (off)\n"
"\n"
"uniform sampler2D color_texture;\n"
"uniform sampler2D colortable_texture;\n"
"uniform vec2      colortable_sz; // ct size\n"
"uniform vec2      colortable_pow2_sz; // pow2 ct size\n"
"uniform vec2      color_texture_pow2_sz; // pow2 tex size\n"
"\n"
"vec4 lutTex2D(in vec2 texcoord)\n"
"{\n"
"	vec4 color_tex;\n"
"	vec2 color_map_coord;\n"
"\n"
"	// normalized texture coordinates ..\n"
"	color_tex = texture2D(color_texture, texcoord);\n"
"\n"
"	// GL_UNSIGNED_SHORT GL_ALPHA in ALPHA16 conversion:\n"
"	// general: f = c / ((2*N)-1), c color bitfield, N number of bits\n"
"	// ushort:  c = ((2**16)-1)*f;\n"
"	color_map_coord.x = floor( 65535.0 * color_tex.a + 0.5 );\n"
"\n"
"	// map it to the 2D lut table\n"
"	color_map_coord.y = floor(color_map_coord.x/colortable_sz.x);\n"
"	color_map_coord.x =   mod(color_map_coord.x,colortable_sz.x);\n"
"\n"
"	return texture2D(colortable_texture, color_map_coord/(colortable_pow2_sz-1.0));\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"	vec2 xy = gl_TexCoord[0].st;\n"
"\n"
"	// mix(x,y,a): x*(1-a) + y*a\n"
"	//\n"
"	// bilinear filtering includes 2 mix:\n"
"	//\n"
"	//   pix1 = tex[x0][y0] * ( 1 - u_ratio ) + tex[x1][y0] * u_ratio\n"
"	//   pix2 = tex[x0][y1] * ( 1 - u_ratio ) + tex[x1][y1] * u_ratio\n"
"	//   fin  =    pix1     * ( 1 - v_ratio ) +     pix2    * v_ratio\n"
"	//\n"
"	// so we can use the build in mix function for these 2 computations ;-)\n"
"	//\n"
"	vec2 uv_ratio     = fract(xy*color_texture_pow2_sz); // xy*color_texture_pow2_sz - floor(xy*color_texture_pow2_sz);\n"
"	vec2 one          = 1.0/color_texture_pow2_sz;\n"
"\n"
"	vec4 col1, col2;\n"
"\n"
"	col1 = mix( lutTex2D(xy                   ), lutTex2D(xy + vec2(one.x, 0.0)), uv_ratio.x);\n"
"	col2 = mix( lutTex2D(xy + vec2(0.0, one.y)), lutTex2D(xy + one             ), uv_ratio.x);\n"
"\n"
"	gl_FragColor =  mix ( col1, col2, uv_ratio.y );\n"
"}\n"
"\n"
;