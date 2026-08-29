#pragma once

#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"

namespace nsK2EngineLow {
	/// <summary>
	/// 複数のモデルで共有するメッシュのGPUリソース。
	/// </summary>
	class MeshResource
	{
	public:
		~MeshResource()
		{
			for (auto ib : m_indexBufferArray) {
				delete ib;
			}
		}

		VertexBuffer m_vertexBuffer;
		std::vector<IndexBuffer*> m_indexBufferArray;

		// メッシュ形状から決まる情報なのでskinFlagsも共有
		std::vector<int> m_skinFlags;
	};
}

