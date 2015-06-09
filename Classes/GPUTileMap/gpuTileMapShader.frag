const char* gpuTileMap_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_pixelCoord;

uniform vec2 u_inverseTileTextureSize;
uniform vec2 u_spriteTextureSize;
uniform vec2 u_inverseSpriteTextureSize;
uniform vec2 u_tileSize;

uniform sampler2D u_sprites;
uniform sampler2D u_tiles;


const vec3 bitSh = vec3(256. *256., 256., 1.);
const vec3 bitShifts = vec3(1.) / bitSh;

float unpack (vec3 color)
{
	return dot(color, bitShifts);
}

float packColor(vec3 color) {
    return (color.r * 256.0 * 256.0) + (color.g * 256.0) + color.b;
}

void main()
{
	vec4 tile = texture2D(u_tiles, v_texCoord);
	gl_FragColor = tile;

	vec2 spriteTextureCords = floor(u_spriteTextureSize / u_tileSize);

	float tileIndex = packColor(floor(tile.rgb * 256.0)) -1.;

	if(tileIndex < 0.)
	{

		discard;
	}

	vec2 spriteOffset = vec2( floor(mod(tileIndex , spriteTextureCords.x)), floor(tileIndex / spriteTextureCords.x)) * u_tileSize;

	vec2 spriteCoord = mod(v_pixelCoord, u_tileSize);
    gl_FragColor = texture2D(u_sprites, (spriteOffset + spriteCoord) * u_inverseSpriteTextureSize);
    //gl_FragColor = tile;
}
);