#ifdef GL_ES																		
precision mediump float;                                                            
#endif                                                                              
                                                                                    
varying vec2 v_texCoord;                                                            
varying vec4 v_fragmentColor;                                                       
                                                                                    
uniform vec4 u_tintColor;                                                           
//uniform sampler2D CC_Texture0;                                                      
                                                                                    
void main()                                                                         
{                                                                                   
	vec4 color = texture2D(CC_Texture0, v_texCoord);                                
	vec3 tint_color = (u_tintColor - color).rgb * u_tintColor.a;                    
	gl_FragColor = color + (vec4(tint_color,1) * color.a);
	//gl_FragColor = vec4(1);                          
}                                                                                   