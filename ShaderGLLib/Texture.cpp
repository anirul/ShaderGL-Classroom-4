#include "Texture.h"
#include <algorithm>
#include <functional>
#include <GL/glew.h>
#include "Image.h"
#include "Frame.h"
#include "Render.h"
#include "Program.h"
#include "Mesh.h"

namespace sgl {

	namespace {
		// Get the 6 view for the cube map.
		const std::array<glm::mat4, 6> views_cubemap =
		{
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, -1.0f, 0.0f))
		};
	}

	Texture::Texture(
		const std::string& file, 
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/, 
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/) :
		pixel_element_size_(pixel_element_size),
		pixel_structure_(pixel_structure)
	{
		sgl::Image img(file, pixel_element_size_, pixel_structure_);
		size_ = img.GetSize();
		CreateTexture();
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			sgl::ConvertToGLType(pixel_element_size_, pixel_structure_),
			static_cast<GLsizei>(size_.first),
			static_cast<GLsizei>(size_.second),
			0,
			sgl::ConvertToGLType(pixel_structure_),
			sgl::ConvertToGLType(pixel_element_size_),
			img.Data());
		error_.Display(__FILE__, __LINE__ - 10);
	}

	Texture::Texture(
		const std::pair<std::uint32_t, std::uint32_t> size, 
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/, 
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/) :
		size_(size),
		pixel_element_size_(pixel_element_size),
		pixel_structure_(pixel_structure)
	{
		CreateTexture();
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			sgl::ConvertToGLType(pixel_element_size_, pixel_structure_),
			static_cast<GLsizei>(size_.first),
			static_cast<GLsizei>(size_.second),
			0,
			sgl::ConvertToGLType(pixel_structure_),
			sgl::ConvertToGLType(pixel_element_size_),
			nullptr);
		error_.Display(__FILE__, __LINE__ - 10);
	}

	void Texture::CreateTexture()
	{
		glGenTextures(1, &texture_id_);
		error_.Display(__FILE__, __LINE__ - 1);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture_id_);
	}

	void Texture::Bind(const unsigned int slot /*= 0*/) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		error_.Display(__FILE__, __LINE__ - 1);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	void Texture::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	void Texture::BindEnableMipmap() const
	{
		Bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	TextureManager::~TextureManager()
	{
		DisableAll();
	}

	bool TextureManager::AddTexture(
		const std::string& name, 
		const std::shared_ptr<sgl::Texture>& texture)
	{
		auto ret = name_texture_map_.insert({ name, texture });
		return ret.second;
	}

	const std::shared_ptr<sgl::Texture>& TextureManager::GetTexture(
		const std::string& name) const
	{
		auto it = name_texture_map_.find(name);
		if (it == name_texture_map_.end())
		{
			throw std::runtime_error("No such texture: " + name);
		}
		return it->second;
	}

	bool TextureManager::RemoveTexture(const std::string& name)
	{
		auto it = name_texture_map_.find(name);
		if (it == name_texture_map_.end())
		{
			return false;
		}
		name_texture_map_.erase(it);
		return true;
	}

	const int TextureManager::EnableTexture(const std::string& name) const
	{
		auto it1 = name_texture_map_.find(name);
		if (it1 == name_texture_map_.end())
		{
			throw std::runtime_error("try to enable a texture: " + name);
		}
		for (int i = 0; i < name_array_.size(); ++i)
		{
			if (name_array_[i].empty())
			{
				name_array_[i] = name;
				it1->second->Bind(i);
				return i;
			}
		}
		throw std::runtime_error("No free slots!");
	}

	void TextureManager::DisableTexture(const std::string& name) const
	{
		auto it1 = name_texture_map_.find(name);
		if (it1 == name_texture_map_.end())
		{
			throw std::runtime_error("no texture named: " + name);
		}
		auto it2 = std::find_if(
			name_array_.begin(),
			name_array_.end(),
			[name](const std::string& value)
		{
			return value == name;
		});
		if (it2 != name_array_.end())
		{
			*it2 = "";
			it1->second->UnBind();
		}
		else
		{
			throw std::runtime_error("No slot bind to: " + name);
		}
	}

	void TextureManager::DisableAll() const
	{
		for (int i = 0; i < 32; ++i)
		{
			if (!name_array_[i].empty())
			{
				DisableTexture(name_array_[i]);
			}
		}
	}

	TextureCubeMap::TextureCubeMap(
		const std::array<std::string, 6>& cube_file,
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/,
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/) :
		Texture(pixel_element_size, pixel_structure)
	{
		CreateTextureCubeMap();
		for (const int i : {0, 1, 2, 3, 4, 5})
		{
			sgl::Image image(
				cube_file[i],
				pixel_element_size_,
				pixel_structure_);
			auto size = image.GetSize();
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				sgl::ConvertToGLType(pixel_element_size_, pixel_structure_),
				static_cast<GLsizei>(size.first),
				static_cast<GLsizei>(size.second),
				0,
				sgl::ConvertToGLType(pixel_structure_),
				sgl::ConvertToGLType(pixel_element_size_),
				image.Data());
			error_.Display(__FILE__, __LINE__ - 10);
		}
	}

	TextureCubeMap::TextureCubeMap(
		const std::string& file_name, 
		const std::pair<std::uint32_t, std::uint32_t> size,
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/, 
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/) :
		Texture(pixel_element_size, pixel_structure)
	{
#pragma message ("You have to complete this code!")
	}

	TextureCubeMap::TextureCubeMap(
		const std::pair<std::uint32_t, std::uint32_t> size, 
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/, 
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/) :
		Texture(pixel_element_size, pixel_structure)
	{
		size_ = size;
		CreateTextureCubeMap();
		Bind();
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				ConvertToGLType(pixel_element_size_, pixel_structure_),
				static_cast<GLsizei>(size_.first),
				static_cast<GLsizei>(size_.second),
				0,
				ConvertToGLType(pixel_structure_),
				ConvertToGLType(pixel_element_size_),
				nullptr);
			error_.Display(__FILE__, __LINE__ - 10);
		}
	}

	void TextureCubeMap::Bind(const unsigned int slot /*= 0*/) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		error_.Display(__FILE__, __LINE__ - 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	void TextureCubeMap::UnBind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	void TextureCubeMap::BindEnableMipmap() const
	{
		Bind();
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		error_.Display(__FILE__, __LINE__ - 1);
	}

	void TextureCubeMap::CreateTextureCubeMap()
	{
		glGenTextures(1, &texture_id_);
		error_.Display(__FILE__, __LINE__ - 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		error_.Display(__FILE__, __LINE__ - 1);
		glTexParameteri(
			GL_TEXTURE_CUBE_MAP,
			GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE);
		error_.Display(__FILE__, __LINE__ - 4);
		glTexParameteri(
			GL_TEXTURE_CUBE_MAP,
			GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);
		error_.Display(__FILE__, __LINE__ - 4);
		glTexParameteri(
			GL_TEXTURE_CUBE_MAP,
			GL_TEXTURE_WRAP_R,
			GL_CLAMP_TO_EDGE);
		error_.Display(__FILE__, __LINE__ - 4);
	}

	std::shared_ptr<sgl::Texture> CreateProgramTexture(
		const TextureManager& texture_manager,
		const std::vector<std::string>& texture_selected,
		const std::shared_ptr<Program>& program,
		const std::pair<std::uint32_t, std::uint32_t> size,
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/,
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/)
	{
		return CreateProgramTextureMipmap(
			texture_manager,
			texture_selected,
			program,
			size,
			0,
			[](const int, const std::shared_ptr<sgl::Program>&) {},
			pixel_element_size,
			pixel_structure);
	}

	std::shared_ptr<sgl::Texture> CreateProgramTextureMipmap(
		const TextureManager& texture_manager, 
		const std::vector<std::string>& texture_selected, 
		const std::shared_ptr<Program>& program, 
		const std::pair<std::uint32_t, std::uint32_t> size, 
		const int mipmap,
		const std::function<void(
			const int mipmap,
			const std::shared_ptr<sgl::Program>& program)> func /*=
				[](const int, const std::shared_ptr<sgl::Program>&) {}*/,
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/, 
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/)
	{
#pragma message ("You have to complete this code!")
		return nullptr;
	}

	std::shared_ptr<TextureCubeMap> CreateProgramTextureCubeMap(
		const TextureManager& texture_manager,
		const std::vector<std::string>& texture_selected,
		const std::shared_ptr<Program>& program,
		const std::pair<std::uint32_t, std::uint32_t> size,
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/,
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/)
	{
		return CreateProgramTextureCubeMapMipmap(
			texture_manager,
			texture_selected,
			program,
			size,
			0,
			[](const int, const std::shared_ptr<sgl::Program>&) {},
			pixel_element_size,
			pixel_structure);
	}

	std::shared_ptr<TextureCubeMap> CreateProgramTextureCubeMapMipmap(
		const TextureManager& texture_manager,
		const std::vector<std::string>& texture_selected,
		const std::shared_ptr<Program>& program,
		const std::pair<std::uint32_t, std::uint32_t> size,
		const int mipmap,
		const std::function<void(
			const int mipmap,
			const std::shared_ptr<sgl::Program>& program)> func /*=
				[](const int, const std::shared_ptr<sgl::Program>&) {}*/,
		const PixelElementSize pixel_element_size /*= PixelElementSize::BYTE*/,
		const PixelStructure pixel_structure /*= PixelStructure::RGB*/)
	{
#pragma message ("You have to complete this code!")
		return nullptr;
	}

} // End namespace sgl.
