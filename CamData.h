#pragma once
#ifndef _CAM_DATA_H_
#define _CAM_DATA_H_

#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLMHelper.h"
#include "AACube.h"

namespace VECam
{

	struct Plane
	{
	private:
		float _length;
		glm::vec3 _normal;
		float _p;

	public:
		float A, B, C, D;

		const float& length() const { return _length; }
		const glm::vec3& normal() const { return _normal; }
		const float& p() { return _p; }

		void recalculate()
		{
			_length = glm::sqrt(A * A + B * B + C * C);
			_normal = glm::vec3(A / _length, B / _length, C / _length);
			_p = D / _length;
		}

		// credit: cococubed.asu.edu/code_pages/raybox.shtml
		bool SegmentIntersects(glm::vec3 vA, glm::vec3 vB) const
		{
			float lamb = (_p - glm::dot(_normal, vA)) / glm::dot(_normal, vB - vA);
			return lamb >= 0.0f && lamb <= 1.0f;
		}

	};

	struct FPlanes
	{
		Plane left, right, top, bottom, zNear, zFar;

		Plane& operator[] (int i)
		{
			switch (i)
			{
			case 0: return left;
			case 1: return right;
			case 2: return top;
			case 3: return bottom;
			case 4: return zNear;
			case 5: default: return zFar;
			}
		}

		Plane& get(int i) { return this->operator[](i); }
	};

	struct Frustum
	{
		float fov;
		float aspect;
		float _near;
		float _far;

		FPlanes planes;

		// set planes with View * Projection matrix
		// credit: https://gamedev.stackexchange.com/questions/156743/finding-the-normals-of-the-planes-of-a-view-frustum
		void SetPlanes(glm::mat4 vp)
		{
			// column2 + column3
			planes.zNear.A = vp[2][0] + vp[3][0];
			planes.zNear.B = vp[2][1] + vp[3][1];
			planes.zNear.C = vp[2][2] + vp[3][2];
			planes.zNear.D = vp[2][3] + vp[3][3];

			// column3 - column2
			planes.zFar.A = -vp[2][0] + vp[3][0];
			planes.zFar.B = -vp[2][1] + vp[3][1];
			planes.zFar.C = -vp[2][2] + vp[3][2];
			planes.zFar.D = -vp[2][3] + vp[3][3];

			// column1 + column3
			planes.bottom.A = vp[1][0] + vp[3][0];
			planes.bottom.B = vp[1][1] + vp[3][1];
			planes.bottom.C = vp[1][2] + vp[3][2];
			planes.bottom.D = vp[1][3] + vp[3][3];

			// column3 - column1 
			planes.top.A = -vp[1][0] + vp[3][0];
			planes.top.B = -vp[1][1] + vp[3][1];
			planes.top.C = -vp[1][2] + vp[3][2];
			planes.top.D = -vp[1][3] + vp[3][3];

			// column0 + column3
			planes.left.A = vp[0][0] + vp[3][0];
			planes.left.B = vp[0][1] + vp[3][1];
			planes.left.C = vp[0][2] + vp[3][2];
			planes.left.D = vp[0][3] + vp[3][3];

			// column3 - column0
			planes.right.A = -vp[0][0] + vp[3][0];
			planes.right.B = -vp[0][1] + vp[3][1];
			planes.right.C = -vp[0][2] + vp[3][2];
			planes.right.D = -vp[0][3] + vp[3][3];

			for (int i = 0; i < 6; ++i)
			{
				planes[i].recalculate();
			}
		}

		/*void InitWithFOV(float _fov)
		{
			fov = _fov;
			fovx = glm::asin(1.0f /(glm::tan(fov/2.0f)  * aspect)) * 2.0f;
			auto forward = glm::vec3(0.0f, 0.0f, 1.0f);
			normals.right = GLMHelper::rotateAroundY(forward, -glm::degrees(fovx) / 2.0f - 90.0f);
			normals.left = GLMHelper::rotateAroundY(forward, glm::degrees(fovx) / 2.0f + 90.0f);

			normals.up = GLMHelper::rotateAroundX(forward, -glm::degrees(fov) / 2.0f - 90.0f);
			normals.down = GLMHelper::rotateAroundX(forward, glm::degrees(fov) / 2.0f + 90.0f);
		}*/
	};

	class CamData
	{
	public:
		CamData(GLFWwindow* _window) :
			window(_window)
		{
		}
		~CamData() {}

		glm::mat4 getViewMatrix() const
		{
			return ViewMatrix;
		}

		glm::mat4 getProjectionMatrix() const
		{
			return ProjectionMatrix;
		}

		glm::vec3 getPosition() const
		{
			return position;
		}

		glm::vec3 getDirection() const
		{
			return direction;
		}

		float speed = 20.0f;
		float mouseSpeed = 0.005f;

		Frustum frustum;

		void ComputeMatricesFromInputs();

		bool IsPointInFrustum(glm::vec3 v) const;

		bool IsCubeInFrustum(const VEMath::AACube& cube);

	private:
		GLFWwindow* window;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::vec3 position;
		glm::vec3 direction, right, up;

		float horizontalAngle, verticalAngle;

	};


	static void SetupDefaultCam(CamData& cam)
	{
		cam.frustum.fov = glm::pi<float>() / 4.0f; // glm::radians(45.0f);
		cam.frustum.aspect = 4.0f / 3.0f;
		cam.frustum._near = 0.01f;
		cam.frustum._far = 100.0f;
	}
}
#endif // !_CAM_DATA_H_
