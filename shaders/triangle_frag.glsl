#version 330 core

out vec4 FragColor;

in float Height;
in vec3 Position;
uniform vec3 LightDirection;
uniform float LightIntensity;

void main()
{


	vec3 N = normalize(Position);

	// inverte a direção para calcular o Lambertiano
	vec3 L = -normalize(LightDirection);
	
	// Dot entre dois vetores unitários é equivalente ao cosseno entre esses vetores
	// Quanto maior o ângulo entre os vetores, menor é o cosseno entre eles
	float Lambertian = dot(N, L);

	// O clamp vai manter o valor do Lambertiano entre 0 e 1
	// Se o valor for negativa isso quer dizer que estamos virados pro lado
	// oposto a direção da luz.
	Lambertian = clamp(Lambertian, 0.0, 1.0);
	
	
	 
	
	

    float h = (Height + 16)/32.0f;	// shift and scale the height in to a grayscale value
    FragColor = vec4(h*h*LightIntensity, h*LightIntensity, h*LightIntensity, 1.0);
}