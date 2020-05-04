#include "ShaderTest.h"

namespace test {

	TEST_F(ShaderTest, CreateVertexShaderTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(shader_);
		shader_ = std::make_shared<sgl::Shader>(sgl::ShaderType::VERTEX_SHADER);
		EXPECT_TRUE(shader_);
	}

	TEST_F(ShaderTest, CreateFragmentShaderTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(shader_);
		shader_ = 
			std::make_shared<sgl::Shader>(sgl::ShaderType::FRAGMENT_SHADER);
		EXPECT_TRUE(shader_);
	}

	TEST_F(ShaderTest, LoadFromFileVertexShaderTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(shader_);
		shader_ = std::make_shared<sgl::Shader>(sgl::ShaderType::VERTEX_SHADER);
		EXPECT_TRUE(shader_);
		shader_->LoadFromFile("../Asset/Shader/Simple.vert");
		EXPECT_NE(0, shader_->GetId());
	}

	TEST_F(ShaderTest, LoadFromFileFragmentShaderTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(shader_);
		shader_ = 
			std::make_shared<sgl::Shader>(sgl::ShaderType::FRAGMENT_SHADER);
		EXPECT_TRUE(shader_);
		shader_->LoadFromFile("../Asset/Shader/Simple.frag");
		EXPECT_NE(0, shader_->GetId());
	}

} // End namespace test.
