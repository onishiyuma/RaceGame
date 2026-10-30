#pragma once

namespace nsK2Engine {
	/// <summary>
	/// 処理を遅延するクラス。
	/// </summary>
	class DelayTime : public Noncopyable
	{
	public:
		DelayTime() {};/*コンストラクタ。*/
		~DelayTime() {};/*デストラクタ。*/

	public:

		/// <summary>
		/// 実行処理。
		/// </summary>
		/// <param name="delayTime">遅延時間。</param>
		/// <returns>trueなら遅延時間を過ぎている。</returns>
		bool Execute(float delayTime);

	private:
		Stopwatch m_stopWatch;/*ストップウォッチ。*/
	};
}

