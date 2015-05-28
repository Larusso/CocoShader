#ifdef GL_ES														
attribute mediump vec4 a_position;									
attribute mediump vec2 a_texCoord;									
attribute mediump vec4 a_color;										
																	
varying mediump vec4 v_color;										
varying mediump vec2 v_texCoord;									
																	
#else																
attribute vec4 a_position;											
attribute vec2 a_texCoord;											
attribute vec4 a_color;												
																	
varying vec4 v_color;												
varying vec2 v_texCoord;											
#endif																
																	
void main()															
{																	
	v_color = vec4(a_color.rgb * a_color.a, a_color.a);				
	v_texCoord = a_texCoord;										
																	
	gl_Position = CC_MVPMatrix * a_position;						
}