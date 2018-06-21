#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D crosshair;
uniform float alpha;
void main()
{   
    float crossSize = 0.1f;
    vec4 texColor = vec4(0.0,0.0,0.0,0.0);
    if((TexCoords.x > -crossSize && TexCoords.x < crossSize) && (TexCoords.y > -crossSize && TexCoords.y < crossSize) )
        texColor = texture(crosshair, vec2(TexCoords.x + crossSize, TexCoords.y + crossSize) / (2.0*crossSize) );
    FragColor = texColor + vec4(0.8f,0.0f,0.0f,alpha*length(TexCoords));
}
