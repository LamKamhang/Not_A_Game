#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D crosshair;
uniform sampler2D startTexture;
uniform float alpha;
uniform bool start;

void main()
{   
    vec4 fc;
    if(start){
        fc = vec4(vec3(texture(startTexture,vec2((TexCoords.x + 1.0)/2.0,1.0 - (TexCoords.y + 1.0)/2.0))),0.3);
    }else{
        float crossSize = 0.1f;
        vec4 texColor = vec4(0.0,0.0,0.0,0.0);
        if((TexCoords.x > -crossSize && TexCoords.x < crossSize) && (TexCoords.y > -crossSize && TexCoords.y < crossSize) )
            texColor = texture(crosshair, vec2(TexCoords.x + crossSize, TexCoords.y + crossSize) / (2.0*crossSize) );
        fc = texColor + vec4(0.8f,0.0f,0.0f,alpha*length(TexCoords));
    }
    FragColor = fc;
}
