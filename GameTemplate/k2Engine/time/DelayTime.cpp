#include "k2EnginePreCompile.h"
#include "DelayTime.h"

namespace nsK2Engine {
	//実行処理。
	bool DelayTime::Execute(float delayTime)
	{
        //ストップウォッチが停止している場合、計測を開始する
        if (m_stopWatch.GetElapsed() <= 0.0) {
            m_stopWatch.Start();
        }

        //経過時間を取得
        double elapsedTime = m_stopWatch.GetElapsed();

        //経過時間が遅延時間を超えた場合
        if (elapsedTime >= delayTime) {
            return true;
        }

        m_stopWatch.Stop();
        return false;
	}
}