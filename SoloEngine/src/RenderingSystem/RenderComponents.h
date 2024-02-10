#pragma once
#include "VertexArrayObject.h"
#include "Texture.h"
#include "Uniform.h"
#include "Image.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
namespace Solo
{
	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;

		std::shared_ptr<VertexArrayObject> vao;
	};

	struct MaterialComponent
	{
		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;

		//std::shared_ptr<Material> material;
	};

	struct TextureComponent
	{
		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;

		std::shared_ptr<Texture> texture;
	};



	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		inline void setTranslation(const glm::vec3 &translation)
		{
			translation_ = translation;
			needsUpdate_ = true;

		};

		inline void setRotation(const glm::vec3 &rotation)
		{
			rotation_ = rotation;
			needsUpdate_ = true;

		};

		inline void setScale(const glm::vec3 &scale)
		{
			scale_ = scale;
			needsUpdate_ = true;

		};
		

		inline const glm::mat4& getTransform()
		{
			if (needsUpdate_)
			{
				glm::mat4 dcm = glm::toMat4(glm::quat(rotation_));
				transform_ = glm::translate(glm::mat4(1.0f), translation_)
					* dcm
					* glm::scale(glm::mat4(1.0f), scale_);
				needsUpdate_ = false;
			}
			return transform_;
		};

	private:

		glm::mat4 transform_ = {};

		glm::vec3 translation_ = { 0.0,0.0,0.0 };
		glm::vec3 rotation_ = { 0.0,0.0,0.0 };
		glm::vec3 scale_ = { 0.0,0.0,0.0 };
		bool needsUpdate_ = false; /// This needs to be set if any of the public data are updated
		
	};
}
