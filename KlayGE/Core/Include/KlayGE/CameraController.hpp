// CameraController.hpp
// KlayGE 摄像机控制器类 头文件
// Ver 2.8.0
// 版权所有(C) 龚敏敏, 2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// 增加了timer (2005.8.2)
//
// 2.4.0
// 初次建立 (2005.3.12)
//
// 修改记录
//////////////////////////////////////////////////////////////////////////////////

#ifndef _CAMERACONTROLLER_HPP
#define _CAMERACONTROLLER_HPP

#define KLAYGE_LIB_NAME KlayGE_Core
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/Math.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/Timer.hpp>
#include <KlayGE/Input.hpp>

namespace KlayGE
{
	// 摄像机控制器
	//////////////////////////////////////////////////////////////////////////////////
	class CameraController
	{
	public:
		CameraController();
		virtual ~CameraController();

		void Scalers(float rotationScaler, float moveScaler);

		void AttachCamera(Camera& camera);
		void DetachCamera();

		virtual void Update();

	protected:
		float		rotationScaler_;	// Scaler for rotation
		float		moveScaler_;		// Scaler for movement

		Camera*		camera_;

		Timer timer_;
	};

	class FirstPersonCameraController : public CameraController
	{
	public:
		FirstPersonCameraController();

		void Update();

		void Move(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);

	private:
		float4x4		world_;				// World matrix of the camera (inverse of the view matrix)
		float elapsed_time_;

		enum
		{
			TurnLeftRight,
			TurnUpDown,

			Forward,
			Backward,
			MoveLeft,
			MoveRight,
		};

	private:
		void InputHandler(InputEngine const & sender, InputAction const & action);
	};
}

#endif		// _CAMERACONTROLLER_HPP
