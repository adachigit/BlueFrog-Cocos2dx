// add by hanyi
"											\n\
#ifdef GL_ES								\n\
precision lowp float;						\n\
#endif										\n\
                                            \n\
varying vec2 v_texCoord;					\n\
uniform	vec4 u_color;						\n\
uniform sampler2D CC_Texture0;				\n\
uniform sampler2D CC_Texture1;				\n\
                                            \n\
void main()									\n\
{											\n\
    vec4 c0 = u_color * texture2D(CC_Texture0, v_texCoord);					\n\
    vec4 c1 = texture2D(CC_Texture1, v_texCoord);                           \n\
    gl_FragColor = c0 * vec4(c1.r, c1.r, c1.r, c1.r);                       \n\
}                                                                           \n\
";