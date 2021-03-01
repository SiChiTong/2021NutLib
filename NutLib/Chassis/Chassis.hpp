/**
 * @file Chassis.hpp
 * @brief 機体足回り基幹
 * @author Horie
 * @date 2020/9
 */
#pragma once

#include "../Global.hpp"
#include "../Coordinate.hpp"
#include "../TimeScheduler.hpp"
#include "../Odmetry.hpp"
#include <memory>

namespace nut{
/**
 * @brief 機体足回り基底純粋仮想クラス
 */
class Chassis{
protected:
	/**
	 * @brief 動作タイプ
	 */
	enum class MoveType : uint8_t{
		stop = 0U,//!< 停止
		velocity//!< 速度入力
	};
	MoveType _move_type = MoveType::stop;
	TimeScheduler<void> _scheduler;
	std::shared_ptr<Odmetry> _odmetry;
	Coordinate<float> _target_velocity;



	/**
	 * @brief 周期コールバック関数
	 */
	virtual void ScheduleTask() = 0;


public:
	/**
	 * @brief コンストラクタ
	 * @param[in] period 制御周期
	 * @param[in] odmetry オドメータインスタンス
	 * @details オドメータを使わない場合はヌルポを入れてください
	 */
	Chassis(MilliSecond<uint32_t> period, const std::shared_ptr<Odmetry>& odmetry)
		: _scheduler([this]{ScheduleTask();}, period), _odmetry(odmetry){

	}
	/**
	 * @brief デストラクタ
	 */
	virtual ~Chassis(){

	}

	/**
	 * @brief 初期化関数
	 */
	void Init(){
		_scheduler.Set();
	}

	/**
	 * @brief 速度入力
	 * @param[in] velocity 速度[m/s],[rad/s]
	 * @return 速度入力可能か
	 */
	virtual bool SetVelocity(Coordinate<float> velocity){
		_target_velocity = velocity;
		return true;
	}

	/**
	 * @brief 速度入力
	 * @param[in] velocity_mps 速度[m/s]
	 * @param[in] rot_radps [rad/s]
	 * @return 速度入力可能か
	 */
	virtual bool SetVelocity(Eigen::Vector2f velocity_mps, float rot_radps){
		_target_velocity.x() = velocity_mps.x();
		_target_velocity.y() = velocity_mps.y();
		_target_velocity.theta() = rot_radps;
		return true;
	}

	/**
	 * @brief 入力速度取得
	 * @return 入力速度[m/s]
	 */
	virtual const Coordinate<float> GetVelocity() const{
		return _target_velocity;
	}
};
}
