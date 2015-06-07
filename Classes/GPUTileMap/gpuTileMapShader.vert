const char* gpuTileMap_vert = STRINGIFY(

\n#ifdef GL_ES\n
attribute mediump vec4 a_position;
attribute mediump vec2 a_texCoord;
attribute mediump vec4 a_color;

varying mediump vec4 v_color;
varying mediump vec2 v_texCoord;
varying mediump vec2 v_pixelCoord;

uniform mediump vec2 u_viewOffset;
uniform mediump vec2 u_viewportSize;
uniform mediump vec2 u_inverseTileTextureSize;
uniform mediump vec2 u_inverseTileSize;

\n#else\n
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec4 v_color;
varying vec2 v_texCoord;
varying vec2 v_pixelCoord;

uniform vec2 u_viewOffset;
uniform vec2 u_viewportSize;
uniform vec2 u_inverseTileTextureSize;
uniform vec2 u_inverseTileSize;
\n#endif\n

void main()
{
	v_pixelCoord = (a_texCoord * u_viewportSize) + u_viewOffset;
	v_texCoord = v_pixelCoord * u_inverseTileTextureSize * u_inverseTileSize;
	gl_Position = CC_MVPMatrix * a_position;
}
);