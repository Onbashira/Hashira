#pragma once
#include <memory>
#include <vector>


namespace Hashira {
	class GamePad;

	class GamePadManager
	{
	public:

	private:
		//!システム側で決める接続を許すゲームパッド最大数
		int _maxConectionCount;
		
		//!現在の接続されたゲームパッド
		
		std::vector<std::shared_ptr<GamePad>> _gamePads;
	public:

		/**
		* @fn
		* @brief 各パッドをアップデートする
		*/
		void Update();
		
		/**
		* @fn
		* @brief IDからゲームパッドを取得する
		* @param[in] padID パッドのID
		* @return パッドへの参照
		*/
		std::shared_ptr<GamePad> GetPad(int padID = 0);

		/**
		* @fn
		* @brief パッド全体の初期化
		* @param[in] macConectionCount パッドの数
		*/
		void Initialize(int macConectionCount = 4);

		/**
		* @fn
		* @brief パッド全体の破棄
		*/
		void Discard();

		GamePadManager();

		~GamePadManager();

	private:

		/**
		* @fn
		* @brief パッド全体の更新
		*/
		void AllSetUp();

	};
}
