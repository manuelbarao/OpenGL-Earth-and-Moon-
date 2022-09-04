
#include "headerfiles/matematica.h"




void cMatematica::GenerateSphere(GLuint Resolution, std::vector<Vertex>& Vertices, std::vector<Triangle>& Indices)
{
	Vertices.clear();
	Indices.clear();

	constexpr float Pi = glm::pi<float>();
	constexpr float TwoPi = glm::two_pi<float>();
	float InvResolution = 1.0f / static_cast<float>(Resolution - 1);

	for (GLuint UIndex = 0; UIndex < Resolution; ++UIndex)
	{
		const float U = UIndex * InvResolution;
		const float Theta = glm::mix(0.0f, TwoPi, static_cast<float>(U));

		for (GLuint VIndex = 0; VIndex < Resolution; ++VIndex)
		{
			const float V = VIndex * InvResolution;
			const float Phi = glm::mix(0.0f, Pi, static_cast<float>(V));

			glm::vec3 VertexPosition =
			{
				glm::cos(Theta) * glm::sin(Phi),
				glm::sin(Theta) * glm::sin(Phi),
				glm::cos(Phi)
			};

			glm::vec3 VertexNormal = glm::normalize(VertexPosition);

			Vertices.push_back(Vertex{
				VertexPosition,
				VertexNormal,
				glm::vec3{ 1.0f, 1.0f, 1.0f },
				glm::vec2{ 1.0f - U, 1.0f - V }
				});
		}
	}

	for (GLuint U = 0; U < Resolution - 1; ++U)
	{
		for (GLuint V = 0; V < Resolution - 1; ++V)
		{
			GLuint P0 = U + V * Resolution;
			GLuint P1 = U + 1 + V * Resolution;
			GLuint P2 = U + (V + 1) * Resolution;
			GLuint P3 = U + 1 + (V + 1) * Resolution;

			Indices.push_back(Triangle{ P3, P2, P0 });
			Indices.push_back(Triangle{ P1, P3, P0 });
		}
	}
}
