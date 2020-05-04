#include "SceneTest.h"

namespace test {

	TEST_F(SceneTest, CheckConstructorMatrixTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(scene_);
		glm::mat4 test(1.0f);
		scene_ = std::make_shared<sgl::SceneMatrix>(test);
		EXPECT_TRUE(scene_);
	}

	TEST_F(SceneTest, CheckConstructorMeshTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(scene_);
		auto program = sgl::CreateProgram("Simple");
		auto mesh = std::make_shared<sgl::Mesh>(
			"../Asset/Model/Cube.obj", 
			program);
		scene_ = std::make_shared<sgl::SceneMesh>(mesh);
		EXPECT_TRUE(scene_);
	}

	// Simple test scene with populate tree.
	TEST_F(SceneTest, CheckTreeConstructTest)
	{
		EXPECT_EQ(GLEW_OK, glewInit());
		EXPECT_FALSE(scene_tree_);
		scene_tree_ = std::make_shared<sgl::SceneTree>();
		EXPECT_TRUE(scene_tree_);
		PopulateTree();
		unsigned int count_mesh = 0;
		unsigned int count_matrix = 0;
		for (const auto& scene : *scene_tree_)
		{
			const auto& mesh = scene->GetLocalMesh();
			if (mesh)
			{
				count_mesh++;
			}
			else
			{
				count_matrix++;
			}
		}
		EXPECT_EQ(2, count_mesh);
		EXPECT_EQ(2, count_matrix);
		EXPECT_EQ(4, scene_tree_->size());
	}

} // End namespace test.